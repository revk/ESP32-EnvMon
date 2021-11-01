// Env sensor log to mariadb from MQTT
// Copyright (c) 2019 Adrian Kennard, Andrews & Arnold Limited, see LICENSE file (GPL)

#include <stdio.h>
#include <string.h>
#include <popt.h>
#include <err.h>
#include <malloc.h>
#include <time.h>
#include <sqllib.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <ajl.h>

typedef struct log_s log_t;
struct log_s {
   log_t *next;
   char *tag;
   time_t when;
   char *co2;
   char *rh;
   char *temp;
   char *heat;
   char *fan;
};
log_t *logs = NULL;

int main(int argc, const char *argv[])
{
   const char *sqlhostname = NULL;
   const char *sqldatabase = "env";
   const char *sqlusername = NULL;
   const char *sqlpassword = NULL;
   const char *sqlconffile = NULL;
   const char *sqltable = "env";
   const char *mqtthostname = "localhost";
   const char *mqttusername = NULL;
   const char *mqttpassword = NULL;
   const char *mqttappname = "Env";
   const char *mqttid = NULL;
   int interval = 300;
   int debug = 0;
   {                            // POPT
      poptContext optCon;       // context for parsing command-line options
      const struct poptOption optionsTable[] = {
         { "sql-conffile", 'c', POPT_ARG_STRING, &sqlconffile, 0, "SQL conf file", "filename" },
         { "sql-hostname", 'H', POPT_ARG_STRING, &sqlhostname, 0, "SQL hostname", "hostname" },
         { "sql-database", 'd', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &sqldatabase, 0, "SQL database", "db" },
         { "sql-username", 'U', POPT_ARG_STRING, &sqlusername, 0, "SQL username", "name" },
         { "sql-password", 'P', POPT_ARG_STRING, &sqlpassword, 0, "SQL password", "pass" },
         { "sql-table", 't', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &sqltable, 0, "SQL table", "table" },
         { "sql-debug", 'v', POPT_ARG_NONE, &sqldebug, 0, "SQL Debug" },
         { "mqtt-hostname", 'h', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &mqtthostname, 0, "MQTT hostname", "hostname" },
         { "mqtt-username", 'u', POPT_ARG_STRING, &mqttusername, 0, "MQTT username", "username" },
         { "mqtt-password", 'p', POPT_ARG_STRING, &mqttpassword, 0, "MQTT password", "password" },
         { "mqtt-appname", 'a', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &mqttappname, 0, "MQTT appname", "appname" },
         { "mqtt-id", 0, POPT_ARG_STRING, &mqttid, 0, "MQTT id", "id" },
         { "interval", 'i', POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &interval, 0, "Recording interval", "seconds" },
         { "debug", 'V', POPT_ARG_NONE, &debug, 0, "Debug" },
         POPT_AUTOHELP { }
      };

      optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);

      int c;
      if ((c = poptGetNextOpt(optCon)) < -1)
         errx(1, "%s: %s\n", poptBadOption(optCon, POPT_BADOPTION_NOALIAS), poptStrerror(c));

      if (poptPeekArg(optCon))
      {
         poptPrintUsage(optCon, stderr, 0);
         return -1;
      }
      poptFreeContext(optCon);
   }
   SQL sql;
   int e = mosquitto_lib_init();
   if (e)
      errx(1, "MQTT init failed %s", mosquitto_strerror(e));
   struct mosquitto *mqtt = mosquitto_new(mqttid, 1, NULL);
   if (mqttusername)
   {
      e = mosquitto_username_pw_set(mqtt, mqttusername, mqttpassword);
      if (e)
         errx(1, "MQTT auth failed %s", mosquitto_strerror(e));
   }
   void connect(struct mosquitto *mqtt, void *obj, int rc) {
      obj = obj;
      rc = rc;
      char *sub = NULL;
      asprintf(&sub, "state/%s/#", mqttappname);
      int e = mosquitto_subscribe(mqtt, NULL, sub, 0);
      if (e)
         errx(1, "MQTT subscribe failed %s (%s)", mosquitto_strerror(e), sub);
      if (debug)
         warnx("MQTT Sub %s", sub);
      free(sub);
      asprintf(&sub, "info/%s/#", mqttappname);
      e = mosquitto_subscribe(mqtt, NULL, sub, 0);
      if (e)
         errx(1, "MQTT subscribe failed %s (%s)", mosquitto_strerror(e), sub);
      if (debug)
         warnx("MQTT Sub %s", sub);
      free(sub);
      sub = "shellies/+/ext_temperatures";
      e = mosquitto_subscribe(mqtt, NULL, sub, 0);
      if (e)
         errx(1, "MQTT subscribe failed %s (%s)", mosquitto_strerror(e), sub);
      if (debug)
         warnx("MQTT Sub %s", sub);
      sub = "tele/+/SENSOR";
      e = mosquitto_subscribe(mqtt, NULL, sub, 0);
      if (e)
         errx(1, "MQTT subscribe failed %s (%s)", mosquitto_strerror(e), sub);
      if (debug)
         warnx("MQTT Sub %s", sub);
      asprintf(&sub, "command/%s/*/send", mqttappname);
      e = mosquitto_publish(mqtt, NULL, sub, 0, NULL, 1, 0);
      if (e)
         errx(1, "MQTT publish failed %s (%s)", mosquitto_strerror(e), sub);
      free(sub);
   }
   void disconnect(struct mosquitto *mqtt, void *obj, int rc) {
      obj = obj;
      rc = rc;
   }
   void message(struct mosquitto *mqtt, void *obj, const struct mosquitto_message *msg) {
      obj = obj;
      char *topic = strdupa(msg->topic);
      if (!msg->payloadlen)
      {
         warnx("No payload %s", topic);
         return;
      }
      char *type = strrchr(topic, '/');
      if (!type)
      {
         warnx("Unknown topic %s", topic);
         return;
      }
      *type++ = 0;
      char *tag = strrchr(topic, '/');
      if (!tag)
      {
         warnx("Unknown topic %s", topic);
         return;
      }
      *tag++ = 0;
      log_t *find(const char *tag) {
         log_t *l;
         for (l = logs; l && strcmp(l->tag, tag); l = l->next);
         if (!l)
         {
            l = malloc(sizeof(*l));
            memset(l, 0, sizeof(*l));
            l->tag = strdup(tag);
            l->next = logs;
            logs = l;
            if (debug)
               warnx("New device [%s]", tag);
         }
         return l;
      }
      if (debug)
         warnx("Tag [%s] Type [%s] Val [%.*s]", tag, type, msg->payloadlen, (char *) msg->payload);
      time_t now = (time(0) / interval) * interval;
      void logval(const char *type, char **p, const char *val) {        // Store value
         free(*p);
         if (val)
            *p = strdup((char *) val);
         else
            *p = NULL;
      }
      j_t data = j_create();
      const char *e = j_read_mem(data, msg->payload, msg->payloadlen);
      if (e)
         warnx("Bad JSON [%s] Type [%s] Val [%.*s]", tag, type, msg->payloadlen, (char *) msg->payload);
      else
      {
         void done(log_t * l) {
            if (l->when != now)
            {                   // Log to SQL
               l->when = now;
               sql_safe_query_free(&sql, sql_printf("INSERT IGNORE INTO `%#S` SET `tag`=%#s,`when`=%#T,`temp`=%#s,`rh`=%#s,`co2`=%#s,`heat`=%#s,`fan`=%#s", sqltable, l->tag, now, l->temp, l->rh, l->co2, l->heat, l->fan));
               free(l->rh);
               l->rh = NULL;
               free(l->temp);
               l->temp = NULL;
               free(l->co2);
               l->co2 = NULL;
               free(l->heat);
               l->heat = NULL;
               free(l->fan);
               l->fan = NULL;
            }
         }
         const char *v;
         if (!strcmp(type, "SENSOR"))
         {                      // Tasmota
            for (j_t j = j_first(data); j; j = j_next(j))
            {
               const char *name = j_name(j);
               if (!strncmp(name, "DS18B20", 7) && j_get(j, "Id"))
               {
                  int i = 1;
                  char *tasmota;
                  if (name[7] == '-')
                  {
                     i = atoi(name + 8);
                     if (asprintf(&tasmota, "%s-%d", tag, i) < 0)
                        errx(1, "malloc");
                  } else
                     tasmota = strdup(tag);
                  log_t *l = find(tasmota);
                  if (i == 1)
                     logval("heat", &l->heat, j_test(j, "Switch1", 0) ? "true" : "false");
                  if ((v = j_get(j, "Temperature")))
                  {
                     logval("temp", &l->temp, v);
                     done(l);
                  }
                  free(tasmota);
               }
            }
         } else if (!strcmp(type, "ext_temperatures"))
         {                      // Shelly
            for (j_t j = j_first(data); j; j = j_next(j))
               if (j_get(j, "hwID"))
               {
                  char *shelly;
                  int i = atoi(j_name(j));
                  if (asprintf(&shelly, i ? "%s-%d" : "%s", tag, i + 1) < 0)
                     errx(1, "malloc");
                  log_t *l = find(shelly);
                  if ((v = j_get(j, "tC")))
                  {
                     logval("temp", &l->temp, v);
                     done(l);
                  }
                  free(shelly);
               }
         } else if (!strncmp(topic, "state/", 6) && !strcmp(type, "data"))
         {                      // Env
            log_t *l = find(tag);
            if ((v = j_get(data, "ts")))
            {
               time_t t = j_time(v);
               if (t)
                  now = (t / interval) * interval;
            }
            if ((v = j_get(data, "temp")))
               logval("temp", &l->temp, v);
            if ((v = j_get(data, "rh")))
               logval("rh", &l->rh, v);
            if ((v = j_get(data, "co2")))
               logval("co2", &l->co2, v);
            if ((v = j_get(data, "heat")) && (*v == 't' || !l->heat))
               logval("heat", &l->heat, v);     // True latches
            if ((v = j_get(data, "fan")) && (*v == 't' || !l->fan))
               logval("fan", &l->fan, v);       // True latches
            done(l);
         }
         j_delete(&data);
      }
   }
   mosquitto_connect_callback_set(mqtt, connect);
   mosquitto_disconnect_callback_set(mqtt, disconnect);
   mosquitto_message_callback_set(mqtt, message);
   e = mosquitto_connect(mqtt, mqtthostname, 1883, 60);
   if (e)
      errx(1, "MQTT connect failed (%s) %s", mqtthostname, mosquitto_strerror(e));
   sql_real_connect(&sql, sqlhostname, sqlusername, sqlpassword, sqldatabase, 0, NULL, 0, 1, sqlconffile);
   e = mosquitto_loop_forever(mqtt, -1, 1);
   if (e)
      errx(1, "MQTT loop failed %s", mosquitto_strerror(e));
   mosquitto_destroy(mqtt);
   mosquitto_lib_cleanup();
   sql_close(&sql);
   return 0;
}

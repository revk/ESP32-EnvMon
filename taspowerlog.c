// Tasmota power reporting to database
// Copyright (c) 2022 Adrian Kennard

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

typedef struct device_s device_t;
struct device_s {               // Last reading, so we can record power level for period properly
   device_t *next;
   char *tag;
   time_t last;
   float lastfactor;
};
device_t *devices = NULL;

int main(int argc, const char *argv[])
{
   const char *sqlhostname = NULL;
   const char *sqldatabase = "tasmota";
   const char *sqlusername = NULL;
   const char *sqlpassword = NULL;
   const char *sqlconffile = NULL;
   const char *sqltable = "power";
   const char *mqtthostname = "mqtt";
   const char *mqttusername = NULL;
   const char *mqttpassword = NULL;
   const char *mqttid = NULL;
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
         { "mqtt-id", 0, POPT_ARG_STRING, &mqttid, 0, "MQTT id", "id" },
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
      char *sub = "tele/+/SENSOR";
      int e = mosquitto_subscribe(mqtt, NULL, sub, 0);
      if (e)
         errx(1, "MQTT subscribe failed %s (%s)", mosquitto_strerror(e), sub);
      if (debug)
         warnx("MQTT Sub %s", sub);
      sub = "tele/+/LWT";
      e = mosquitto_subscribe(mqtt, NULL, sub, 0);
      if (e)
         errx(1, "MQTT subscribe failed %s (%s)", mosquitto_strerror(e), sub);
      if (debug)
         warnx("MQTT Sub %s", sub);
   }
   void disconnect(struct mosquitto *mqtt, void *obj, int rc) {
      obj = obj;
      rc = rc;
   }
   void message(struct mosquitto *mqtt, void *obj, const struct mosquitto_message *msg) {
      obj = obj;
      char *topic = strdupa(msg->topic);
      char *tag = strchr(topic, '/');
      if (!tag)
         return;
      tag++;
      if (!strncmp(tag, "EM-", 3))
         tag += 3;
      char *e = strchr(tag, '/');
      if (!e)
         return;
      *e++ = 0;
      if (!strcmp(e, "LWT"))
      {
         for (device_t * d = devices; d; d = d->next)
            if (strstr(tag, d->tag))
               d->last = 0;
         return;
      }
      if (strcmp(e, "SENSOR"))
         return;

      j_t data = j_create();
      if (j_read_mem(data, msg->payload, msg->payloadlen))
      {
         j_delete(&data);
         return;
      }
      time_t ts = j_time(j_get(data, "Time"));
      if (ts)
      {
         j_t energy = j_find(data, "ENERGY");
         if (energy)
         {
            if (debug)
               warnx("%s: %.*s", tag, msg->payloadlen, (char *) msg->payload);
            void process(const char *tag, j_t period, int n) {
               if (!period || !j_isnumber(period))
                  return;
               device_t *d;
               for (d = devices; d && strcmp(d->tag, tag); d = d->next);
               if (!d)
               {
                  d = malloc(sizeof(*d));
                  d->last = 0;
                  d->tag = strdup(tag);
                  d->next = devices;
                  devices = d;
               }
               double wh = strtod(j_val(period), NULL);
               sql_s_t s = { 0 };
               sql_sprintf(&s, "INSERT IGNORE INTO `%#S` SET `device`=%#s,`ts`=%#T,`wh`=%lf", sqltable, tag, ts, wh);
               if (d->last && ts > d->last)
                  sql_sprintf(&s, ",`prev`=%#T,`w`=%lf", d->last, wh * 3600.0 / (ts - d->last));
               void extra(const char *name) {
                  j_t f = j_find(energy, name);
                  if (!f)
                     return;
                  if (n && j_isarray(f))
                     f = j_index(f, n - 1);
                  if (!j_isnumber(f))
                     return;
                  double v = strtod(j_val(f), NULL);
                  if (!strcmp(name, "Factor"))
                  {             // Special handling for power factor as end of high usage get misleadingly low values as factor is a snapshot so at end of sample
                     if (v < 0)
                        v = 0 - v;      // Do factors as positive regardless, we log power usage negative
                     double w = d->lastfactor;
                     d->lastfactor = v;
                     if (d->last && w > v)
                        v = w;  // Max of previous entry
                  }
                  sql_sprintf(&s, ",`%#S`=%lf", name, v);
               }
               extra("Power");
               extra("Voltage");
               extra("Current");
               extra("Factor");
               sql_safe_query_s(&sql, &s);
               d->last = ts;
            }
            j_t period = j_find(energy, "Period");
            if (period)
            {
               if (j_isarray(period))
               {
                  if (strchr(tag, '-'))
                     for (int n = 0; n < j_len(period); n++)
                     {          // Expect enough dashes to make names
                        char *dot = strchr(tag, '-');
                        if (dot)
                           *dot++ = 0;
                        if (*tag)
                           process(tag, j_index(period, n), n + 1);
                        if (dot)
                           tag = dot;
                  } else        // No dots
                     for (int n = 0; n < j_len(period); n++)
                     {
                        char *tag2 = NULL;
                        if (asprintf(&tag2, "%s.%d", tag, n + 1) < 0)
                           errx(1, "malloc");
                        process(tag2, j_index(period, n), n + 1);
                        free(tag2);
                     }
               } else
                  process(tag, period, 0);
            }
         }
      }
      j_delete(&data);
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

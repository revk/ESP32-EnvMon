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

typedef struct vals_s vals_t;
struct vals_s
{
   unsigned char set:1;
   double latest;
   double high;
   double low;
};

typedef struct bools_s bools_t;
struct bools_s
{
   unsigned char set:1;
   unsigned char val:1;
};

typedef struct log_s log_t;
struct log_s
{
   log_t *next;
   char *tag;
   time_t when;
   vals_t co2;
   vals_t rh;
   vals_t als;
   vals_t temp;
   vals_t tempt1;
   vals_t tempt2;
   bools_t heat;
   bools_t fan;
};
log_t *logs = NULL;

int
main (int argc, const char *argv[])
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
   const char *mqttappname2 = "Remote";
   const char *mqttid = NULL;
   int interval = 60;
   int debug = 0;
   poptContext optCon;          // context for parsing command-line options
   {                            // POPT
      const struct poptOption optionsTable[] = {
         {"sql-conffile", 'c', POPT_ARG_STRING, &sqlconffile, 0, "SQL conf file", "filename"},
         {"sql-hostname", 'H', POPT_ARG_STRING, &sqlhostname, 0, "SQL hostname", "hostname"},
         {"sql-database", 'd', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &sqldatabase, 0, "SQL database", "db"},
         {"sql-username", 'U', POPT_ARG_STRING, &sqlusername, 0, "SQL username", "name"},
         {"sql-password", 'P', POPT_ARG_STRING, &sqlpassword, 0, "SQL password", "pass"},
         {"sql-table", 't', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &sqltable, 0, "SQL table", "table"},
         {"sql-debug", 'v', POPT_ARG_NONE, &sqldebug, 0, "SQL Debug"},
         {"mqtt-hostname", 'h', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &mqtthostname, 0, "MQTT hostname", "hostname"},
         {"mqtt-username", 'u', POPT_ARG_STRING, &mqttusername, 0, "MQTT username", "username"},
         {"mqtt-password", 'p', POPT_ARG_STRING, &mqttpassword, 0, "MQTT password", "password"},
         {"mqtt-appname", 'a', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &mqttappname, 0, "MQTT appname", "appname"},
         {"mqtt-appname", 0, POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &mqttappname2, 0, "MQTT appname", "appname"},
         {"mqtt-id", 0, POPT_ARG_STRING, &mqttid, 0, "MQTT id", "id"},
         {"interval", 'i', POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &interval, 0, "Recording interval", "seconds"},
         {"debug", 'V', POPT_ARG_NONE, &debug, 0, "Debug"},
         POPT_AUTOHELP {}
      };

      optCon = poptGetContext (NULL, argc, argv, optionsTable, 0);

      int c;
      if ((c = poptGetNextOpt (optCon)) < -1)
         errx (1, "%s: %s\n", poptBadOption (optCon, POPT_BADOPTION_NOALIAS), poptStrerror (c));

      if (poptPeekArg (optCon))
      {
         poptPrintUsage (optCon, stderr, 0);
         return -1;
      }
   }
   SQL sql;
   int e = mosquitto_lib_init ();
   if (e)
      errx (1, "MQTT init failed %s", mosquitto_strerror (e));
   struct mosquitto *mqtt = mosquitto_new (mqttid, 1, NULL);
   if (mqttusername)
   {
      e = mosquitto_username_pw_set (mqtt, mqttusername, mqttpassword);
      if (e)
         errx (1, "MQTT auth failed %s", mosquitto_strerror (e));
   }
   void connect (struct mosquitto *mqtt, void *obj, int rc)
   {
      obj = obj;
      rc = rc;
      char *sub = NULL;
      asprintf (&sub, "state/%s/#", mqttappname);
      int e = mosquitto_subscribe (mqtt, NULL, sub, 0);
      if (e)
         errx (1, "MQTT subscribe failed %s (%s)", mosquitto_strerror (e), sub);
      if (debug)
         warnx ("MQTT Sub %s", sub);
      free (sub);
      asprintf (&sub, "state/%s/#", mqttappname2);
      e = mosquitto_subscribe (mqtt, NULL, sub, 0);
      if (e)
         errx (1, "MQTT subscribe failed %s (%s)", mosquitto_strerror (e), sub);
      if (debug)
         warnx ("MQTT Sub %s", sub);
      free (sub);
      asprintf (&sub, "info/%s/#", mqttappname);
      e = mosquitto_subscribe (mqtt, NULL, sub, 0);
      if (e)
         errx (1, "MQTT subscribe failed %s (%s)", mosquitto_strerror (e), sub);
      if (debug)
         warnx ("MQTT Sub %s", sub);
      free (sub);
      sub = "shellies/+/ext_temperatures";
      e = mosquitto_subscribe (mqtt, NULL, sub, 0);
      if (e)
         errx (1, "MQTT subscribe failed %s (%s)", mosquitto_strerror (e), sub);
      if (debug)
         warnx ("MQTT Sub %s", sub);
      sub = "info/BLE-Env/+/report";
      e = mosquitto_subscribe (mqtt, NULL, sub, 0);
      if (e)
         errx (1, "MQTT subscribe failed %s (%s)", mosquitto_strerror (e), sub);
      if (debug)
         warnx ("MQTT Sub %s", sub);
      sub = "tele/+/SENSOR";
      e = mosquitto_subscribe (mqtt, NULL, sub, 0);
      if (e)
         errx (1, "MQTT subscribe failed %s (%s)", mosquitto_strerror (e), sub);
      if (debug)
         warnx ("MQTT Sub %s", sub);
      sub = "stat/+/STATUS10";
      e = mosquitto_subscribe (mqtt, NULL, sub, 0);
      if (e)
         errx (1, "MQTT subscribe failed %s (%s)", mosquitto_strerror (e), sub);
      if (debug)
         warnx ("MQTT Sub %s", sub);
      sub = "stat/+/RESULT";
      e = mosquitto_subscribe (mqtt, NULL, sub, 0);
      if (e)
         errx (1, "MQTT subscribe failed %s (%s)", mosquitto_strerror (e), sub);
      if (debug)
         warnx ("MQTT Sub %s", sub);
      asprintf (&sub, "command/%s/*/send", mqttappname);
      e = mosquitto_publish (mqtt, NULL, sub, 0, NULL, 1, 0);
      if (e)
         errx (1, "MQTT publish failed %s (%s)", mosquitto_strerror (e), sub);
      free (sub);
   }
   void disconnect (struct mosquitto *mqtt, void *obj, int rc)
   {
      obj = obj;
      rc = rc;
   }
   void message (struct mosquitto *mqtt, void *obj, const struct mosquitto_message *msg)
   {
      obj = obj;
      char *topic = strdupa (msg->topic);
      if (!msg->payloadlen)
      {
         warnx ("No payload %s", topic);
         return;
      }
      char *type = strrchr (topic, '/');
      if (!type)
      {
         warnx ("Unknown topic %s", topic);
         return;
      }
      *type++ = 0;
      char *tag = strrchr (topic, '/');
      if (!tag)
      {
         warnx ("Unknown topic %s", topic);
         return;
      }
      *tag++ = 0;
      j_t data = j_create ();
      const char *e = j_read_mem (data, msg->payload, msg->payloadlen);
      if (e)
         warnx ("Bad JSON [%s] Type [%s] Val [%.*s]", tag, type, msg->payloadlen, (char *) msg->payload);
      else
      {
         log_t *find (const char *t)
         {
            char *tag = strdupa (t);
            for (char *p = tag; *p; p++)
               if (*p == ' ')
                  *p = '_';
            log_t *l;
            for (l = logs; l && strcmp (l->tag, tag); l = l->next);
            if (!l)
            {
               l = malloc (sizeof (*l));
               memset (l, 0, sizeof (*l));
               l->tag = strdup (tag);
               l->next = logs;
               logs = l;
               if (debug)
                  warnx ("New device [%s]", tag);
            }
            return l;
         }
         if (debug)
            warnx ("Topic [%s] Tag [%s] Type [%s] Val [%.*s]", topic, tag, type, msg->payloadlen, (char *) msg->payload);
         time_t now = (time (0) / interval) * interval;
         void logval (const char *type, vals_t * v, const char *val)
         {                      // Store value
            if (!val)
               return;
            double value = strtod (val, NULL);
            v->latest = value;
            if (!v->set || v->high < value)
               v->high = value;
            if (!v->set || v->low > value)
               v->low = value;
            v->set = 1;
         }
         void clearval (vals_t * v)
         {
            v->set = 0;
         }
         void logbool (const char *type, bools_t * b, int val)
         {
            b->set = 1;
            b->val = (val ? 1 : 0);
         }
         void clearbool (bools_t * b)
         {
         }
         void done (log_t * l, int force)
         {
            if (l->when != now || force)
            {                   // Log to SQL
               l->when = now;
               sql_s_t s = { 0 };
               sql_sprintf (&s, "INSERT IGNORE INTO `%#S` SET `tag`=%#s,`utc`=%#U", sqltable, l->tag, now);
               if (l->temp.set)
                  sql_sprintf (&s, ",`temp`=%lf,`temph`=%lf,`templ`=%lf", l->temp.latest, l->temp.high, l->temp.low);
               if (l->tempt1.set)
                  sql_sprintf (&s, ",`tempt1`=%lf", l->tempt1.latest);
               if (l->tempt2.set)
                  sql_sprintf (&s, ",`tempt2`=%lf", l->tempt2.latest);
               if (l->rh.set)
                  sql_sprintf (&s, ",`rh`=%lf,`rhh`=%lf,`rhl`=%lf", l->rh.latest, l->rh.high, l->rh.low);
               if (l->als.set)
                  sql_sprintf (&s, ",`als`=%lf,`alsh`=%lf,`alsl`=%lf", l->als.latest, l->als.high, l->als.low);
               if (l->co2.set)
                  sql_sprintf (&s, ",`co2`=%lf,`co2h`=%lf,`co2l`=%lf", l->co2.latest, l->co2.high, l->co2.low);
               if (l->heat.set)
                  sql_sprintf (&s, ",`heat`=%#s", l->heat.val ? "true" : "false");
               if (l->fan.set)
                  sql_sprintf (&s, ",`fan`=%#s", l->fan.val ? "true" : "false");
               clearval (&l->temp);
               clearval (&l->tempt1);
               clearval (&l->tempt2);
               clearval (&l->co2);
               clearval (&l->rh);
               clearbool (&l->heat);
               clearbool (&l->fan);
               sql_safe_query_s (&sql, &s);
            }
         }
         const char *v;
         if (!strcmp (topic, "info/BLE-Env"))
         {
            log_t *l = find (j_get (data, "name"));
            if ((v = j_get (data, "temp")))
               logval ("temp", &l->temp, v);
            if ((v = j_get (data, "rh")))
               logval ("rh", &l->rh, v);
            done (l, 1);
         } else if (!strcmp (type, "RESULT"))
         {                      // Tasmota (power control) - log as heat
            j_t j = j_first (data);
            if (j)
            {
               const char *name = j_name (j);
               if (!strncmp (name, "POWER", 5))
               {
                  char *tasmota;
                  if (!name[5])
                     tasmota = strdup (tag);
                  else if (asprintf (&tasmota, "%s-%s", tag, name + 5) < 0)
                     errx (1, "malloc");
                  log_t *l = find (tasmota);
                  const char *p = j_val (j);
                  if (p)
                     logbool ("heat", &l->heat, !strcmp (p, "ON"));
                  free (tasmota);
               }
            }
         } else if (!strcmp (type, "SENSOR") || !strcmp (type, "STATUS10"))
         {                      // Tasmota - temp reporting
            if (!strcmp (type, "STATUS10"))
               data = j_find (data, "StatusSNS");
            now = j_time (j_get (data, "Time"));
            for (j_t j = j_first (data); j; j = j_next (j))
            {
               const char *name = j_name (j);
               if (!strncmp (name, "DS18B20", 7) && j_get (j, "Id"))
               {
                  int i = 1;
                  char *tasmota;
                  if (name[7] == '-')
                  {
                     i = atoi (name + 8);
                     if (asprintf (&tasmota, "%s-%d", tag, i) < 0)
                        errx (1, "malloc");
                  } else
                     tasmota = strdup (tag);
                  log_t *l = find (tasmota);
                  if ((v = j_get (j, "Temperature")))
                  {
                     logval ("temp", &l->temp, v);
                     done (l, 1);
                  }
                  free (tasmota);
               }
            }
         } else if (!strcmp (type, "ext_temperatures"))
         {                      // Shelly
            for (j_t j = j_first (data); j; j = j_next (j))
               if (j_get (j, "hwID"))
               {
                  char *shelly;
                  int i = atoi (j_name (j));
                  if (asprintf (&shelly, i ? "%s-%d" : "%s", tag, i + 1) < 0)
                     errx (1, "malloc");
                  log_t *l = find (shelly);
                  if ((v = j_get (j, "tC")))
                  {
                     logval ("temp", &l->temp, v);
                     done (l, 0);
                  }
                  free (shelly);
               }
         } else if (!strncmp (topic, "state", 5) && (!strcmp (tag, "Env") || !strcmp (tag, "Remote") || !strcmp (type, "data")))
         {                      // Env
            log_t *l = find (!strcmp (tag, "Env") || !strcmp (tag, "Remote") ? type : tag);
            if ((v = j_get (data, "ts")))
            {
               time_t t = j_time (v);
               if (t)
                  now = (t / interval) * interval;
            }
            j_t tt = j_find (data, "temp-target");
            if (tt)
            {
               if (j_isarray (tt))
               {
                  logval ("tempt1", &l->tempt1, j_val (j_index (tt, 0)));
                  logval ("tempt2", &l->tempt2, j_val (j_index (tt, 1)));
               } else
                  logval ("tempt1", &l->tempt1, j_val (tt));
            }
            if ((v = j_get (data, "temp")))
               logval ("temp", &l->temp, v);
            if ((v = j_get (data, "rh")))
               logval ("rh", &l->rh, v);
            if ((v = j_get (data, "co2")))
               logval ("co2", &l->co2, v);
            if ((v = j_get (data, "als")))
               logval ("als", &l->als, v);
            v = j_get (data, "heat");
            logbool ("heat", &l->heat, v && *v == 't');
            v = j_get (data, "fan");
            logbool ("fan", &l->fan, v && *v == 't');
            done (l, 0);
         }
         j_delete (&data);
      }
   }
   mosquitto_connect_callback_set (mqtt, connect);
   mosquitto_disconnect_callback_set (mqtt, disconnect);
   mosquitto_message_callback_set (mqtt, message);
   mosquitto_reconnect_delay_set (mqtt, 5, 300, true);
   e = mosquitto_connect (mqtt, mqtthostname, 1883, 60);
   if (e)
      errx (1, "MQTT connect failed (%s) %s", mqtthostname, mosquitto_strerror (e));
   sql_real_connect (&sql, sqlhostname, sqlusername, sqlpassword, sqldatabase, 0, NULL, 0, 1, sqlconffile);
   e = mosquitto_loop_forever (mqtt, -1, 1);
   if (e)
      errx (1, "MQTT loop failed %s", mosquitto_strerror (e));
   mosquitto_destroy (mqtt);
   mosquitto_lib_cleanup ();
   sql_close (&sql);
   poptFreeContext (optCon);
   return 0;
}

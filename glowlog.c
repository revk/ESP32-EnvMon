// Hilderbrand Glow power logging
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

int main(int argc, const char *argv[])
{
   const char *sqlhostname = NULL;
   const char *sqldatabase = "home";
   const char *sqlusername = NULL;
   const char *sqlpassword = NULL;
   const char *sqlconffile = NULL;
   const char *sqltable = "smartmeter";
   const char *mqtthostname = "mqtt";
   const char *mqttusername = NULL;
   const char *mqttpassword = NULL;
   const char *mqttappname = "glow";
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
         { "mqtt-appname", 'a', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &mqttappname, 0, "MQTT appname", "appname" },
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
      char *sub = "glow/+/SENSOR/#";
      int e = mosquitto_subscribe(mqtt, NULL, sub, 0);
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
      char *ihd = strchr(topic, '/');
      if (!ihd++)
         return;
      char *type = strchr(ihd, '/');
      if (!type)
         return;
      *type++ = 0;
      type = strchr(type, '/');
      if (!type++)
         return;

      j_t data = j_create();
      if (!j_read_mem(data, msg->payload, msg->payloadlen))
      {
         if (debug)
            j_err(j_write_pretty(data, stderr));
         j_t j = j_find(data, type);
         if (j)
         {
            {
               char *m = strstr(type, "meter");
               if (m)
                  *m = 0;
            }
            time_t ts = j_time(j_get(j, "timestamp"));
            const char *power = j_get(j, "power.value");
            j = j_find(j, "energy");
            if (j)
            {
               const char *meter = j_get(j, "import.mpan") ? : j_get(j, "import.mprn");
               const char *import = j_get(j, "import.cumulative");
               const char *export = j_get(j, "export.cumulative");
               const char *importunitrate = j_get(j, "import.price.unitrate");
               const char *exportunitrate = j_get(j, "export.price.unitrate");
               const char *standingcharge = j_get(j, "import.price.standingcharge");
               sql_s_t s = { 0 };
               sql_sprintf(&s, "INSERT INTO `%s` SET `timestamp`=%#U,`meter`=%#s,`type`=%#s", sqltable, ts, meter, type);
               void fields(void) {
                  if (import)
                     sql_sprintf(&s, ",`import`=%s", import);
                  if (export)
                     sql_sprintf(&s, ",`export`=%s", export);
                  if (power)
                     sql_sprintf(&s, ",`power`=%s", power);
                  if (importunitrate)
                     sql_sprintf(&s, ",`importunitrate`=%s", importunitrate);
                  if (exportunitrate)
                     sql_sprintf(&s, ",`exportunitrate`=%s", exportunitrate);
                  if (standingcharge)
                     sql_sprintf(&s, ",`standingcharge`=%s", standingcharge);
               }
               fields();
               sql_sprintf(&s, " ON DUPLICATE KEY UPDATE `type`=%#s", type);
               fields();
               sql_safe_query_s(&sql, &s);
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

// Loading SolarEdge data in to tasmota power recording database
// Copyright (c) 2022 Adrian Kennard

#include <stdio.h>
#include <string.h>
#include <popt.h>
#include <err.h>
#include <malloc.h>
#include <time.h>
#include <sqllib.h>
#include <stdlib.h>
#include <ajlcurl.h>

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
   const char *sekey = NULL;
   int sesite = 0;
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
         { "se-site", 's', POPT_ARG_INT, &sesite, 0, "SolarEdge site code", "N" },
         { "se-key", 'k', POPT_ARG_STRING, &sekey, 0, "SolarEdge API key", "key" },
         { "debug", 'V', POPT_ARG_NONE, &debug, 0, "Debug" },
         POPT_AUTOHELP { }
      };

      optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);

      int c;
      if ((c = poptGetNextOpt(optCon)) < -1)
         errx(1, "%s: %s\n", poptBadOption(optCon, POPT_BADOPTION_NOALIAS), poptStrerror(c));

      if (poptPeekArg(optCon) || !sesite || !sekey)
      {
         poptPrintUsage(optCon, stderr, 0);
         return -1;
      }
      poptFreeContext(optCon);
   }
   SQL sql;
   sql_real_connect(&sql, sqlhostname, sqlusername, sqlpassword, sqldatabase, 0, NULL, 0, 1, sqlconffile);
   SQL_RES *res = sql_safe_query_store_free(&sql, sql_printf("SELECT * FROM `%#S` WHERE `device`='Production' ORDER BY `ts` DESC LIMIT 1", sqltable));
   time_t from = 0;
   if (sql_fetch_row(res))
      from = sql_time(sql_colz(res, "ts"));
   sql_free_result(res);
   CURL *curl = curl_easy_init();
   if (sqldebug)
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
   if (!from)
   {
      j_t j = j_create();
      j_store_string(j, "api_key", sekey);
      const char *err = j_curl_get(curl, j, j, NULL, "https://monitoringapi.solaredge.com/site/%d/dataPeriod", sesite);
      if (err)
         errx(1, "Failed to get data period: %s", err);
      from = j_time(j_get(j, "dataPeriod.startDate"));
      if (debug)
         j_err(j_write_pretty(j, stderr));
      j_free(j);
   }
   j_t j = j_create();
   j_store_string(j, "api_key", sekey);
   j_store_datetime(j, "startTime", from - 15 * 60);
   j_store_datetime(j, "endTime", time(0));
   const char *err = j_curl_get(curl, j, j, NULL, "https://monitoringapi.solaredge.com/site/%d/powerDetails", sesite);
   if (err)
      errx(1, "Failed to get data period: %s", err);
   if (debug)
      j_err(j_write_pretty(j, stderr));
   j_t meters = j_find(j, "powerDetails.meters");
   if (!meters || !j_isarray(meters) || !(meters = j_first(meters)))
      errx(1, "No meters array");
   do
   {
      const char *type = j_get(meters, "type");
      if (!type)
      {
         warnx("No type");
         continue;
      }
      double s = 1;
      if (!strcmp(type, "FeedIn"))
         s = -1;
      j_t v = j_find(meters, "values");
      if (!v || !j_isarray(v) || !(v = j_first(v)))
      {
         warnx("No values");
         continue;
      }
      time_t prev = 0;
      do
      {
         time_t ts = j_time(j_get(v, "date"));
         if (!ts)
            continue;
         const char *value = j_get(v, "value");
         if (!value)
            continue;
         double w = strtod(value, NULL) * s;
         sql_string_t s = { };
         sql_sprintf(&s, "INSERT IGNORE INTO `%#S` SET `device`=%#s,`ts`=%#T", sqltable, type, ts);
         if (prev)
            sql_sprintf(&s, ",`prev`=%#T", prev);
         sql_sprintf(&s, ",`wh`=%lf", w / 4);   // Quarter hour
         sql_sprintf(&s, ",`w`=%lf", w);
         sql_safe_query_s(&sql, &s);
         prev = ts;
      } while ((v = j_next(v)));
   } while ((meters = j_next(meters)));
   j_free(j);
   curl_easy_cleanup(curl);
   sql_close(&sql);
   return 0;
}

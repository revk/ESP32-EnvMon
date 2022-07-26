// Environment graph from mariadb
// Copyright (c) 2019 Adrian Kennard, Andrews & Arnold Limited, see LICENSE file (GPL)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <popt.h>
#include <err.h>
#include <curl/curl.h>
#include <sqllib.h>
#include <axl.h>
#include <math.h>

int debug = 0;

int main(int argc, const char *argv[])
{
   const char *sqlhostname = NULL;
   const char *sqldatabase = "env";
   const char *sqlusername = NULL;
   const char *sqlpassword = NULL;
   const char *sqlconffile = NULL;
   const char *sqltable = "env";
   const char *sqlweather = "weather";
   const char *weathertag = NULL;
   const char *tag = NULL;
   const char *title = NULL;
   const char *control = NULL;
   const char *me = NULL;
   char *tempcol = "#f00";
   char *tempcolo = "#800";
   char *co2col = "#080";
   char *rhcol = "#00f";
   char *date = NULL;
   double xsize = 36;
   double ysize = 36;
   double tempstep = 1;
   double co2step = 50;
   double rhstep = 3;
   double co2line = 1000;
   double templine = 0;
   double rhline = 30;
   int debug = 0;
   int nogrid = 0;
   int noaxis = 0;
   int nodate = 0;
   int days = 1;
   int spacing = 5;
   int only = 0;
   int back = 0;
   int temptop = 0;
   int co2top = 0;
   int rhtop = 0;
   {                            // POPT
      poptContext optCon;       // context for parsing command-line options
      const struct poptOption optionsTable[] = {
         { "sql-conffile", 'c', POPT_ARG_STRING, &sqlconffile, 0, "SQL conf file", "filename" },
         { "sql-hostname", 'H', POPT_ARG_STRING, &sqlhostname, 0, "SQL hostname", "hostname" },
         { "sql-database", 'd', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &sqldatabase, 0, "SQL database", "db" },
         { "sql-username", 'U', POPT_ARG_STRING, &sqlusername, 0, "SQL username", "name" },
         { "sql-password", 'P', POPT_ARG_STRING, &sqlpassword, 0, "SQL password", "pass" },
         { "sql-table", 't', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &sqltable, 0, "SQL table", "table" },
         { "sql-weather", 0, POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &sqlweather, 0, "SQL weather table", "table" },
         { "weather-tag", 0, POPT_ARG_STRING, &weathertag, 0, "SQL weather tag", "tag" },
         { "sql-debug", 'v', POPT_ARG_NONE, &sqldebug, 0, "SQL Debug" },
         { "x-size", 0, POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &xsize, 0, "X size per hour", "pixels" },
         { "y-size", 0, POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &ysize, 0, "Y size per step", "pixels" },
         { "temp-step", 0, POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &tempstep, 0, "Temp per Y step", "Celsius" },
         { "co2-step", 0, POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &co2step, 0, "CO₂ per Y step", "ppm" },
         { "rh-step", 0, POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &rhstep, 0, "RH per Y step", "%" },
         { "temp-line", 0, POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &templine, 0, "Temp line", "Celsius" },
         { "co2-line", 0, POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &co2line, 0, "CO₂ line", "ppm" },
         { "rh-line", 0, POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &rhline, 0, "RH line", "%" },
         { "tag", 'i', POPT_ARG_STRING, &tag, 0, "Device ID", "tag" },
         { "date", 'D', POPT_ARG_STRING, &date, 0, "Date", "YYYY-MM-DD" },
         { "title", 'T', POPT_ARG_STRING, &title, 0, "Title", "text" },
         { "days", 'N', POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &days, 0, "Days", "N" },
         { "temp-top", 0, POPT_ARG_INT, &temptop, 0, "Top temp", "C" },
         { "co2-top", 0, POPT_ARG_INT, &co2top, 0, "Top co2", "PPM" },
         { "rh-top", 0, POPT_ARG_INT, &rhtop, 0, "Top rh", "%" },
         { "back", 0, POPT_ARG_INT, &back, 0, "Back days", "N" },
         { "control", 'C', POPT_ARG_STRING, &control, 0, "Control", "[-]N[T/C/R]" },
         { "spacing", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &spacing, 0, "Spacing", "N" },
         { "no-grid", 0, POPT_ARG_NONE, &nogrid, 0, "No grid" },
         { "no-axis", 0, POPT_ARG_NONE, &noaxis, 0, "No axis" },
         { "no-date", 0, POPT_ARG_NONE, &nodate, 0, "No date" },
         { "debug", 'V', POPT_ARG_NONE, &debug, 0, "Debug" },
         { "me", 0, POPT_ARG_STRING | POPT_ARGFLAG_DOC_HIDDEN, &me, 0, "Me link", "URL" },
         POPT_AUTOHELP { }
      };

      optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);
      //poptSetOtherOptionHelp (optCon, "");

      int c;
      if ((c = poptGetNextOpt(optCon)) < -1)
         errx(1, "%s: %s\n", poptBadOption(optCon, POPT_BADOPTION_NOALIAS), poptStrerror(c));

      if (poptPeekArg(optCon) || !tag)
      {
         poptPrintUsage(optCon, stderr, 0);
         return -1;
      }
      poptFreeContext(optCon);
   }
   enum {
      CO2,
      RH,
      TEMP,
      TEMPO,
      MAX,
   };
#define	ONLY_CO2	(1<<CO2)
#define	ONLY_RH		(1<<RH)
#define	ONLY_TEMP	((1<<TEMP)|(1<<TEMPO))
   struct data_s {
      const char *arg;
      const char *secondary;
      const char *target1;
      const char *target2;
      const char *unit;
      const char *colour;
      xml_t g;
      char *path;               // Main path
      char *path2;              // Secondary path
      char *path3;              // Target path 1
      char *path4;              // Target path 2
      size_t size;
      size_t size2;
      size_t size3;
      size_t size4;
      FILE *f;
      FILE *f2;
      FILE *f3;
      FILE *f4;
      double line;
      double min;
      double max;
      double scale;
      char m;
      char m2;
      char m3;
      char m4;
      double lastx;
      double lasty;
      int count;
   } data[MAX] = {
    { arg: "co2", secondary: "fan", scale: ysize / co2step, line: co2line, unit:"ppm" },
    { arg: "rh", scale: ysize / rhstep, line: rhline, unit:"%" },
    { arg: "temp", secondary: "heat", target1: "tempt1", target2: "tempt2", scale: ysize / tempstep, line: templine, unit:"℃" },
    { arg: "tempc", scale: ysize / tempstep, line: templine, unit:"℃" },
   };

   if (control)
   {
      if (*control == '-')
      {
         control++;
         days = 1;
         if (isdigit(*control))
         {
            back = 0;
            while (isdigit(*control))
               back = back * 10 + *control++ - '0';
         }

      }
      if (isdigit(*control))
      {
         days = 0;
         while (isdigit(*control))
            days = days * 10 + *control++ - '0';
      }
      while (isalpha(*control))
      {
         char t = tolower(*control++);
         int v = 0;
         while (isdigit(*control))
            v = v * 10 + *control++ - '0';
         char *col = NULL;
         if (*control == '=')
         {
            control++;
            col = (char *) control;
            while (isxdigit(*control))
               control++;
            col = strndup(col - 1, control + 1 - col);
            *col = '#';
         }
         if (t == 't')
         {
            only |= ONLY_TEMP;
            temptop = v;
            if (col)
               tempcol = col;
         }
         if (t == 'c')
         {
            only |= ONLY_CO2;
            co2top = v;
            if (col)
               co2col = col;
         }
         if (t == 'r')
         {
            only |= ONLY_RH;
            rhtop = v;
            if (col)
               rhcol = col;
         }
      }
   }
   if (temptop)
   {
      data[TEMP].max = temptop;
      data[TEMPO].max = temptop;
   }
   data[TEMP].colour = tempcol;
   data[TEMPO].colour = tempcolo;
   if (co2top)
      data[CO2].max = co2top;
   data[CO2].colour = co2col;
   if (rhtop)
      data[RH].max = rhtop;
   data[RH].colour = rhcol;
   SQL sql;
   sql_real_connect(&sql, sqlhostname, sqlusername, sqlpassword, sqldatabase, 0, NULL, 0, 1, sqlconffile);
   time_t stime,
    etime;
   char *sdate = NULL;
   char *edate = NULL;
   char *ldate = NULL;
   {
      struct tm t;
      time_t now = time(0);
      if (date)
         now = xml_time(date);
      localtime_r(&now, &t);
      if (back)
         t.tm_mday -= back - 1;
      else
         t.tm_mday -= days - 1;
      t.tm_hour = t.tm_min = t.tm_sec = 0;
      t.tm_isdst = -1;
      stime = mktime(&t);
      asprintf(&date, "%04d-%02d-%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
      sdate = strdup(date);
      t.tm_mday += days;
      t.tm_isdst = -1;
      etime = mktime(&t);
      asprintf(&edate, "%04d-%02d-%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
      t.tm_mday--;
      t.tm_isdst = -1;
      mktime(&t);
      asprintf(&ldate, "%04d-%02d-%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
   }
   int d;
   int day = 0;
   xml_t svg = xml_tree_new("svg");
   if (me)
      xml_addf(svg, "a@rel=me@href", me);
   xml_element_set_namespace(svg, xml_namespace(svg, NULL, "http://www.w3.org/2000/svg"));
   xml_t top = xml_element_add(svg, "g");
   xml_t grid = xml_element_add(top, "g");
   xml_t axis = xml_element_add(top, "g");
   for (d = 0; d < MAX; d++)
   {
      data[d].g = xml_element_add(top, "g");
      xml_add(data[d].g, "@stroke", data[d].colour);
      xml_add(data[d].g, "@fill", "none");
   }
   void sod(void) {
      for (d = 0; d < MAX; d++)
      {
         data[d].f = open_memstream(&data[d].path, &data[d].size);
         data[d].m = 'M';
         if (data[d].secondary)
         {                      // Control trace
            data[d].f2 = open_memstream(&data[d].path2, &data[d].size2);
            data[d].m2 = 'M';
         }
         if (data[d].target1)
         {                      // Target trace
            data[d].f3 = open_memstream(&data[d].path3, &data[d].size3);
            data[d].m3 = 'M';
         }
         if (data[d].target2)
         {                      // Target trace
            data[d].f4 = open_memstream(&data[d].path4, &data[d].size4);
            data[d].m4 = 'M';
         }
      }
   }
   void eod(void) {
      day++;
      for (d = 0; d < MAX; d++)
      {
         if (data[d].target1)
         {
            fclose(data[d].f3);
            if (*data[d].path3)
            {
               xml_t p = xml_element_add(data[d].g, "path");
               xml_addf(p, "@opacity", "%.1f", (double) day / days);
               xml_add(p, "@stroke-dasharray", "1 3");
               xml_add(p, "@d", data[d].path3);
            }
            free(data[d].path3);
         }
         if (data[d].target2)
         {
            fclose(data[d].f4);
            if (*data[d].path4)
            {
               xml_t p = xml_element_add(data[d].g, "path");
               xml_addf(p, "@opacity", "%.1f", (double) day / days);
               xml_add(p, "@stroke-dasharray", "1 3");
               xml_add(p, "@d", data[d].path4);
            }
            free(data[d].path4);
         }
         fclose(data[d].f);
         if (*data[d].path)
         {
            xml_t p = xml_element_add(data[d].g, "path");
            xml_addf(p, "@opacity", "%.1f", (double) day / days);
            xml_add(p, "@d", data[d].path);
            free(data[d].path);
         }
         if (data[d].secondary)
         {
            fclose(data[d].f2);
            if (*data[d].path2)
            {
               xml_t p = xml_element_add(data[d].g, "path");
               xml_addf(p, "@opacity", "%.1f", (double) day / days);
               xml_add(p, "@stroke-width", "2");
               xml_add(p, "@d", data[d].path2);
            }
            free(data[d].path2);
         }
      }
   }
   int maxx = xsize * 24;
   void run(SQL_RES * res) {
      if (!res)
         return;
      sod();
      time_t start = stime;
      while (sql_fetch_row(res))
      {
         const char *when = sql_col(res, "utc");
         double x = (xml_time_utc(when) - start) * xsize / 3600;
         void add(void) {
            for (d = 0; d < MAX; d++)
            {
               if (only && !(only & (1 << d)))
                  continue;
               const char *val = sql_col(res, data[d].arg);
               if (!val)
                  continue;
               double v = strtod(val, NULL);
               if (!data[d].count || data[d].min > v)
                  data[d].min = v;
               if ((!data[d].count && !data[d].max) || data[d].max < v)
                  data[d].max = v;
               data[d].count++;
               double y = v * data[d].scale;
               data[d].lastx = x;
               data[d].lasty = y;
               // Extra
               if (data[d].secondary)
               {                // Control trace
                  char on = (*sql_colz(res, data[d].secondary) == 't');
                  if (on || data[d].m2 == 'L')
                     fprintf(data[d].f2, "%c%.1lf,%.1lf", data[d].m2, x, y);
                  data[d].m2 = (on ? 'L' : 'M');
               }
               fprintf(data[d].f, "%c%.1lf,%.1lf", data[d].m, x, y);
               if (data[d].m2 == 'L')
                  data[d].m = 'M';
               else
                  data[d].m = 'L';
               if (data[d].target1)
               {                // Target trace
                  const char *val = sql_col(res, data[d].target1);
                  if (val)
                  {
                     double v = strtod(val, NULL);
                     double y = v * data[d].scale;
                     fprintf(data[d].f3, "%c%.1lf,%.1lf", data[d].m3, x, y);
                     data[d].m3 = 'L';
                  } else
                     data[d].m3 = 'M';
               }
               if (data[d].target2)
               {                // Target trace
                  const char *val = sql_col(res, data[d].target2);
                  if (val)
                  {
                     double v = strtod(val, NULL);
                     double y = v * data[d].scale;
                     fprintf(data[d].f4, "%c%.1lf,%.1lf", data[d].m4, x, y);
                     data[d].m4 = 'L';
                  } else
                     data[d].m4 = 'M';
               }
            }
         }
         if (x >= maxx)
         {                      // New day
            if (start && x <= floor((maxx + xsize - 1) / xsize) * xsize)
               add();           // End of day
            struct tm t;
            localtime_r(&start, &t);
            t.tm_mday++;
            t.tm_isdst = -1;
            start = mktime(&t);
            eod();
            sod();
            x = 0;
         }
         add();
      }
      sql_free_result(res);
   }
   run(sql_safe_query_store_free(&sql, sql_printf("SELECT * FROM `%#S` WHERE `tag`=%#s AND `utc`>=%#U AND `utc`<=%#U ORDER BY `utc`", sqltable, tag, stime, etime)));
   if (sqlweather && weathertag)
      run(sql_safe_query_store_free(&sql, sql_printf("SELECT * FROM `%#S` WHERE `tag`=%#s AND `utc`>=%#U AND `utc`<=%#U ORDER BY `utc`", sqlweather, weathertag, stime, etime)));
   sql_close(&sql);
   eod();
   for (d = 0; d < MAX; d++)
      if (data[d].lastx && data[d].lastx < 24 * xsize)
      {
         xml_t c = xml_element_add(data[d].g, "circle");
         xml_addf(c, "@cx", "%.1lf", data[d].lastx);
         xml_addf(c, "@cy", "%.1lf", data[d].lasty);
         xml_addf(c, "@r", "%d", (int) (ysize / 6));
         xml_addf(c, "@fill", data[d].colour);
         xml_add(c, "@stroke", "none");
         xml_add(c, "@opacity", "0.5");
      }
   maxx = floor((maxx + xsize - 1) / xsize) * xsize;
   int maxy = 0;
   // Normalise min and work out y size
   int offset = 0;
   for (d = 0; d < MAX; d++)
      if (data[d].count)
      {
         data[d].min = floor(data[d].min * data[d].scale / ysize - 1 + offset) * ysize / data[d].scale;
         data[d].max = floor(data[d].max * data[d].scale / ysize + 1 + offset) * ysize / data[d].scale;
         double v = data[d].max - data[d].min;
         int y = v * data[d].scale - (offset - 1) * ysize;
         if (y > maxy)
            maxy = y;
         offset -= spacing;
      }
   for (d = 0; d < MAX; d++)
      if (data[d].count)
         data[d].max = data[d].min + (double) maxy / data[d].scale;
   if (data[TEMP].max > data[TEMPO].max)
      data[TEMPO].max = data[TEMP].max;
   else
      data[TEMP].max = data[TEMPO].max;
   if (!nogrid)
   {                            // Grid
      xml_add(grid, "@stroke", "black");
      xml_add(grid, "@fill", "none");
      xml_add(grid, "@opacity", "0.1");
      for (int y = 0; y <= maxy; y += ysize)
      {
         xml_t l = xml_element_add(grid, "path");
         xml_addf(l, "@d", "M0,%dL%d,%d", y, maxx, y);
      }
      for (int x = 0; x <= maxx; x += xsize)
      {
         xml_t l = xml_element_add(grid, "path");
         xml_addf(l, "@d", "M%d,0L%d,%d", x, x, maxy);
      }
   }
   if (!noaxis)
   {                            // Axis
      xml_add(axis, "@opacity", "0.5");
      xml_add(top, "@stroke-linecap", "round");
      xml_add(top, "@stroke-linejoin", "round");
      xml_add(top, "@font-family", "sans-serif");
      xml_add(top, "@font-size", "15");
      int x = 0;
      for (d = 0; d < MAX; d++)
         if (d != TEMPO || !data[TEMP].count)
            if (data[d].count)
            {
               xml_t g = xml_element_add(data[d].g, "g");
               xml_add(g, "@opacity", "0.5");
               xml_add(g, "@text-anchor", "end");
               xml_add(g, "@fill", data[d].colour);
               for (double v = data[d].min + ysize / data[d].scale; v < data[d].max; v += ysize / data[d].scale)
               {
                  if (d == RH && v < 0)
                     continue;
                  if (d == RH && v > 100)
                     break;
                  if (d == CO2 && v < 0)
                     continue;
                  xml_t t = xml_addf(g, "+text", "%.0f", v);
                  xml_addf(t, "@transform", "translate(%d,%d)scale(1,-1)", x * 40 + 40, (int) (v * data[d].scale - 5));
                  //xml_add(t, "@alignment-baseline", "middle"); // Does not convert to pdf, hence -5
               }
               xml_t t = xml_add(g, "+text", data[d].unit);
               xml_addf(t, "@transform", "translate(%d,%d)scale(1,-1)", x * 40 + 40, (int) ((data[d].max - 0.1) * data[d].scale - 10));
               //xml_add(t, "@alignment-baseline", "hanging"); // Does not convert to pdf, hence -10
               if (data[d].line)
               {                // Reference line
                  int y = data[d].line * data[d].scale;
                  xml_t l = xml_element_add(g, "path");
                  xml_addf(l, "@d", "M0,%dL%d,%d", y, maxx, y);
                  xml_add(l, "@stroke-dasharray", "1 2");
               }
               x++;
            }
      for (int x = xsize; x < maxx; x += xsize)
      {
         xml_t t = xml_addf(axis, "+text", "%02d", (int) (x / xsize) % 24);
         xml_addf(t, "@x", "%d", x);
         xml_addf(t, "@y", "%d", maxy - 2);
         xml_add(axis, "@text-anchor", "middle");
      }
   }
   if (title)
   {
      char *txt = strdupa(title);
      int y = 0;
      while (txt && *txt)
      {
         char *e = strchr(txt, '/');
         if (e)
            *e++ = 0;
         xml_t t = xml_element_add(top, "text");
         if (*txt == '-')
         {
            y += 9;
            txt++;
            xml_add(t, "@font-size", "7");
         } else
            y += 17;
         xml_element_set_content(t, txt);
         xml_addf(t, "@x", "%d", maxx);
         xml_addf(t, "@y", "%d", y);
         xml_add(t, "@text-anchor", "end");
         txt = e;
      }
      free(txt);
      if (!nodate)
      {
         y += 17;
         xml_t t = xml_element_add(top, "text");
         if (strcmp(sdate, ldate))
            asprintf(&txt, "%s⇒%s", sdate, ldate);
         else
            asprintf(&txt, "%s", date);
         xml_element_set_content(t, txt);
         xml_addf(t, "@x", "%d", maxx);
         xml_addf(t, "@y", "%d", y);
         xml_add(t, "@text-anchor", "end");
         free(txt);
      }
   }
   xml_addf(svg, "@width", "%d", maxx + 1);
   xml_addf(svg, "@height", "%d", maxy + 1);
   // Position and invert
   for (d = 0; d < MAX; d++)
      if (data[d].count)
         xml_addf(data[d].g, "@transform", "translate(0,%.1f)scale(1,-1)", data[d].scale * data[d].max);
   // Write out
   xml_write(stdout, svg);
   xml_tree_delete(svg);
   return 0;
}

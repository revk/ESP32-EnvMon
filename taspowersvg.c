// Power monitoring graphing
// Copyright (c) 2022 Adrian Kennard

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

typedef struct device_s device_t;
struct device_s {
   device_t *next;
   const char *device;
};

#define FNV32_OFFSET_BASIS  2166136261U
#define FNV32_PRIME         16777619

static inline unsigned int FNV32_progressive(unsigned int hash, const unsigned char *data, int count)
{
   // Note the prime - 16777619 - is 2**24 + 2**8 + 2**7 + 2**4 + 2 + 1
   // It is possible that using adds and shifts may give a faster calculation
   // than the multiplication below
   const unsigned char *end = data + count;
   while (data < end)
      hash = (hash ^ *data++) * FNV32_PRIME;
   return hash;
}

static inline unsigned int FNV32_bitfold(unsigned int hash, int bits)
{
   if (bits >= 32)
      return hash;              // Compiler gets 32 wrong!
   return ((hash >> bits) ^ hash) & ((1 << bits) - 1);
}

static inline unsigned int FNV32(const unsigned char *data, int count, int bits)
{
   return FNV32_bitfold(FNV32_progressive(FNV32_OFFSET_BASIS, data, count), bits);
}

void colour(xml_t g, const char *tag, const char *name)
{
   unsigned int q = FNV32((unsigned char *) name, strlen(name), 24);
   if ((q & 0x808080) == 0x808080)
      q ^= 0x808080;
   xml_addf(g, tag, "#%06X", q);
}

int main(int argc, const char *argv[])
{
   const char *sqlhostname = NULL;
   const char *sqldatabase = "tasmota";
   const char *sqlusername = NULL;
   const char *sqlpassword = NULL;
   const char *sqlconffile = NULL;
   const char *sqltable = "power";
   const char *control = NULL;
   const char *href = NULL;
   char *date = NULL;
   const char *title = NULL;
   device_t *devices = NULL;
   int devicecount = 0;
   int debug = 0,
       today = 0,
       yesterday = 0,
       thismonth = 0,
       lastmonth = 0,
       thisyear = 0,
       lastyear = 0,
       trace = 0,
       noaxis = 0,
       nogrid = 0,
       nodate = 0,
       nokey = 0,
       background = 0,
       raw = 0;
   int xsize = 40,
       ysize = 10,
       tsize = 9,
       ystep = 1,
       ydiv = 1000;
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
         { "control", 'C', POPT_ARG_STRING, &control, 0, "date/device{/device}" },
         { "date", 'D', POPT_ARG_STRING, &date, 0, "YYYY[-MM[-DD]]" },
         { "title", 'T', POPT_ARG_STRING, &title, 0, "Title" },
         { "href", 'H', POPT_ARG_STRING, &href, 0, "href" },
         { "today", 0, POPT_ARG_NONE, &today, 0, "Today" },
         { "yesterday", 0, POPT_ARG_NONE, &yesterday, 0, "Yesterday" },
         { "this-month", 0, POPT_ARG_NONE, &thismonth, 0, "This month" },
         { "last-month", 0, POPT_ARG_NONE, &lastmonth, 0, "Last month" },
         { "this-year", 0, POPT_ARG_NONE, &thisyear, 0, "This year" },
         { "last-year", 0, POPT_ARG_NONE, &lastyear, 0, "Last year" },
         { "trace", 0, POPT_ARG_NONE, &trace, 0, "Trace lines" },
         { "background", 0, POPT_ARG_NONE, &background, 0, "First device is a total/background" },
         { "no-axis", 0, POPT_ARG_NONE, &noaxis, 0, "No axis" },
         { "no-grid", 0, POPT_ARG_NONE, &nogrid, 0, "No grid" },
         { "no-date", 0, POPT_ARG_NONE, &nodate, 0, "No date" },
         { "no-key", 0, POPT_ARG_NONE, &nokey, 0, "No key" },
         { "no-factor", 0, POPT_ARG_NONE, &raw, 0, "Don't apply power factor" },
         { "x-size", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &xsize, 0, "Units per period" },
         { "y-size", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &ysize, 0, "Units per kWh" },
         { "text-size", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &tsize, 0, "Text size" },
         { "debug", 'V', POPT_ARG_NONE, &debug, 0, "Debug" },
         POPT_AUTOHELP { }
      };

      optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);
      poptSetOtherOptionHelp(optCon, "devices");

      int c;
      if ((c = poptGetNextOpt(optCon)) < -1)
         errx(1, "%s: %s\n", poptBadOption(optCon, POPT_BADOPTION_NOALIAS), poptStrerror(c));

      device_t **last = &devices;
      {                         // Device list
         const char *v;
         while ((v = poptGetArg(optCon)))
         {
            device_t *d = malloc(sizeof(*d));
            if (!d)
               errx(1, "malloc");
            d->device = v;
            d->next = NULL;
            *last = d;
            last = &d->next;
            devicecount++;
         }
      }
      if (control)
      {                         // control sets date and device list as a single string
         char *c = strdup(control);
         if (*c == '/')
            c++;                // Path info can be used directly
         char *s = strchr(c, '/');
         if (!date)
         {
            if (s)
               *s++ = 0;
            date = c;
            c = s;
            if (!strcasecmp(date, "LY"))
               lastyear = 1;
            else if (!strcasecmp(date, "Y"))
               thisyear = 1;
            else if (!strcasecmp(date, "LM"))
               lastmonth = 1;
            else if (!strcasecmp(date, "M"))
               thismonth = 1;
            else if (!strcasecmp(date, "P"))
               yesterday = 1;
            else if (!strcasecmp(date, "D"))
               today = 1;
         }
         if (c && *c == '-')
         {
            background = 1;
            c++;
         }
         // Devices
         while (c && *c)
         {
            s = strchr(c, '/');
            if (s)
               *s++ = 0;
            device_t *d = malloc(sizeof(*d));
            if (!d)
               errx(1, "malloc");
            d->device = c;
            d->next = NULL;
            *last = d;
            last = &d->next;
            devicecount++;
            c = s;
         }
      }
      struct tm t;
      time_t now = time(0);
      localtime_r(&now, &t);
      if (today || yesterday)
      {
         if (yesterday)
            t.tm_mday--;
         timelocal(&t);
         if (asprintf(&date, "%04d-%02d-%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday) < 0)
            errx(1, "malloc");
      } else if (thismonth || lastmonth)
      {
         if (lastmonth)
         {
            t.tm_mon--;
            t.tm_mday = 1;
         }
         timelocal(&t);
         if (asprintf(&date, "%04d-%02d", t.tm_year + 1900, t.tm_mon + 1) < 0)
            errx(1, "malloc");
      } else if (thisyear || lastyear)
      {
         if (lastyear)
         {
            t.tm_year--;
            t.tm_mday = 1;
         }
         timelocal(&t);
         if (asprintf(&date, "%04d", t.tm_year + 1900) < 0)
            errx(1, "malloc");
      }
      if (!date || !devices)
      {
         poptPrintUsage(optCon, stderr, 0);
         return -1;
      }
      poptFreeContext(optCon);
   }

   SQL sql;
   sql_real_connect(&sql, sqlhostname, sqlusername, sqlpassword, sqldatabase, 0, NULL, 0, 1, sqlconffile);

   xml_t svg = xml_tree_new("svg");
   xml_element_set_namespace(svg, xml_namespace(svg, NULL, "http://www.w3.org/2000/svg"));
   if (href)
      xml_add(svg, "style", ":visited{fill:none;}");
   if (background)
   {
      int u = xsize / 8 * 2;
      xml_t defs = xml_element_add(svg, "defs");
      xml_t p = xml_element_add(defs, "pattern");
      xml_add(p, "@id", "background");
      xml_addf(p, "@width", "%d", u);
      xml_addf(p, "@height", "%d", u);
      xml_add(p, "@patternUnits", "userSpaceOnUse");
      xml_t l = xml_element_add(p, "path");
      xml_addf(l, "@d", "M0,0l%d,0l0,%dl%d,0Z", u, u, -u);
      xml_addf(l, "@fill", "white");
      xml_addf(l, "@stroke", "white");
      l = xml_element_add(p, "path");
      xml_addf(l, "@d", "M-2,-1l%d,%dM%d,-1l%d,%d", u + 4, u + 4, u - 2, u + 4, u + 4);
      xml_addf(l, "@fill", "none");
      colour(l, "@stroke", devices->device);
   }

   int Y = 0,
       M = 0,
       D = 0;
   if (sscanf(date, "%d-%d-%d", &Y, &M, &D) < 1 || Y < 2000 || (M && (M < 1 || M > 12)) || (D && (D < 1 || D > 31)))
      errx(1, "Date is YYYY[-MM[-DD]]");
   if (control && D && devices && !devices->next)
      trace = 1;
   if (control && D)
      ysize *= 5;
   if (control && !M)
      ydiv *= 20;
   while (ysize * ystep * 1000 < 3 * ydiv)
      ystep = 10;
   int periods = 0;
   const char *factor = (raw ? "" : "/if(`factor`<0.5,1,`factor`)");
   char *q = NULL;
   if (!M)
   {                            // Year
      periods = 12;
      q = sql_printf("SELECT *,substring(`ts`,6,2) AS `P`,sum(`wh`%s) AS `T` FROM `%#S` WHERE `ts` LIKE '%04d-%%' GROUP BY `device`,`P` ORDER BY `device`,`P`", factor, sqltable, Y);
   } else if (!D)
   {                            // Month
      struct tm t = { };
      t.tm_year = Y - 1900;
      t.tm_mon = M - 1 + 1;
      t.tm_mday = 1 - 1;
      t.tm_isdst = -1;
      timelocal(&t);
      periods = t.tm_mday;
      q = sql_printf("SELECT *,substring(`ts`,9,2) AS `P`,sum(`wh`%s) AS `T` FROM `%#S` WHERE `ts` LIKE '%04d-%02d-%%' GROUP BY `device`,`P` ORDER BY `device`,`P`", factor, sqltable, Y, M);
   } else if (!trace)
   {                            // Day
      periods = 24;             // DST causes gap or double on change...
      q = sql_printf("SELECT *,substring(`ts`,12,2) AS `P`,sum(`wh`%s) AS `T` FROM `%#S` WHERE `ts` LIKE '%04d-%02d-%02d %%' GROUP BY `device`,`P` ORDER BY `device`,`P`", factor, sqltable, Y, M, D);
   } else
   {
      periods = 24;             // DST causes gap or double on change...
      q = sql_printf("SELECT *,substring(`ts`,12,8) AS `P`,sum(if(`w` is null,`power`,`w`)) AS `T`,sum(if(`w` is null,`power`,`w`)%s) AS `TF` FROM `%#S` WHERE `ts` LIKE '%04d-%02d-%02d %%' GROUP BY `device`,`P` ORDER BY `device`,`P`", factor, sqltable, Y, M, D);
   }
   SQL_RES *res = sql_safe_query_store_free(&sql, q);
   // Plot
   xml_t g = xml_element_add(svg, "g");
   xml_t grid = xml_element_add(svg, "g");
   xml_t axis = xml_element_add(svg, "g");
   xml_t text = xml_element_add(svg, "g");

   double hwm[periods],
    max = 0;
   for (int x = 0; x < periods; x++)
      hwm[x] = 0;
   int left = tsize * 3;
   int right = left + periods * xsize + 1;

   // Work through device list...
   char *dlist = NULL;
   size_t dlen = 0;
   FILE *dfile = open_memstream(&dlist, &dlen);
   device_t *d = devices;
   while (d)
   {
      if (background && d == devices)
         fprintf(dfile, "/-%s", d->device);
      else
         fprintf(dfile, "/%s", d->device);
      sql_data_seek(res, 0);
      xml_t t = NULL;
      if (!trace)
      {
         t = xml_element_add(g, "g");
         if (background && d == devices)
            xml_add(t, "@style", "fill:url(#background);");
         else
            colour(t, "@fill", d->device);
         colour(t, "@stroke", d->device);
         xml_add(t, "@stroke-width", "0.1");
      }
      char *path = NULL,
          *path2 = NULL;
      size_t len,
       len2;
      FILE *o = NULL,
          *o2 = NULL;
      char tag = 'M';
      if (trace)
      {
         o = open_memstream(&path, &len);
         o2 = open_memstream(&path2, &len2);
      }
      while (sql_fetch_row(res))
      {
         if (strcasecmp(d->device, sql_colz(res, "device")))
            continue;
         double T = strtod(sql_colz(res, "T"), NULL);
         if (trace)
         {
            double TF = strtod(sql_colz(res, "TF"), NULL);
            const char *PS = sql_colz(res, "P");
            int P = atoi(PS);
            if (Y && !D)
               P--;
            double x = P * xsize + xsize / 2;
            if (D && strlen(PS) == 8)
               x = (double) (atoi(PS) * 3600 + atoi(PS + 3) * 60 + atoi(PS + 6)) * xsize / 3600;
            fprintf(o, "%c%f,%f", tag, x, T * ysize / ydiv);
            fprintf(o2, "%c%f,%f", tag, x, TF * ysize / ydiv);
            tag = 'L';
            if (T > max)
               max = T;
            if (TF > max)
               max = TF;
         } else
         {
            int P = atoi(sql_colz(res, "P"));
            if (Y && !D)
               P--;
            if (P < 0 || P >= periods)
               continue;
            xml_t path = t;
            if (href)
            {
               path = xml_element_add(path, "a");
               if (!D)
                  xml_addf(path, "@href", "%s/%s-%s/%s", href, date, sql_colz(res, "P"), d->device);
               else
                  xml_addf(path, "@href", "%s/%s/%s", href, date, d->device);
            }
            path = xml_element_add(path, "path");
            xml_addf(path, "title", "%s %.2fkW%s", d->device, T / 1000, D ? "" : "h");
            double new = hwm[P] + T;
            xml_addf(path, "@d", "M%d,%fL%d,%fL%d,%fL%d,%fZ",   //
                     P * xsize, hwm[P] * ysize / ydiv,  //
                     (P + 1) * xsize, hwm[P] * ysize / ydiv,    //
                     (P + 1) * xsize, new * ysize / ydiv,       //
                     P * xsize, new * ysize / ydiv);
            if (new > max)
               max = new;
            if (!background || d != devices)
               hwm[P] += T;
         }
      }
      if (trace)
      {
         fclose(o);
         fclose(o2);
         t = xml_element_add(g, "path");
         xml_add(t, "@fill", "none");
         colour(t, "@stroke", d->device);
         if (!raw)
            xml_add(t, "@stroke-dasharray", "1 2");
         xml_add(t, "@d", path);
         if (!raw)
         {
            t = xml_element_add(g, "path");
            xml_add(t, "@fill", "none");
            colour(t, "@stroke", d->device);
            xml_add(t, "@d", path2);
         }
         free(path);
         free(path2);
      }
      d = d->next;
   }
   fclose(dfile);
   sql_free_result(res);
   int kwh = ceil(max / 1000);
   int bottom = tsize + 2;
   int top = bottom + kwh * ysize * 1000 / ydiv + 1;
   // Axis
   xml_add(g, "@stroke-linecap", "round");
   xml_add(g, "@stroke-linejoin", "round");
   xml_add(text, "@font-family", "sans-serif");
   xml_addf(text, "@font-size", "%d", tsize);
   xml_add(axis, "@font-family", "sans-serif");
   xml_addf(axis, "@font-size", "%d", tsize);
   if (noaxis)
      xml_element_delete(axis);
   else
   {
      xml_add(axis, "@opacity", "0.5");
      xml_t g = xml_element_add(axis, "g");
      for (int x = 0; x < periods; x++)
      {
         if (xsize < 2 * tsize && (x % (2 * tsize / xsize)))
            continue;
         xml_t t = g;
         if (href && !D)
         {
            t = xml_element_add(t, "a");
            if (Y && !D)
               xml_addf(t, "@href", "%s/%s-%02d%s", href, date, x + 1, dlist);
            else
               xml_addf(t, "@href", "%s/%s-%02d%s", href, date, x, dlist);
         }
         if (Y && !D)
            t = xml_addf(t, "+text", "%2d", x + 1);
         else
            t = xml_addf(t, "+text", "%02d", x);
         xml_addf(t, "@x", "%d", left + x * xsize + (trace ? 0 : xsize / 2));
         xml_addf(t, "@y", "%d", top - 1);
         xml_add(g, "@text-anchor", "middle");
      }
      g = xml_element_add(axis, "g");
      int l = 0;
      for (int y = 0; y < kwh; y += ystep)
      {
         int q = y * ysize * 1000 / ydiv;
         if (y && q - l <= tsize)
            continue;           // Too close
         l = q;
         xml_t t = xml_addf(g, "+text", "%d", y);
         xml_addf(t, "@x", "%d", left - 2);
         xml_addf(t, "@y", "%d", top - bottom - q + tsize / 2 - 1);
         xml_add(g, "@text-anchor", "end");
      }
      xml_t t = xml_add(axis, "+text", D ? "kW" : "kWh");
      xml_addf(t, "@transform", "translate(%d,%d)rotate(-90)", tsize + 1, top - 1);
   }
   if (nogrid)
      xml_element_delete(grid);
   else
   {
      xml_add(grid, "@opacity", "0.1");
      xml_add(grid, "@stroke", "black");
      xml_add(grid, "@fill", "none");
      xml_add(grid, "@pointer-events", "none");
      for (int x = 0; x <= periods; x++)
      {
         xml_t l = xml_element_add(grid, "path");
         xml_addf(l, "@d", "M%d,%dL%d,%d", left + x * xsize, top - bottom, left + x * xsize, top - bottom - kwh * ysize * 1000 / ydiv);
      }
      for (int y = 0; y <= kwh; y += ystep)
      {
         xml_t l = xml_element_add(grid, "path");
         xml_addf(l, "@d", "M%d,%dL%d,%d", left, top - bottom - y * ysize * 1000 / ydiv, left + periods * xsize, top - bottom - y * ysize * 1000 / ydiv);
      }
   }
   int y = 0;
   if (title)
   {
      char *txt = strdupa(title);
      while (txt && *txt)
      {
         char *e = strchr(txt, '/');
         if (e)
            *e++ = 0;
         xml_t t = xml_element_add(text, "text");
         if (*txt == '-')
         {
            y += tsize / 2 + 1;
            txt++;
            xml_addf(t, "@font-size", "%d", tsize / 2);
         } else
            y += tsize + 1;
         xml_element_set_content(t, txt);
         xml_addf(t, "@x", "%d", right - 2);
         xml_addf(t, "@y", "%d", y);
         xml_add(t, "@text-anchor", "end");
         txt = e;
      }
      free(txt);
   }
   if (!nodate)
   {
      y += tsize + 1;
      xml_t t = xml_element_add(text, "text");
      xml_element_set_content(t, date);
      xml_addf(t, "@x", "%d", right - 2);
      xml_addf(t, "@y", "%d", y);
      xml_add(t, "@text-anchor", "end");
   }
   if (!nokey)
   {
      void label(device_t * d) {
         xml_t t = text;
         if (href)
         {
            t = xml_element_add(t, "a");
            xml_addf(t, "@href", "%s/%s/%s", href, date, d->device);
         }
         t = xml_element_add(t, "text");
         xml_element_set_content(t, d->device);
         colour(t, "@fill", d->device);
         xml_addf(t, "@x", "%d", right - 2);
         xml_addf(t, "@y", "%d", y);
         xml_add(t, "@text-anchor", "end");
      }
      d = devices;
      if (background)
      {
         y += tsize + 1;
         label(d);
         d = d->next;
         y -= tsize + 1;
      }
      y += (tsize + 1) * devicecount;
      while (d)
      {
         label(d);
         d = d->next;
         y -= tsize + 1;
      }
   }
   xml_addf(svg, "@width", "%d", right);
   xml_addf(svg, "@height", "%d", top);
   xml_addf(g, "@transform", "translate(%d,%d)scale(1,-1)", left, top - bottom);
   // Write out
   xml_write(stdout, svg);
   xml_tree_delete(svg);
   free(dlist);
   return 0;
}

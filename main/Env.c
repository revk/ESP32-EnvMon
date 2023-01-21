/* CO2 + other sensors all */
/* Copyright(c) 2019-21 Adrian Kennard, Andrews & Arnold Limited, see LICENSE file(GPL) */
const char TAG[] = "Env";

/* #define       DEBUGTEMP */

#include "revk.h"
#include <driver/i2c.h>
#include <hal/spi_types.h>
#include <math.h>
#include <sntp.h>

#include "ds18b20.h"
#include "gfx.h"

#include "icons.h"
/*
 * Setting for "logo" is 32 x32 bytes(4 bits per pixel) Note that MQTT config needs to allow a large enough message for the logo
 */
#define LOGOW   32
#define LOGOH   32

#define ACK_CHECK_EN 0x1        /* !< I2C master will check ack from slave */
#define ACK_CHECK_DIS 0x0       /* !< I2C master will not check ack from slave */
#define ACK_VAL 0x0             /* !< I2C ack value */
#define NACK_VAL 0x1            /* !< I2C nack value */
#define	MAX_OWB	8
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)

/* temp targets of 0 mean off, so use 1 or - 1 if really targeting 0 */

#define settings	\
	u32(reporting,60)	\
	u8(lag,5)	\
	io(sda,22)	\
	io(scl,21)	\
	s8(co2address,0x62)	\
	s8(alsaddress,0x10)	\
	u16(alsdark,200)	\
	s8(alsplaces,-2)	\
	s8(co2places,-1)	\
	u32(co2damp,100)	\
	u32(startup,300)	\
	s8(tempplaces,1)	\
	s8(rhplaces,0)	\
	u32(rhdamp,10)	\
	io(ds18b20,14)	\
	s32(ds18b20mC,0)	\
	io(gfxmosi,32)	\
	io(gfxsck,33)	\
	io(gfxcs,25)	\
	io(gfxdc,26)	\
	io(gfxrst,27)	\
	u8(gfxflip,3)	\
	u8(gfxlight,255)\
	u8(gfxdark,1)	\
	u32(msgtime,30)	\
	b(f)	\
	b(ha)	\
	s(fanon)	\
	s(fanoff)	\
	u8(fanswitch,30)	\
	u32(fanco2on,0)	\
	u32(fanco2off,0)	\
	u32(fanrhon,0)	\
	u32(fanrhoff,0)	\
	io(fanco2gpio,)	\
	u32(fanresend,600)	\
	s(heaton)	\
	s(heatoff)	\
	s(heataircon)	\
	b(heatmonitor)	\
	u8(heatswitch,30)	\
	u32(heatresend,600)	\
	io(heatgpio,)	\
	u16(hhmmnight,0)	\
	u16(hhmmday,0)		\
	b(nologo)	\
	b(notime)	\
	s32(heatdaymC,0)	\
	s32(heatnightmC,0)	\
	u16a(temphhmm,10)	\
	s32a(tempmaxmC,10)	\
	s32a(tempminmC,10)	\
	ioa(button,3,4 13 15)	\

#define u32(n,d)	uint32_t n;
#define u16(n,d)	uint16_t n;
#define u16a(n,q)	uint16_t n[q];
#define s32(n,d)	int32_t n;
#define s32a(n,q)	int32_t n[q];
#define s8(n,d)	int8_t n;
#define u8(n,d)	uint8_t n;
#define b(n) uint8_t n;
#define s(n) char * n;
#define io(n,d)         uint8_t n;
#define ioa(n,a,d)      uint8_t n[a];
#define	IO_MASK	0x3F
#define	IO_INV	0x40
settings
#undef u32
#undef u16
#undef u16a
#undef s32
#undef s32a
#undef s8
#undef u8
#undef b
#undef s
#undef io
#undef ioa
static uint8_t scd41 = 0;
static uint32_t scd41_settled = 1;      /* uptime when started measurements */

static uint8_t airconpower = 0;
static char airconmode = 0;
static uint32_t airconlast = 0;
static char display_icon = 0;

static uint8_t logo[LOGOW * LOGOH / 2];
static char lasticon = 0;
static float lastco2 = NAN;
static float lastals = NAN;
static float lastrh = NAN;
static float lasttemp = NAN;
static float lastotemp = NAN;
static float thisco2 = NAN;
static float thistemp = NAN;
static float thisrh = NAN;
static float temptargetmin = NAN;
static float temptargetmax = NAN;
static int8_t temptimeprev = -1;
static int8_t temptimenext = -1;
static float tempoverridemin = NAN;
static float tempoverridemax = NAN;
static uint16_t scd_tempoffset = 0;
static uint16_t scd_altitude = 0;
static uint16_t scd_selfcal = 0;
static unsigned long long scd_serial = 0;
static int8_t i2cport = -1;
static volatile uint32_t do_co2 = 0;
static int8_t num_ds18b20 = 0;
static DeviceAddress adr_ds18b20[2];
static char co2_found = 0;
static char als_found = 0;
static char sendinfo = 0;

static uint32_t fantime = 0;
static uint32_t heattime = 0;
static int8_t fanlast = -1,
    fanmax = -1;
static int8_t heatlast = -1,
    heatmax = -1;

static volatile uint8_t gfx_update = 0;
static volatile uint8_t gfx_changed = 1;
static volatile uint8_t gfx_dark = 0;
static volatile uint32_t gfx_msg_time = 0;      /* message timer */
static volatile uint32_t menu_time = 0; /* menu timer */
static char gfx_msg[100];       /* message text */

static const char *co2_setting(uint16_t cmd, uint16_t val);

typedef struct value_s value_t;
struct value_s {
   value_t *next;
   const char *tag;
   int8_t places;
   float value;
};
value_t *values = NULL;
time_t reportlast = 0,
    reportchange = 0,
    reportconfig = 0;

static void reportall(time_t now)
{                               /* Do reporting of values */
   if ((!reportchange || now < reportchange + lag) && (!reporting || now / reporting == reportlast / reporting))
      return;                   /* Slight delay on changes */
   if (values)
   {
      value_t *v;
      jo_t j = jo_object_alloc();
      void add(const char *tag, float value, int8_t places) {
         if (places <= 0)
            jo_litf(j, tag, "%d", (int) value);
         else
            jo_litf(j, tag, "%.*f", places, value);
      }
      if (now < 1000000000)
         jo_litf(j, "ts", "%ld", now);
      else
      {
         struct tm tm;
         gmtime_r(&now, &tm);
         jo_stringf(j, "ts", "%04d-%02d-%02dT%02d:%02d:%02dZ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
      }
      for (v = values; v; v = v->next)
         if (!isnan(v->value))
            add(v->tag, v->value, v->places);
      if (heatmax >= 0)
         jo_bool(j, "heat", heatmax);
      if (fanmax >= 0)
         jo_bool(j, "fan", fanmax);
      jo_bool(j, "dark", gfx_dark);
      fanmax = fanlast;
      heatmax = heatlast;
      if (!isnan(temptargetmin) && !isnan(temptargetmax))
      {
         if (temptargetmin == temptargetmax)
            jo_litf(j, "temp-target", "%.3f", temptargetmin);
         else
         {
            jo_array(j, "temp-target");
            jo_litf(j, NULL, "%.3f", temptargetmin);
            jo_litf(j, NULL, "%.3f", temptargetmax);
            jo_close(j);
         }
      }
      if (display_icon)
         jo_litf(j, "icon", "\"%c\"", display_icon);
      revk_state("data", &j);
      if (*heataircon && !isnan(lasttemp))
      {                         /* Aircon control */
         static float last = NAN;
         if (isnan(last) || last != lasttemp || !reporting || now / reporting != reportlast / reporting)
         {
            char topic[100];
            snprintf(topic, sizeof(topic), "command/%s/control", heataircon);
            jo_t j = jo_object_alloc();
            if (tempplaces <= 0)
               jo_litf(j, "env", "%d", (int) lasttemp);
            else
               jo_litf(j, "env", "%.*f", tempplaces, lasttemp);
            if (!heatmonitor)
            {
               if (!isnan(temptargetmin) && temptargetmin == temptargetmax)
                  jo_litf(j, "target", "%.3f", temptargetmin);
               else
               {
                  jo_array(j, "target");
                  jo_litf(j, NULL, "%.3f", temptargetmin);
                  jo_litf(j, NULL, "%.3f", temptargetmax);
                  jo_close(j);
               }
            }
            revk_mqtt_send_clients(NULL, 0, topic, &j, 1);
            last = lasttemp;
         }
      }
      reportlast = now;
      reportchange = 0;
   }
}

static float report(const char *tag, float last, float this, int places)
{
   value_t *v;
   for (v = values; v && v->tag != tag; v = v->next);
   if (!v)
   {
      v = malloc(sizeof(*v));
      v->tag = tag;
      v->places = places;
      v->next = values;
      values = v;
   }
   float mag = powf(10.0, -places);
   if (!isnan(last))
   {                            /* Hysteresis */
      if (this < last)
      {
         this += mag * 0.4;     /* Hysteresis, and it would have to go a further 0.5 to flip on the roundf() */
         if (this >= last)
            return last;
      } else if (this > last)
      {
         this -= mag * 0.4;     /* Hysteresis, and it would have to go a further 0.5 to flip on the roundf() */
         if (this <= last)
            return last;
      }
   }
   if (!isnan(this))
      this = roundf(this / mag) * mag;  /* Rounding */
   if (!isnan(last) && this != last && !reportchange)
      reportchange = time(0);
   v->value = this;
   return this;
}

static void sendall(void)
{
   reportlast = 0;
   reportconfig = 0;
   heattime = 0;
   fantime = 0;
}

static void sendconfig(void)
{
   if (!ha)
      return;
   reportconfig = time(0);
   char *topic;
   const char *us = hostname;
   if (!*us)
      us = revk_id;
   void add(const char *tag, const char *type, const char *unit, const char *json) {
      if (asprintf(&topic, "homeassistant/sensor/%s-%c/config", us, *tag) >= 0)
      {
         jo_t j = jo_object_alloc();
         jo_stringf(j, "unique_id", "%s-%c", us, *tag);
         jo_object(j, "dev");
         jo_array(j, "ids");
         jo_string(j, NULL, revk_id);
         jo_close(j);
         jo_string(j, "name", us);
         jo_string(j, "mdl", appname);
         jo_string(j, "sw", revk_version);
         jo_string(j, "mf", "www.me.uk");
         jo_close(j);
         jo_string(j, "dev_cla", type);
         jo_stringf(j, "name", "%s %s", us, tag);
         jo_stringf(j, "stat_t", "state/%s/%s/data", appname, us);
         jo_string(j, "unit_of_meas", unit);
         jo_stringf(j, "val_tpl", "{{value_json.%s}}", json);
         revk_mqtt_send(NULL, 1, topic, &j);
      }
   }
   if (ds18b20 || i2cport >= 0)
      add("Temp", "temperature", "°C", "temp");
   if (i2cport >= 0)
      add("R/H", "humidity", "%", "rh");
   if (i2cport >= 0)
      add("CO₂", "co2", "ppm", "co2");
}

const char *app_callback(int client, const char *prefix, const char *target, const char *suffix, jo_t j)
{

   if (*heataircon && prefix && !strcmp(prefix, "Daikin") && target && !strcmp(target, heataircon) && airconlast)
      airconlast = uptime();
   if (*heataircon && prefix && !strcmp(prefix, "state") && target && !strcmp(target, heataircon) && jo_here(j) == JO_OBJECT)
   {                            // Aircon state
      airconlast = uptime();
      jo_type_t t = jo_next(j); // Start object
      while (t == JO_TAG)
      {
         char tag[10] = "",
             val[10];
         jo_strncpy(j, tag, sizeof(tag));
         t = jo_next(j);
         if (!strcmp(tag, "mode"))
         {
            jo_strncpy(j, val, sizeof(val));
            airconmode = *val;
         } else if (!strcmp(tag, "power"))
            airconpower = (t == JO_TRUE);
         t = jo_skip(j);
      }
      return "";
   }
   if (client || !prefix || target || strcmp(prefix, "command") || !suffix)
      return NULL;
   if (!strcmp(suffix, "override"))
   {                            // Expects array of min/max
      if (jo_here(j) == JO_ARRAY)
      {
         scd41_settled = 0;     // Cancel startup
         if (jo_next(j) == JO_NUMBER)
            tempoverridemin = jo_read_float(j);
         else
            tempoverridemin = NAN;
         if (jo_next(j) == JO_NUMBER)
            tempoverridemax = jo_read_float(j);
         else
            tempoverridemax = tempoverridemin;
      } else
         tempoverridemin = tempoverridemax = NAN;       // Cancelled
      return "";
   }
   if (!strcmp(suffix, "send"))
   {
      sendall();
      return "";
   }
   if (!strcmp(suffix, "connect"))
   {
      sendinfo = 1;
      fanlast = -1;
      heatlast = -1;
      sendall();
      if (*heataircon)
      {
         char temp[100];
         snprintf(temp, sizeof(temp), "state/%s/status", heataircon);
         lwmqtt_subscribe(revk_mqtt(0), temp);
         snprintf(temp, sizeof(temp), "Daikin/%s", heataircon);
         lwmqtt_subscribe(revk_mqtt(0), temp);
      }
      return "";
   }
   if (!strcmp(suffix, "message"))
   {
      jo_strncpy(j, gfx_msg, sizeof(gfx_msg));
      if (msgtime)
         gfx_msg_time = uptime() + msgtime;
      return "";
   }
   if (!strcmp(suffix, "night"))
   {                            /* Night mode, or can be sent with true / false to set night or day */
      if (jo_here(j) == JO_FALSE)
         gfx_dark = 0;
      else
         gfx_dark = 1;
      return "";
   }
   if (!strcmp(suffix, "day"))
   {
      gfx_dark = 0;
      return "";
   }
   if (!strcmp(suffix, "contrast"))
   {
      gfx_set_contrast(jo_read_int(j));
      return "";                /* OK */
   }
   if (!strcmp(suffix, "co2factory") && scd41)
      return co2_setting(0x3632, -1);
   if (!strcmp(suffix, "co2reload") && scd41)
      return co2_setting(0x3646, -1);
   if (!strcmp(suffix, "co2autocal"))
      return co2_setting(scd41 ? 0x2416 : 0x5306, 1);
   if (!strcmp(suffix, "co2nocal"))
      return co2_setting(scd41 ? 0x2416 : 0x5306, 0);
   if (!strcmp(suffix, "co2cal"))
      return co2_setting(scd41 ? 0x362f : 0x5204, jo_read_int(j));      /* ppm */
   if (!strcmp(suffix, "co2tempoffset"))
      return co2_setting(scd41 ? 0x241d : 0x5403, jo_read_int(j));      /* Use T * 65536 / 175 */
   if (!strcmp(suffix, "co2tempcal"))
   {                            // Set the current temp in C
      if (isnan(lasttemp))
         return "No temp now";
      return co2_setting(scd41 ? 0x241d : 0x5403, scd_tempoffset - (jo_read_float(j) - lasttemp) * 65536.0 / 175.0);    // Oddly the offset seems to be negative
   }
   if (!strcmp(suffix, "co2alt"))
      return co2_setting(scd41 ? 0x2427 : 0x5102, jo_read_int(j));      /* m */
   return NULL;
}

static uint8_t co2_crc(uint8_t b1, uint8_t b2)
{
   uint8_t crc = 0xFF;
   void b(uint8_t v) {
      crc ^= v;
      uint8_t n = 8;
      while (n--)
      {
         if (crc & 0x80)
            crc = (crc << 1) ^ 0x31;
         else
            crc <<= 1;
      }
   }
   b(b1);
   b(b2);
   return crc;
}

static i2c_cmd_handle_t co2_setup(uint16_t c)
{                               /* Set up command */
   /* ESP_LOGE(TAG, "CO2 cmd %04X", c); */
   i2c_cmd_handle_t i = i2c_cmd_link_create();
   i2c_master_start(i);
   i2c_master_write_byte(i, (co2address << 1), ACK_CHECK_EN);
   i2c_master_write_byte(i, c >> 8, ACK_CHECK_EN);
   i2c_master_write_byte(i, c, ACK_CHECK_EN);
   return i;
}

static void co2_add(i2c_cmd_handle_t i, uint16_t v)
{                               /* Add word to command */
   /* ESP_LOGE(TAG, "CO2 val %04X", v); */
   i2c_master_write_byte(i, v >> 8, ACK_CHECK_EN);
   i2c_master_write_byte(i, v, ACK_CHECK_EN);
   i2c_master_write_byte(i, co2_crc(v >> 8, v), ACK_CHECK_EN);
}

static esp_err_t co2_done(i2c_cmd_handle_t * i)
{                               /* Finish command */
   i2c_master_stop(*i);
   esp_err_t err = i2c_master_cmd_begin(i2cport, *i, 1000 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete(*i);
   *i = NULL;
   return err;
}

static esp_err_t co2_command(uint16_t c)
{
   i2c_cmd_handle_t i = co2_setup(c);
   return co2_done(&i);
}

static esp_err_t co2_read(int len, uint8_t * buf)
{
   i2c_cmd_handle_t i = i2c_cmd_link_create();
   i2c_master_start(i);
   i2c_master_write_byte(i, (co2address << 1) + 1, ACK_CHECK_EN);
   i2c_master_read(i, buf, len - 1, ACK_VAL);
   i2c_master_read_byte(i, buf + len - 1, NACK_VAL);
   return co2_done(&i);
}

static esp_err_t co2_scd41_stop_measure(void)
{
   esp_err_t err = co2_command(0x3f86); /* Stop measurement(SCD41) */
   sleep(1);
   return err;
}

static esp_err_t co2_scd41_start_measure(void)
{
   scd41_settled = uptime() + startup;  /* Time for temp to settle */
   return co2_command(0x21b1);  /* Start measurement(SCD41) */
}

static const char *co2_setting(uint16_t cmd, uint16_t val)
{
   do_co2 = (cmd << 16) + val;
   return "";
}

static uint16_t als_read(uint8_t cmd)
{
   uint8_t h = 0,
       l = 0;
   i2c_cmd_handle_t t = i2c_cmd_link_create();
   i2c_master_start(t);
   i2c_master_write_byte(t, (alsaddress << 1) | I2C_MASTER_WRITE, true);
   i2c_master_write_byte(t, cmd, true);
   i2c_master_start(t);
   i2c_master_write_byte(t, (alsaddress << 1) | I2C_MASTER_READ, true);
   i2c_master_read_byte(t, &l, I2C_MASTER_ACK);
   i2c_master_read_byte(t, &h, I2C_MASTER_LAST_NACK);
   i2c_master_stop(t);
   i2c_master_cmd_begin(i2cport, t, 10 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete(t);
   return (h << 8) + l;
}

static void als_write(uint8_t cmd, uint16_t val)
{
   i2c_cmd_handle_t t = i2c_cmd_link_create();
   i2c_master_start(t);
   i2c_master_write_byte(t, (alsaddress << 1) | I2C_MASTER_WRITE, true);
   i2c_master_write_byte(t, cmd, true);
   i2c_master_write_byte(t, val & 0xFF, true);
   i2c_master_write_byte(t, val >> 8, true);
   i2c_master_stop(t);
   i2c_master_cmd_begin(i2cport, t, 10 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete(t);
}

void i2c_task(void *p)
{
   p = p;
   ESP_LOGI(TAG, "I2C start");
   int try = 10;
   esp_err_t err = 0;
   if (co2address)
   {                            // CO2
      while (try--)
      {
         sleep(1);
         if (scd41)
         {
            err = co2_scd41_stop_measure();
            if (!err)
               err = co2_command(0x3646);       /* Reinit */
            if (!err)
               break;
         } else
         {                      /* Just try starting measurement */
            err = co2_command(0x0010);  /* Start measurement */
            if (!err)
               break;
         }
         ESP_LOGI(TAG, "CO2 retry");
      }
      if (err)
      {                         /* failed */
         ESP_LOGE(TAG, "No CO2");
         jo_t j = jo_object_alloc();
         jo_string(j, "error", "No CO2 sensor");
         jo_string(j, "device", scd41 ? "SCD41" : "SCD30");
         jo_int(j, "sda", sda & IO_MASK);
         jo_int(j, "scl", scl & IO_MASK);
         jo_int(j, "code", err);
         jo_string(j, "description", esp_err_to_name(err));
         revk_error("CO2", &j);
      } else
      {
         co2_found = 1;
         if (scd41)
            co2_setting(0x3682, 0);     // Get serial, etc
      }
   }
   if (alsaddress)
   {
      uint16_t id = als_read(0x09);
      if ((id & 0xFF) != 0x35)
      {
         gfx_dark = 0;
         ESP_LOGE(TAG, "No ALS");
         jo_t j = jo_object_alloc();
         jo_string(j, "error", "No ALS");
         jo_int(j, "sda", sda & IO_MASK);
         jo_int(j, "scl", scl & IO_MASK);
         jo_int(j, "id", id);
         jo_string(j, "description", esp_err_to_name(err));
         revk_error("ALS", &j);
      } else
      {
         als_found = 1;
         als_write(0x00, 0x0040);
      }
   }
   if (!co2_found && !als_found)
   {                            // No I2C to do
      vTaskDelete(NULL);
      return;
   }
   /* Get measurements */
   while (1)
   {
      sleep(1);
      if (co2_found)
      {
         if (do_co2)
         {                      /* Do a command from mqtt */
            uint16_t cmd = (do_co2 >> 16);
            uint16_t val = do_co2;
            do_co2 = 0;
            if (scd41)
               co2_scd41_stop_measure();
            i2c_cmd_handle_t i = co2_setup(cmd);
            if (cmd != 0x3632 && cmd != 0x3615 && cmd != 0x3646 && cmd != 0x3682)
               co2_add(i, val);
            err = co2_done(&i);
            if (err)
               ESP_LOGI(TAG, "CMD failed %s", esp_err_to_name(err));
            if (cmd == 0x362f || cmd == 0x3615 || cmd == 0x3632)
               sleep(1);
            else if (cmd == 0x3639)
               sleep(10);
            if ((cmd == 0x241d || cmd == 0x2416 || cmd == 0x2427) && !err)
            {
               err = co2_command(0x3615);       /* Persist */
               sleep(2);
               if (!err)
                  err = co2_command(0x3615);    /* Re init */
               sleep(5);
               if (!err)
                  err = co2_command(cmd = 0x3682);      /* Serial */
            }
            if (cmd == 0x3682 && !err)
            {                   /* Get serial/etc */
               uint8_t buf[9];
               err = co2_read(9, buf);
               if (!err && co2_crc(buf[0], buf[1]) == buf[2] && co2_crc(buf[3], buf[4]) == buf[5] && co2_crc(buf[6], buf[7]) == buf[8])
                  scd_serial = ((unsigned long long) buf[0] << 40) + ((unsigned long long) buf[1] << 32) + ((unsigned long long) buf[3] << 24) + ((unsigned long long) buf[4] << 16) + ((unsigned long long) buf[6] << 8) + ((unsigned long long) buf[7]);
               if (!err)
                  err = co2_command(0x2318);
               if (!err)
                  err = co2_read(3, buf);
               if (!err && co2_crc(buf[0], buf[1]) == buf[2])
                  scd_tempoffset = (buf[0] << 8) + buf[1];
               if (!err)
                  err = co2_command(0x2322);
               if (!err)
                  err = co2_read(3, buf);
               if (!err && co2_crc(buf[0], buf[1]) == buf[2])
                  scd_altitude = (buf[0] << 8) + buf[1];
               if (!err)
                  err = co2_command(0x2313);
               if (!err)
                  err = co2_read(3, buf);
               if (!err && co2_crc(buf[0], buf[1]) == buf[2])
                  scd_selfcal = (buf[0] << 8) + buf[1];
               sendinfo = 1;
            }
            if (scd41)
               co2_scd41_start_measure();
            continue;
         }
         {                      /* Ready status */
            err = co2_command(scd41 ? 0xe4b8 : 0x0202);
            if (err)
            {
               ESP_LOGI(TAG, "Tx GetReady %s", esp_err_to_name(err));
               continue;
            }
            {                   /* Read status */
               uint8_t buf[3];
               err = co2_read(sizeof(buf), buf);
               if (err)
               {
                  ESP_LOGI(TAG, "Rx GetReady %s", esp_err_to_name(err));
                  continue;
               }
               if (co2_crc(buf[0], buf[1]) != buf[2])
               {
                  ESP_LOGI(TAG, "Rx GetReady CRC error %02X %02X", co2_crc(buf[0], buf[1]), buf[2]);
                  continue;
               }
               if (scd41 && !(buf[0] & 0x80))
                  co2_scd41_start_measure();    /* Undocumented but top bit 8 if not running */
               if (!scd41 && (buf[0] << 8) + buf[1] != 1)
                  continue;     /* Not ready(1 means ready) */
               if (scd41 && !(buf[0] & 7) && !buf[1])
                  continue;     /* Not ready(least 11 bits 0 means not ready) */
            }
         }
         /* Data */
         if (scd41)
         {
            err = co2_command(0xec05);  /* read data */
            if (err)
            {
               ESP_LOGI(TAG, "Tx GetData %s", esp_err_to_name(err));
               continue;
            }
            uint8_t buf[9];
            err = co2_read(sizeof(buf), buf);
            if (err)
            {
               ESP_LOGI(TAG, "Rx Data %s", esp_err_to_name(err));
               continue;
            }
            if (co2_crc(buf[0], buf[1]) != buf[2] || co2_crc(buf[3], buf[4]) != buf[5] || co2_crc(buf[6], buf[7]) != buf[8])
            {
               ESP_LOGI(TAG, "Rx bad CRC");
               continue;
            }
            float c = (float) ((buf[0] << 8) + buf[1]);
            float t = -45.0 + 175.0 * (float) ((buf[3] << 8) + buf[4]) / 65536.0;
            float r = 100.0 * (float) ((buf[6] << 8) + buf[7]) / 65536.0;
            if (c > 0)
            {
               if (isnan(thisco2))
                  thisco2 = c;  /* First */
               else
                  thisco2 = (thisco2 * co2damp + c) / (co2damp + 1);
               lastco2 = report("co2", lastco2, thisco2, co2places);
            }
            if (!scd41_settled)
            {
               if (r > 0)
               {
                  if (isnan(thisrh))
                     thisrh = r;        /* First */
                  else
                     thisrh = (thisrh * rhdamp + r) / (rhdamp + 1);
                  lastrh = report("rh", lastrh, thisrh, rhplaces);
               }
               if (!num_ds18b20)
                  lasttemp = report("temp", lasttemp, thistemp = t, tempplaces);        /* Treat as temp not itemp as we trust the SCD41 to * be sane */
            }
         } else
         {                      /* Wait for data to be ready */
            err = co2_command(0x0300);
            if (err)
            {
               ESP_LOGI(TAG, "Tx GetData %s", esp_err_to_name(err));
               continue;
            }
            {
               uint8_t buf[18];
               err = co2_read(sizeof(buf), buf);
               if (err)
               {
                  ESP_LOGI(TAG, "Rx Data %s", esp_err_to_name(err));
                  continue;
               }
               /* ESP_LOG_BUFFER_HEX_LEVEL(TAG, buf, 18, ESP_LOG_INFO); */
               uint8_t d[4];
               d[3] = buf[0];
               d[2] = buf[1];
               d[1] = buf[3];
               d[0] = buf[4];
               float co2 = *(float *) d;
               if (co2_crc(buf[0], buf[1]) != buf[2] || co2_crc(buf[3], buf[4]) != buf[5])
                  co2 = -1;
               d[3] = buf[6];
               d[2] = buf[7];
               d[1] = buf[9];
               d[0] = buf[10];
               float t = *(float *) d;
               if (co2_crc(buf[6], buf[7]) != buf[8] || co2_crc(buf[9], buf[10]) != buf[11])
                  t = -1000;
               d[3] = buf[12];
               d[2] = buf[13];
               d[1] = buf[15];
               d[0] = buf[16];
               float rh = *(float *) d;
               if (co2_crc(buf[12], buf[13]) != buf[14] || co2_crc(buf[15], buf[16]) != buf[17])
                  rh = -1000;
               if (co2 > 100)
               {                /* Have a reading */
                  if (isnan(thisco2))
                     thisco2 = co2;     /* First */
                  else
                     thisco2 = (thisco2 * co2damp + co2) / (co2damp + 1);
               }
               if (rh > 0)
               {                /* Have a reading */
                  if (isnan(thisrh))
                     thisrh = rh;       /* First */
                  else
                     thisrh = (thisrh * rhdamp + rh) / (rhdamp + 1);
               }
               if (!num_ds18b20 && t >= -1000)
                  lasttemp = report("itemp", lasttemp, thistemp = t, tempplaces);
               /* Use temp here as no DS18B20 */
               lastco2 = report("co2", lastco2, thisco2, co2places);
               lastrh = report("rh", lastrh, thisrh, rhplaces);
            }
         }
      }
      if (als_found)
         lastals = report("als", lastals, als_read(0x05), alsplaces);
   }
}

void ds18b20_task(void *p)
{
   p = p;
   ESP_LOGI(TAG, "DS18B20 start (%d sensor%s)", num_ds18b20, num_ds18b20 == 1 ? "" : "s");
   if (!ds18b20_setResolution(adr_ds18b20, num_ds18b20, 12))
      ESP_LOGE(TAG, "DS18B20 set failed");
   while (1)
   {
      usleep(250000);
      ds18b20_requestTemperatures();
      float c[num_ds18b20];
      for (int i = 0; i < num_ds18b20; ++i)
         c[i] = ds18b20_getTempC(&adr_ds18b20[i]);
      if (!isnan(c[0]))
         lasttemp = report("temp", lasttemp, thistemp = c[0] + ((float) ds18b20mC) / 1000.0, tempplaces);
      if (num_ds18b20 > 1 && !isnan(c[1]))
         lastotemp = report("otemp", lastotemp, c[1], tempplaces);
   }
}

void menuinit(void)
{                               /* Common menu stuff */
   gfx_set_contrast(gfxlight);
   gfx_clear(0);
}

void gfx_temp(float t)
{
   char s[30];                  /* Temp string */
   if (f)
   {                            /* Fahrenheit */
      int fh = (t + 40.0) * 1.8 - 40.0;
      if (fh <= -100)
         strcpy(s, "___");
      else if (fh >= 1000)
         strcpy(s, "^^^");
      else
         sprintf(s, "%3d", fh);
   } else
   {                            /* Celsius */
      if (t <= -10)
         strcpy(s, "__._");
      else if (t >= 100)
         strcpy(s, "^^.^");
      else
         sprintf(s, "%4.1f", t);
   }
   gfx_text(5, s);
   gfx_text(1, "o");
   gfx_pos(gfx_x(), gfx_y(), GFX_T | GFX_L | GFX_V);
   gfx_text(2, f ? "F" : "C");
   if (!num_ds18b20 && !scd41)
      gfx_text(2, "~");
}

uint8_t menufunc1(char key)
{                               /* Initial menu: Temp control - eventually menu structure somehow... */
   if (key == '2')
      return 0;
   int space = (gfx_height() - 35 * 3) / 2,
       y = 0;
   menuinit();
   float d = 0;
   if (key == '1')
      d = 0.1;
   if (key == '3')
      d = -0.1;
   gfx_pos(10, y, GFX_T | GFX_L | GFX_H);
   gfx_colour('R');
   if (!isnan(temptargetmax))
      gfx_temp(temptargetmax + d);
   y += 35 + space;
   gfx_pos(10, y, GFX_T | GFX_L | GFX_H);
   if (isnan(thistemp))
   {
      gfx_colour('O');
      gfx_text(5, "WAIT");
   } else
   {
      gfx_colour(thistemp < temptargetmin + d ? 'B' : thistemp > temptargetmax + d ? 'R' : 'G');
      gfx_temp(thistemp);
   }
   y += 35 + space;
   gfx_pos(10, y, GFX_T | GFX_L | GFX_H);
   gfx_colour('B');
   if (!isnan(temptargetmin))
      gfx_temp(temptargetmin + d);
   if (temptimeprev < 0)
      return 0;
   if (key)
   {                            /* store settings */
      jo_t j = jo_object_alloc();
      char s[30];
      if (!isnan(temptargetmin))
      {
         sprintf(s, "tempminmC%d", temptimeprev + 1);
         jo_int(j, s, 1000 * d + tempminmC[temptimeprev]);
         sprintf(s, "tempminmC%d", temptimenext + 1);
         jo_int(j, s, 1000 * d + tempminmC[temptimenext]);
      }
      if (!isnan(temptargetmax))
      {
         sprintf(s, "tempmaxmC%d", temptimeprev + 1);
         jo_int(j, s, 1000 * d + tempmaxmC[temptimeprev]);
         sprintf(s, "tempmaxmC%d", temptimenext + 1);
         jo_int(j, s, 1000 * d + tempmaxmC[temptimenext]);
      }
      revk_setting(j);
      jo_free(&j);
   }
   return 1;
}

typedef uint8_t menufunc_t(char);
menufunc_t *menufunc[] = {
   menufunc1,
};

void app_main()
{
   revk_boot(&app_callback);
   revk_register("heat", 0, 0, &heaton, NULL, SETTING_SECRET);
   revk_register("fan", 0, 0, &fanon, NULL, SETTING_SECRET);
   revk_register("gfx", 0, sizeof(gfxmosi), &gfxmosi, NULL, SETTING_SECRET);
   revk_register("co2", 0, sizeof(co2places), &co2places, "-1", SETTING_SIGNED | SETTING_SECRET);
   revk_register("hhmm", 0, sizeof(hhmmday), &hhmmday, NULL, SETTING_SECRET);
#define str(x) #x
#define b(n) revk_register(#n,0,sizeof(n),&n,NULL,SETTING_BOOLEAN);
#define u32(n,d) revk_register(#n,0,sizeof(n),&n,#d,0);
#define u16(n,d) revk_register(#n,0,sizeof(n),&n,#d,0);
#define u16a(n,q) revk_register(#n,q,sizeof(*n),&n,NULL,SETTING_LIVE);
#define s32(n,d) revk_register(#n,0,sizeof(n),&n,#d,SETTING_SIGNED|SETTING_LIVE);
#define s32a(n,q) revk_register(#n,q,sizeof(*n),&n,NULL,SETTING_SIGNED|SETTING_LIVE);
#define s8(n,d) revk_register(#n,0,sizeof(n),&n,#d,SETTING_SIGNED);
#define u8(n,d) revk_register(#n,0,sizeof(n),&n,#d,0);
#define s(n) revk_register(#n,0,0,&n,NULL,0);
#define io(n,d)         revk_register(#n,0,sizeof(n),&n,"- "str(d),SETTING_SET|SETTING_BITFIELD);
#define ioa(n,a,d)      revk_register(#n,a,sizeof(*n),&n,"- "str(d),SETTING_SET|SETTING_BITFIELD);
   settings
#undef u32
#undef u16
#undef u16a
#undef s32
#undef s32a
#undef s8
#undef u8
#undef b
#undef s
#undef io
#undef ioa
       revk_register("logo", 0, sizeof(logo), &logo, NULL, SETTING_BINDATA);    /* fixed logo */
   revk_start();
   if (fanco2gpio)
      gpio_set_direction(fanco2gpio & IO_MASK, GPIO_MODE_OUTPUT);
   if (heatgpio)
      gpio_set_direction(heatgpio & IO_MASK, GPIO_MODE_OUTPUT);
   {
      int p;
      for (p = 0; p < sizeof(logo) && !logo[p]; p++);
      if (p == sizeof(logo))
         memcpy(logo, icon_logo, sizeof(icon_logo));    /* default */
   }
   if (sda && scl)
   {
      scd41 = (co2address == 0x62 ? 1 : 0);
      i2cport = 0;
      if (i2c_driver_install(i2cport, I2C_MODE_MASTER, 0, 0, 0))
      {
         jo_t j = jo_object_alloc();
         jo_string(j, "error", "Install fail");
         jo_int(j, "sda", sda & IO_MASK);
         jo_int(j, "scl", scl & IO_MASK);
         revk_error("I2C", &j);
         i2cport = -1;
      } else
      {
         i2c_config_t config = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = sda & IO_MASK,
            .scl_io_num = scl & IO_MASK,
            .sda_pullup_en = true,
            .scl_pullup_en = true,
            .master.clk_speed = 100000,
         };
         if (i2c_param_config(i2cport, &config))
         {
            i2c_driver_delete(i2cport);
            jo_t j = jo_object_alloc();
            jo_string(j, "error", "Config fail");
            jo_int(j, "sda", sda & IO_MASK);
            jo_int(j, "scl", scl & IO_MASK);
            revk_error("I2C", &j);
            i2cport = -1;
         } else
            i2c_set_timeout(i2cport, 80000 * 5);        /* 5 ms ? allow for clock stretching */
      }
   }
#ifndef	CONFIG_GFX_NONE
   if (gfxmosi)
   {
    const char *e = gfx_init(cs: gfxcs & IO_MASK, sck: gfxsck & IO_MASK, mosi: gfxmosi & IO_MASK, dc: gfxdc & IO_MASK, rst: gfxrst & IO_MASK, flip:gfxflip);
      if (e)
      {
         jo_t j = jo_object_alloc();
         jo_string(j, "error", "Failed to start");
         jo_string(j, "description", e);
         revk_error("GFX", &j);
      }
   }
#endif
   for (int i = 0; i < sizeof(button) / sizeof(*button); i++)
      if (button[i])
      {                         /* Control buttons */
         gpio_reset_pin(button[i] & IO_MASK);
         gpio_set_direction(button[i] & IO_MASK, GPIO_MODE_INPUT);
         gpio_set_pull_mode(button[i] & IO_MASK, GPIO_PULLUP_ONLY);
      }
   gfx_lock();
   gfx_colour('B');
   gfx_box(gfx_width(), gfx_height(), 255);
   gfx_unlock();
   if (i2cport >= 0)
      revk_task("I2C", i2c_task, NULL);
   if (ds18b20)
   {                            /* DS18B20 init */
      ds18b20_init(ds18b20 & IO_MASK);
      int try = 0;
      while (try++ < 5)
      {
         ds18b20_reset_search();
         while (num_ds18b20 < sizeof(adr_ds18b20) / sizeof(*adr_ds18b20) && ds18b20_search(adr_ds18b20[num_ds18b20], true))
            num_ds18b20++;
         if (num_ds18b20)
            break;
         usleep(100000);
      }
      if (!num_ds18b20)
      {
         jo_t j = jo_object_alloc();
         jo_string(j, "error", "No DS18B20 devices");
         jo_int(j, "port", ds18b20 & IO_MASK);
         revk_error("temp", &j);
         ESP_LOGE(TAG, "No DS18B20");
      } else
      {
         revk_task("DS18B20", ds18b20_task, NULL);
         sendinfo = 1;
      }
   }
   gfx_lock();
   gfx_clear(0);
   gfx_unlock();
   /* Main task... */
   time_t showtime = 0;
   char showlogo = 1;
   float showco2 = NAN;
   float showtemp = NAN;
   float showrh = NAN;
   void reset(void) {           /* re display all */
      gfx_clear(0);
      showlogo = 1;
      showtime = 0;
      showco2 = NAN;
      showtemp = NAN;
      showrh = NAN;
      lasticon = 0;
   };
   if (alsdark && sda && scl && alsaddress)
      gfx_dark = 1;             // Start dark
   while (1)
   {                            /* Main loop - handles display and UI, etc. */
      if (!airconlast || airconlast + 300 < uptime())
      {                         // Not seen aircon for a while
         airconmode = 0;
         airconpower = 0;
         airconlast = 0;
      }
      char icon = 0;
      if (airconpower)
         icon = airconmode;     // Display icon
      usleep(10000LL - (esp_timer_get_time() % 10000LL));       /* wait a bit */
      time_t now = time(0);
      struct tm t;
      localtime_r(&now, &t);
      uint16_t hhmm = t.tm_hour * 100 + t.tm_min;
      uint32_t up = uptime();
      if (sendinfo && (co2_found || num_ds18b20) && !do_co2)
      {                         /* Send device info */
         sendinfo = 0;
         jo_t j = jo_object_alloc();
         if (co2_found)
         {
            jo_object(j, scd41 ? "SCD41" : "SCD30");
            if (scd_serial)
               jo_stringf(j, "serial", "%012llX", scd_serial);
            if (scd_tempoffset)
               jo_int(j, "temperature-offset", scd_tempoffset);
            if (scd_altitude)
               jo_int(j, "sensor-altitude", scd_altitude);
            if (scd41)
               jo_bool(j, "automatic-self-calibration", scd_selfcal);
            jo_close(j);
         }
         if (num_ds18b20)
         {
            jo_array(j, "DS18B20");
            for (int i = 0; i < num_ds18b20; i++)
               jo_stringf(j, NULL, "%016llX", *(unsigned long long *) &adr_ds18b20[i]);
            jo_close(j);
         }
         revk_info("info", &j);
      }

      if (!reportconfig && up > 10)
         sendconfig();
      if (!isnan(lastals))
      {                         // ALS based night mode
         if (lastals > alsdark)
            gfx_dark = 0;
         else
            gfx_dark = 1;
      } else if (hhmmnight || hhmmday)
      {                         /* Time bases night mode */
         if (hhmmnight > hhmmday && hhmm >= hhmmnight)
            gfx_dark = 1;
         else if (hhmm >= hhmmday)
            gfx_dark = 0;
         else if (hhmm >= hhmmnight)
            gfx_dark = 1;
      }
      /* Reference temp */
      /* heatdaymC or heatnightmC take priority.If not set(0) then temphhmm / tempminmC / tempmaxmC apply */
      /*
       * The temp are a set, with hhmm points(in order, can start 0000) and heating and cooling settings, and 0 means same as other
       * setting
       */
      int32_t heat_target = (gfx_dark ? heatnightmC : heatdaymC);
      if (!tempminmC[0] && !tempmaxmC[0])
      {                         /* Temp is set based on night / day, use that as heating basis(min) and no cooling set - legacy */
         temptargetmin = ((float) heat_target) / 1000.0;
         temptargetmax = 32.0;
         temptimeprev = temptimenext = -1;
      } else
      {
         /* Setting from temphhmm / tempminmC / tempmaxmC */
#define	TIMES	(sizeof(temphhmm)/sizeof(*temphhmm))
         int i;
         temptimeprev = 0;
         temptimenext = 0;
         for (i = 0; i < TIMES && (tempminmC[i] || tempmaxmC[i]) && temphhmm[i] <= hhmm; i++);
         if (!i)
         {                      /* wrap as first entry is later */
            for (i = 1; i < TIMES && (tempminmC[i] || tempmaxmC[i]); i++);
            temptimeprev = i - 1;
            temptimenext = 0;
         } else if (i < TIMES && temphhmm[i] > hhmm)
         {                      /* Simple range */
            temptimeprev = i - 1;
            temptimenext = i;
         } else
            temptimeprev = i - 1;       /* Next is 0, wrapping */
         int sprev = (temphhmm[temptimeprev] / 100) * 3600 + (temphhmm[temptimeprev] % 100) * 60;
         int snext = (temphhmm[temptimenext] / 100) * 3600 + (temphhmm[temptimenext] % 100) * 60;
         if (snext <= sprev)
            snext += 86400;
         int snow = t.tm_hour * 3600 + t.tm_min * 60 + t.tm_sec;
         if (snow < sprev)
            snow += 86400;
         float min = NAN,
             max = NAN;
         int a,
          b;
         if ((a = (tempminmC[temptimeprev] ? : tempmaxmC[temptimeprev])) && (b = (tempminmC[temptimenext] ? : tempmaxmC[temptimenext])))
         {                      /* Heat valid */
            heat_target = a + (b - a) * (snow - sprev) / (snext - sprev);
            min = ((float) heat_target) / 1000.0;
         }
         if ((a = (tempmaxmC[temptimeprev] ? : tempminmC[temptimeprev])) && (b = (tempmaxmC[temptimenext] ? : tempminmC[temptimenext])))
            max = (float) (a + (b - a) * (snow - sprev) / (snext - sprev)) / 1000.0;    /* Cool valid */
         else
            max = min;          /* same as heat */
         temptargetmin = min;
         temptargetmax = max;
      }
      if (!isnan(tempoverridemin))
      {
         temptargetmin = tempoverridemin;
         heat_target = temptargetmin * 1000;
         temptimeprev = temptimenext = -1;
      }
      if (!isnan(tempoverridemax))
      {
         temptargetmax = tempoverridemax;
         temptimeprev = temptimenext = -1;
      }
      /* Report */
      if (up > 60 || sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED)
         reportall(now);        /* Don 't report right away if clock may be duff */
      static uint32_t fanwait = 0;
      if (!isnan(thisco2) && !isnan(thisrh) && (fanlast < 0 || fanwait < up) && (fanco2on || fanco2off || fanrhon || fanrhoff))
      {                         /* Fan control */
         const char *fan = NULL;
         if (((fanco2on && thisco2 > fanco2on) || (fanrhon && thisrh > fanrhon)))
         {                      /* Fan on */
            if (fanlast != 1)
            {                   /* Change */
               if (fanco2gpio)
                  gpio_set_level(fanco2gpio & IO_MASK, (fanco2gpio & IO_INV) ? 0 : 1);
               fan = fanon;
               fanlast = 1;
            }
         } else if (fanlast != 0)
         {                      /* Fan off, change */
            if (fanco2gpio)
               gpio_set_level(fanco2gpio & IO_MASK, (fanco2gpio & IO_INV) ? 1 : 0);
            fan = fanoff;
            fanlast = 0;
         }
         if (!fan && fanresend && fantime < up && fanlast > 0)
            fan = (fanlast ? fanon : fanoff);
         if (fan && *fan)
         {
            fanwait = up + fanswitch;
            fantime = up + fanresend;
            revk_mqtt_send_str(fan);
            if (fanlast > fanmax)
               fanmax = fanlast;
         }
      }
      if (fanon && *fanon && fanlast == 1)
         icon = 'E';            // Extractor fan icon
      static uint32_t heatwait = 0;
      if (!isnan(thistemp) && (heatlast < 0 || heatwait < up) && (heatnightmC || heatdaymC || tempminmC[0] || heat_target || heatgpio || heaton || heatoff))
      {                         /* Heat control */
         if (heat_target || heatlast == 1)
         {                      /* We have a reference temp to work with or we left on */
            if (heatresend && heattime < up)
               heatlast = -1;
            const char *heat = NULL;
            int32_t thismC = thistemp * 1000;
            if (!heat_target || thismC > heat_target || (airconpower && airconmode != 'H'))
            {                   /* Heat off */
               if (heatlast != 0)
               {                /* Change */
                  if (heatgpio)
                     gpio_set_level(heatgpio & IO_MASK, (heatgpio & IO_INV) ? 1 : 0);
                  heat = heatoff;
                  heatlast = 0;
               }
            } else if (heatlast != 1)
            {                   /* Heat on, change */
               if (heatgpio)
                  gpio_set_level(heatgpio & IO_MASK, (heatgpio & IO_INV) ? 0 : 1);
               heat = heaton;
               heatlast = 1;
            }
            if (heat && *heat)
            {
               heatwait = up + heatswitch;
               heattime = up + heatresend;
               revk_mqtt_send_str(heat);
               if (heatlast > heatmax)
                  heatmax = heatlast;
            }
         }
      }
      if (heaton && *heaton && heatlast == 1)
         icon = 'R';            // Radiator icon
      static uint8_t menu = 0;  /* Menu selection - 0 if idle */
      /* Handle key presses */
      char key = 0;
      {                         /* Simple debounce */
         uint8_t last0 = 0;
         static uint8_t last1 = 0;
         static uint8_t last2 = 0;
         for (int i = 0; i < sizeof(button) / sizeof(*button); i++)
            if (button[i] && !gpio_get_level(button[i] & IO_MASK))
               last0 |= (1 << i);
         if (last0 == last1)
         {                      // stable
            for (int i = 0; i < sizeof(button) / sizeof(*button); i++)
               if (!(last2 & (1 << i)) && (last1 & (1 << i)))
                  key = '1' + i;
            last2 = last1;
         }
         last1 = last0;
      }
      if (!menu && key)
      {
         menu_time = uptime() + 5;
         menu = 1;              // Base menu
         key = 0;               // Don't pass initial key, used just to wake up...
      }
      /* Display */
      char s[30];               /* Temp string */
      if (gfx_msg_time)
      {                         /* Fixed message */
         gfx_set_contrast(gfxlight);
         if (menu || gfx_msg_time < uptime())
         {                      /* Time up, clear and drop through */
            gfx_msg_time = 0;
            gfx_lock();
            reset();
            gfx_unlock();
         } else
         {
            gfx_message(gfx_msg);       /* does own locking */
            continue;
         }
      }
      gfx_lock();
      if (menu)
      {                         /* Display menu selection */
         if (key)
            menu_time = uptime() + 10;
         if (menu_time < uptime())
            menu = 0;
         else if (menu > sizeof(menufunc) / sizeof(*menufunc))
            menu = 0;
         else if (menu)
            menu = menufunc[menu - 1] (key);
         if (!menu)
            reset();            // Exit menu
         else
         {
            gfx_unlock();
            continue;
         }
      }
      if (gfx_dark)
      {                         /* Night mode */
         gfx_set_contrast(gfxdark);     /* Dark / dim */
         revk_blink(0, 0, "K");
         gfx_colour('b');
         reset();
         if (!notime)
         {
            strftime(s, sizeof(s), "%H:%M", &t);
            int d = t.tm_sec;
            if (t.tm_min & 1)
               d = 60 - d;
            int y = gfx_height() / 2 + (d - 30);
            d = t.tm_min;
            if (t.tm_hour & 1)
               d = 60 - d;
            int x = gfx_width() / 2 + (d - 30) * 2 / 6; /* Adjusted to fit display */
            gfx_pos(x, y, GFX_M | GFX_C);
            gfx_text(4, s);
         }
         gfx_unlock();
         continue;
      }
      /* Normal - idle temp status */
      gfx_set_contrast(gfxlight);
      if (showlogo)
      {
         showlogo = 0;
         gfx_clear(0);
         if (!nologo)
         {
            gfx_pos(gfx_width() - LOGOW, gfx_width() - 12, GFX_B | GFX_L);
            gfx_icon16(LOGOW, LOGOH, logo);
         }
      }
      if (now != showtime && !notime)
      {
         showtime = now;
         if (t.tm_year > 100)
         {
            strftime(s, sizeof(s), "%F\004%T %Z", &t);
            gfx_pos(0, gfx_height() - 1, GFX_B | GFX_L);
            gfx_text(1, s);
         }
      }
      int y = 0,
          space = (gfx_height() - 28 - 35 - 21 - 9) / 3;
      char co2col = (isnan(thisco2) ? 'K' : thisco2 > (fanco2on ? : 1000) ? 'R' : thisco2 > (fanco2off ? : 750) ? 'Y' : 'G');
      char tempcol = (isnan(thistemp) ? 'K' : thistemp > temptargetmax + 0.25 ? 'R' : thistemp < temptargetmin - 0.25 ? 'B' : 'G');
      char rhcol = (isnan(thisrh) ? 'K' : 'C');
      {                         /* Colours for LED */
         static char cols[4];
         char *c = cols;
         if (co2col != 'K')
            *c++ = co2col;
         if (tempcol != 'K')
            *c++ = tempcol;
         if (rhcol != 'K')
            *c++ = rhcol;
         *c = 0;
         revk_blink(0, 0, *cols ? cols : NULL);
      }
      if (thisco2 != showco2 && !isnan(thisco2))
      {
         showco2 = thisco2;
         gfx_colour(co2col);
         if (showco2 < 200)
            strcpy(s, "?LOW");
         else if (showco2 >= 10000.0)
            strcpy(s, "HIGH");
         else
            sprintf(s, "%4d", (int) showco2);
         gfx_pos(4, y, GFX_T | GFX_L | GFX_H);
         gfx_text(4, s);
         int x;
         gfx_pos(x = gfx_x(), gfx_y(), GFX_T | GFX_L | GFX_H);
         gfx_text(1, "CO");
         gfx_pos(gfx_x(), gfx_y() + 3, GFX_T | GFX_L | GFX_V);
         gfx_text(0, "2");
         gfx_pos(x, gfx_y(), GFX_T | GFX_L | GFX_V);
         gfx_text(-1, "ppm");
      }
      y += 28 + space;
      gfx_pos(10, y, GFX_T | GFX_L | GFX_H);
      if (!num_ds18b20 && scd41 && scd41_settled >= up)
      {                         // Waiting
         sprintf(s, "%ld:%02ld ", (scd41_settled - up) / 60, (scd41_settled - up) % 60);
         gfx_colour('O');
         gfx_text(5, scd41_settled == up ? "-:--" : s);
         showtemp = NAN;
      } else
      {
         scd41_settled = 0;
         if (!isnan(thistemp) && thistemp != showtemp)
         {
            showtemp = thistemp;
            gfx_colour(tempcol);
            gfx_temp(showtemp);
         }
      }
      y += 35 + space;
      if (thisrh != showrh && !isnan(thisrh))
      {
         showrh = thisrh;
         gfx_colour(rhcol);
         gfx_pos(3, y, GFX_T | GFX_L | GFX_H);
         if (showrh <= 0)
            strcpy(s, "__");
         else if (showrh >= 100)
            strcpy(s, "^^");
         else
            sprintf(s, "%2d", (int) showrh);
         gfx_text(3, s);
         gfx_text(2, "%%");
         gfx_text(1, "R");
         gfx_text(1, "H");
      }
      y += 21 + space;
      if (!icon && airconlast)
         icon = 'P';            // power
      display_icon = icon;
      if (display_icon != lasticon)
      {                         // Status icon
         lasticon = display_icon;
         gfx_pos(gfx_width() - LOGOW * 2 - 2, gfx_height() - 12, GFX_B | GFX_L);
         gfx_colour(icon == 'R' ? 'r' : icon == 'F' ? 'C' : icon == 'E' ? 'g' : icon == 'C' ? 'B' : icon == 'H' ? 'R' : icon == 'D' ? 'Y' : icon == 'A' ? 'G' : icon == 'P' ? 'w' : 'W');
         gfx_icon16(LOGOW, LOGOH, icon == 'R' ? icon_rad : icon == 'F' ? icon_modeF : icon == 'E' ? icon_fan : icon == 'C' ? icon_modeC : icon == 'H' ? icon_modeH : icon == 'D' ? icon_modeD : icon == 'A' ? icon_modeA : icon == 'P' ? icon_power : NULL);
      }
      gfx_unlock();
   }
}

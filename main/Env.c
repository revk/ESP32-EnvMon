// CO2 + other sensors all
/* Copyright(c) 2019-21 Adrian Kennard, Andrews & Arnold Limited, see LICENSE file(GPL) */
const char TAG[] = "Env";

//#define       DEBUGTEMP

#include "revk.h"
#include <driver/i2c.h>
#include <hal/spi_types.h>
#include <math.h>
#include <sntp.h>

#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"
#include "gfx.h"

#include "logo.h"
#include "fan.h"
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

// temp targets of 0 mean off, so use 1 or -1 if really targeting 0

#define settings	\
	u32(reporting,60)	\
	u8(lag,5)	\
	s8(co2sda,-1)	\
	s8(co2scl,-1)	\
	s8(co2address,0x62)	\
	s8(co2places,-1)	\
	u32(co2damp,100)	\
	s8(tempplaces,1)	\
	s8(rhplaces,0)	\
	u32(rhdamp,10)	\
	s8(ds18b20,-1)	\
	s32(ds18b20mC,0)	\
	u8(gfxmosi,)	\
	u8(gfxsck,)	\
	u8(gfxcs,)	\
	u8(gfxdc,)	\
	u8(gfxrst,)	\
	u8(gfxflip,)	\
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
	s8(fanco2gpio,-1)	\
	u32(fanresend,600)	\
	s(heaton)	\
	s(heatoff)	\
	s(heataircon)	\
	u8(heatswitch,30)	\
	u32(heatresend,600)	\
	s8(heatgpio,-1)	\
	u16(hhmmnight,0)	\
	u16(hhmmday,0)		\
	b(nologo)	\
	b(notime)	\
	s32(heatdaymC,0)	\
	s32(heatnightmC,0)	\
	u16a(temphhmm,10)	\
	s32a(tempheatmC,10)	\
	s32a(tempcoolmC,10)	\

#define u32(n,d)	uint32_t n;
#define u16(n,d)	uint16_t n;
#define u16a(n,q)	uint16_t n[q];
#define s32(n,d)	int32_t n;
#define s32a(n,q)	int32_t n[q];
#define s8(n,d)	int8_t n;
#define u8(n,d)	uint8_t n;
#define b(n) uint8_t n;
#define s(n) char * n;
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
static uint8_t scd41 = 0;
static uint32_t scd41_settled = 0;      // uptime when started measurements

static uint8_t logo[LOGOW * LOGOH / 2];
static float lastco2 = NAN;
static float lastrh = NAN;
static float lasttemp = NAN;
static float lastotemp = NAN;
static float thisco2 = NAN;
static float thistemp = NAN;
static float thisrh = NAN;
static float temptargetmin = NAN;
static float temptargetmax = NAN;
static int8_t co2port = -1;
static int8_t num_owb = 0;
static volatile uint32_t do_co2 = 0;
static OneWireBus *owb = NULL;
static owb_rmt_driver_info rmt_driver_info;
static DS18B20_Info *ds18b20s[MAX_OWB] = { 0 };

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
{
   // Do reporting of values
   if ((!reportchange || now < reportchange + lag) && (!reporting || now / reporting == reportlast / reporting))
      return;
   // Slight delay on changes
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
      revk_state("data", &j);
      if (*heataircon && !isnan(lasttemp))
      {                         // Aircon control
         static float last = NAN,
             lasttemptargetmin = NAN,
             lasttemptargetmax = NAN;
         float this = lasttemp;
         // Work out if changed
         if (!isnan(last) && now / reporting != reportlast / reporting && temptargetmin == lasttemptargetmin && temptargetmax == lasttemptargetmax)
         {                      // Hysteresis
            float mag = powf(10.0, -tempplaces);
            if (this < last)
            {
               this += mag * 0.4;       // Hysteresis, and it would have to go a further 0.5 to flip on the roundf()
               if (this >= last)
                  this = NAN;
            } else if (this > last)
            {
               this -= mag * 0.4;       // Hysteresis, and it would have to go a further 0.5 to flip on the roundf()
               if (this <= last)
                  this = NAN;
            }
         }
         if (!isnan(this))
         {
            char topic[100];
            snprintf(topic, sizeof(topic), "command/%s/control", heataircon);
            jo_t j = jo_object_alloc();
            if (tempplaces <= 0)
               jo_litf(j, "env", "%d", (int) lasttemp);
            else
               jo_litf(j, "env", "%.*f", tempplaces, lasttemp);
            if (!isnan(temptargetmin) && temptargetmin == temptargetmax)
               jo_litf(j, "target", "%.3f", temptargetmin);
            else
            {
               jo_array(j, "target");
               jo_litf(j, NULL, "%.3f", temptargetmin);
               jo_litf(j, NULL, "%.3f", temptargetmax);
               jo_close(j);
            }
            revk_mqtt_send_clients(NULL, 0, topic, &j, 1);
            lasttemptargetmin = temptargetmin;
            lasttemptargetmax = temptargetmax;
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
   {                            // Hysteresis
      if (this < last)
      {
         this += mag * 0.4;     // Hysteresis, and it would have to go a further 0.5 to flip on the roundf()
         if (this >= last)
            return last;
      } else if (this > last)
      {
         this -= mag * 0.4;     // Hysteresis, and it would have to go a further 0.5 to flip on the roundf()
         if (this <= last)
            return last;
      }
   }
   if (!isnan(this))
      this = roundf(this / mag) * mag;  // Rounding
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
   if (ds18b20 >= 0 || co2port >= 0)
      add("Temp", "temperature", "°C", "temp");
   if (co2port >= 0)
      add("R/H", "humidity", "%", "rh");
   if (co2port >= 0)
      add("CO₂", "co2", "ppm", "co2");
}

const char *app_callback(int client, const char *prefix, const char *target, const char *suffix, jo_t j)
{
   if (client || !prefix || target || strcmp(prefix, "command") || !suffix)
      return NULL;
   if (!strcmp(suffix, "send"))
   {
      sendall();
      return "";
   }
   if (!strcmp(suffix, "connect"))
   {
      if (scd41)
         co2_setting(0x3682, 0);
      fanlast = -1;
      heatlast = -1;
      sendall();
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
   {
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
      return co2_setting(scd41 ? 0x362f : 0x5204, jo_read_int(j));      // ppm
   if (!strcmp(suffix, "co2tempoffset"))
      return co2_setting(scd41 ? 0x241d : 0x5403, jo_read_int(j));      // Use T*65536/175
   if (!strcmp(suffix, "co2alt"))
      return co2_setting(scd41 ? 0x2427 : 0x5102, jo_read_int(j));      // m
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
{                               // Set up command
   //ESP_LOGE(TAG, "CO2 cmd %04X", c);
   i2c_cmd_handle_t i = i2c_cmd_link_create();
   i2c_master_start(i);
   i2c_master_write_byte(i, (co2address << 1), ACK_CHECK_EN);
   i2c_master_write_byte(i, c >> 8, ACK_CHECK_EN);
   i2c_master_write_byte(i, c, ACK_CHECK_EN);
   return i;
}

static void co2_add(i2c_cmd_handle_t i, uint16_t v)
{                               // Add word to command
   //ESP_LOGE(TAG, "CO2 val %04X", v);
   i2c_master_write_byte(i, v >> 8, ACK_CHECK_EN);
   i2c_master_write_byte(i, v, ACK_CHECK_EN);
   i2c_master_write_byte(i, co2_crc(v >> 8, v), ACK_CHECK_EN);
}

static esp_err_t co2_done(i2c_cmd_handle_t * i)
{                               // Finish command
   i2c_master_stop(*i);
   esp_err_t err = i2c_master_cmd_begin(co2port, *i, 1000 / portTICK_PERIOD_MS);
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
   scd41_settled = 0;
   esp_err_t err = co2_command(0x3f86); // Stop measurement (SCD41)
   sleep(1);
   return err;
}

static esp_err_t co2_scd41_start_measure(void)
{
   scd41_settled = uptime() + 300;      // Time for temp to settle
   return co2_command(0x21b1);  // Start measurement (SCD41)
}

static const char *co2_setting(uint16_t cmd, uint16_t val)
{
   do_co2 = (cmd << 16) + val;
   return "";
}

void co2_task(void *p)
{
   p = p;
   ESP_LOGI(TAG, "CO2 start");
   int try = 10;
   esp_err_t err = 0;
   while (try--)
   {
      sleep(1);
      if (scd41)
      {
         err = co2_scd41_stop_measure();
         if (!err)
            err = co2_command(0x3646);  // Reinit
         if (!err)
            break;
      } else
      {                         // Just try starting measurement
         err = co2_command(0x0010);     // Start measurement
         if (!err)
            break;
      }
      ESP_LOGI(TAG, "CO2 retry");
   }
   if (err)
   {                            /* failed */
      jo_t j = jo_object_alloc();
      jo_string(j, "error", "Config fail");
      jo_int(j, "code", err);
      jo_string(j, "description", esp_err_to_name(err));
      revk_error("CO2", &j);
      vTaskDelete(NULL);
      return;
   }
   /* Get measurements */
   while (1)
   {
      usleep(100000);
      if (do_co2)
      {                         // Do a command from mqtt
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
            err = co2_command(0x3615);  // Persist
            sleep(1);
         }
         if (cmd == 0x3682 && !err)
         {                      // Get serial
            jo_t j = jo_object_alloc();
            uint8_t buf[9];
            err = co2_read(9, buf);
            if (!err && co2_crc(buf[0], buf[1]) == buf[2] && co2_crc(buf[3], buf[4]) == buf[5] && co2_crc(buf[6], buf[7]) == buf[8])
               jo_stringf(j, "serial", "%02X%02X%02X%02X%02X%02X", buf[0], buf[1], buf[3], buf[4], buf[6], buf[7]);
            if (!err)
               err = co2_command(0x2318);
            if (!err)
               err = co2_read(3, buf);
            if (!err && co2_crc(buf[0], buf[1]) == buf[2])
               jo_int(j, "temperature-offset", (buf[0] << 8) + buf[1]);
            if (!err)
               err = co2_command(0x2322);
            if (!err)
               err = co2_read(3, buf);
            if (!err && co2_crc(buf[0], buf[1]) == buf[2])
               jo_int(j, "sensor-altitude", (buf[0] << 8) + buf[1]);
            if (!err)
               err = co2_command(0x2313);
            if (!err)
               err = co2_read(3, buf);
            if (!err && co2_crc(buf[0], buf[1]) == buf[2])
               jo_bool(j, "automatic-self-calibration", (buf[0] << 8) + buf[1]);
            revk_info("CO2", &j);
         }
         if (scd41)
            co2_scd41_start_measure();
         continue;
      }
      {                         // Ready status
         err = co2_command(scd41 ? 0xe4b8 : 0x0202);
         if (err)
         {
            ESP_LOGI(TAG, "Tx GetReady %s", esp_err_to_name(err));
            continue;
         }
         {                      // Read status
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
               co2_scd41_start_measure();       // Undocumented but top bit 8 if not running
            if (!scd41 && (buf[0] << 8) + buf[1] != 1)
               continue;        // Not ready (1 means ready)
            if (scd41 && !(buf[0] & 7) && !buf[1])
               continue;        // Not ready (least 11 bits 0 means not ready)
         }
      }
      // Data
      if (scd41)
      {
         err = co2_command(0xec05);     // read data
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
         if (scd41_settled && scd41_settled < uptime())
         {
            if (c > 0)
            {
               if (isnan(thisco2))
                  thisco2 = c;  /* First */
               else
                  thisco2 = (thisco2 * co2damp + c) / (co2damp + 1);
               lastco2 = report("co2", lastco2, thisco2, co2places);
            }
            if (r > 0)
            {
               if (isnan(thisrh))
                  thisrh = r;   /* First */
               else
                  thisrh = (thisrh * rhdamp + r) / (rhdamp + 1);
               lastrh = report("rh", lastrh, thisrh, rhplaces);
            }
            if (!num_owb)
               lasttemp = report("temp", lasttemp, thistemp = t, tempplaces);   // Treat as temp not itemp as we trust the SCD41 to be sane
         }
      } else
      {                         // Wait for data to be ready
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
            //ESP_LOG_BUFFER_HEX_LEVEL(TAG, buf, 18, ESP_LOG_INFO);
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
            {                   // Have a reading
               if (isnan(thisco2))
                  thisco2 = co2;        /* First */
               else
                  thisco2 = (thisco2 * co2damp + co2) / (co2damp + 1);
            }
            if (rh > 0)
            {                   // Have a reading
               if (isnan(thisrh))
                  thisrh = rh;  /* First */
               else
                  thisrh = (thisrh * rhdamp + rh) / (rhdamp + 1);
            }
            if (!num_owb && t >= -1000)
               lasttemp = report("itemp", lasttemp, thistemp = t, tempplaces);
            /* Use temp here as no DS18B20 */
            lastco2 = report("co2", lastco2, thisco2, co2places);
            lastrh = report("rh", lastrh, thisrh, rhplaces);
         }
      }
   }
}

void ds18b20_task(void *p)
{
   p = p;
   ESP_LOGI(TAG, "DS18B20 retry");
   while (1)
   {
      usleep(100000);
      ds18b20_convert_all(owb);
      ds18b20_wait_for_conversion(ds18b20s[0]);
      float readings[MAX_OWB] = { 0 };
      DS18B20_ERROR errors[MAX_OWB] = { 0 };
      for (int i = 0; i < num_owb; ++i)
         errors[i] = ds18b20_read_temp(ds18b20s[i], &readings[i]);
      if (!errors[0])
         lasttemp = report("temp", lasttemp, thistemp = readings[0] + ((float) ds18b20mC) / 1000.0, tempplaces);
      if (num_owb > 1 && !errors[1])
         lastotemp = report("otemp", lastotemp, readings[1], tempplaces);
   }
}

void app_main()
{
   revk_boot(&app_callback);
   revk_register("heat", 0, 0, &heaton, NULL, SETTING_SECRET);
   revk_register("fan", 0, 0, &fanon, NULL, SETTING_SECRET);
   revk_register("gfx", 0, sizeof(gfxmosi), &gfxmosi, NULL, SETTING_SECRET);
   revk_register("co2", 0, sizeof(co2places), &co2places, "-1", SETTING_SIGNED | SETTING_SECRET);
   revk_register("hhmm", 0, sizeof(hhmmday), &hhmmday, NULL, SETTING_SECRET);
#define b(n) revk_register(#n,0,sizeof(n),&n,NULL,SETTING_BOOLEAN);
#define u32(n,d) revk_register(#n,0,sizeof(n),&n,#d,0);
#define u16(n,d) revk_register(#n,0,sizeof(n),&n,#d,0);
#define u16a(n,q) revk_register(#n,q,sizeof(*n),&n,NULL,SETTING_LIVE);
#define s32(n,d) revk_register(#n,0,sizeof(n),&n,#d,SETTING_SIGNED|SETTING_LIVE);
#define s32a(n,q) revk_register(#n,q,sizeof(*n),&n,NULL,SETTING_SIGNED|SETTING_LIVE);
#define s8(n,d) revk_register(#n,0,sizeof(n),&n,#d,SETTING_SIGNED);
#define u8(n,d) revk_register(#n,0,sizeof(n),&n,#d,0);
#define s(n) revk_register(#n,0,0,&n,NULL,0);
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
       revk_register("logo", 0, sizeof(logo), &logo, NULL, SETTING_BINDATA);    /* fixed logo */
   revk_start();
   if (fanco2gpio >= 0)
      gpio_set_direction(fanco2gpio, GPIO_MODE_OUTPUT);
   if (heatgpio >= 0)
      gpio_set_direction(heatgpio, GPIO_MODE_OUTPUT);
   {
      int p;
      for (p = 0; p < sizeof(logo) && !logo[p]; p++);
      if (p == sizeof(logo))
         memcpy(logo, aalogo, sizeof(logo));    /* default */
   }
   if (co2sda >= 0 && co2scl >= 0)
   {
      scd41 = (co2address == 0x62 ? 1 : 0);
      co2port = 0;
      if (i2c_driver_install(co2port, I2C_MODE_MASTER, 0, 0, 0))
      {
         jo_t j = jo_object_alloc();
         jo_string(j, "error", "Install fail");
         revk_error("CO2", &j);
         co2port = -1;
      } else
      {
         i2c_config_t config = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = co2sda,
            .scl_io_num = co2scl,
            .sda_pullup_en = true,
            .scl_pullup_en = true,
            .master.clk_speed = 100000,
         };
         if (i2c_param_config(co2port, &config))
         {
            i2c_driver_delete(co2port);
            jo_t j = jo_object_alloc();
            jo_string(j, "error", "Config fail");
            revk_error("CO2", &j);
            co2port = -1;
         } else
            i2c_set_timeout(co2port, 80000 * 5);        /* 5 ms ? allow for clock stretching */
      }
   }
   if (gfxmosi)
   {
    const char *e = gfx_init(cs: gfxcs, sck: gfxsck, mosi: gfxmosi, dc: gfxdc, rst: gfxrst, flip:gfxflip);
      if (e)
      {
         jo_t j = jo_object_alloc();
         jo_string(j, "error", "Failed to start");
         jo_string(j, "description", e);
         revk_error("GFX", &j);
      }
   }
   gfx_lock();
   gfx_colour('B');
   gfx_box(gfx_width(), gfx_height(), 255);
   gfx_unlock();
   if (co2port >= 0)
      revk_task("CO2", co2_task, NULL);
   if (ds18b20 >= 0)
   {                            /* DS18B20 init */
      owb = owb_rmt_initialize(&rmt_driver_info, ds18b20, RMT_CHANNEL_1, RMT_CHANNEL_0);
      owb_use_crc(owb, true);   /* enable CRC check for ROM code */
      OneWireBus_ROMCode device_rom_codes[MAX_OWB] = {
         0
      };
      OneWireBus_SearchState search_state = { 0 };
      bool found = false;
      owb_search_first(owb, &search_state, &found);
      while (found && num_owb < MAX_OWB)
      {
         char rom_code_s[17];
         owb_string_from_rom_code(search_state.rom_code, rom_code_s, sizeof(rom_code_s));
         device_rom_codes[num_owb] = search_state.rom_code;
         ++num_owb;
         owb_search_next(owb, &search_state, &found);
      }
      for (int i = 0; i < num_owb; i++)
      {
         DS18B20_Info *ds18b20_info = ds18b20_malloc(); /* heap allocation */
         ds18b20s[i] = ds18b20_info;
         if (num_owb == 1)
            ds18b20_init_solo(ds18b20_info, owb);       /* only one device on bus */
         else
            ds18b20_init(ds18b20_info, owb, device_rom_codes[i]);       /* associate with bus and device */
         ds18b20_use_crc(ds18b20_info, true);   /* enable CRC check for temperature readings */
         ds18b20_set_resolution(ds18b20_info, DS18B20_RESOLUTION);
      }
      if (!num_owb)
      {
         jo_t j = jo_object_alloc();
         jo_string(j, "error", "No OWB devices");
         revk_error("temp", &j);
      } else
         revk_task("DS18B20", ds18b20_task, NULL);
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
   };
   while (1)
   {
      usleep(100000LL - (esp_timer_get_time() % 100000LL));     /* wait a bit */
      time_t now = time(0);
      struct tm t;
      localtime_r(&now, &t);
      uint16_t hhmm = t.tm_hour * 100 + t.tm_min;
      uint32_t up = uptime();
      if (!reportconfig && up > 10)
         sendconfig();
      if (hhmmnight || hhmmday)
      {                         /* Auto day / night */
         if (hhmmnight > hhmmday && hhmm >= hhmmnight)
            gfx_dark = 1;
         else if (hhmm >= hhmmday)
            gfx_dark = 0;
         else if (hhmm >= hhmmnight)
            gfx_dark = 1;
      }
      // Reference temp
      // heatdaymC or heatnightmC take priority. If not set (0) then temphhmm/tempheatmC/tempcoolmC apply
      // The temp are a set, with hhmm points (in order, can start 0000) and heating and cooling settings, and 0 means same as other setting
      int32_t heat_target = (gfx_dark ? heatnightmC : heatdaymC);
      if (heat_target)
      {                         // Temp is set base don night/day, use that as heating basis (min) and no cooling set
         temptargetmin = ((float) heat_target) / 1000.0;
         temptargetmax = 32.0;
      } else
      {                         // Setting from temphhmm/tempheatmC/tempcoolmC
#define	TIMES	(sizeof(temphhmm)/sizeof(*temphhmm))
         int i,
          prev = 0,
             next = 0;
         for (i = 0; i < TIMES && (tempheatmC[i] || tempcoolmC[i]) && temphhmm[i] <= hhmm; i++);
         if (!i)
         {                      // wrap as first entry is later
            for (i = 1; i < TIMES && (tempheatmC[i] || tempcoolmC[i]); i++);
            prev = i - 1;
            next = 0;
         } else if (i < TIMES && temphhmm[i] > hhmm)
         {                      // Simple range
            prev = i - 1;
            next = i;
         } else
            prev = i - 1;       // Next is 0, wrapping
         int sprev = (temphhmm[prev] / 100) * 3600 + (temphhmm[prev] % 100) * 60;
         int snext = (temphhmm[next] / 100) * 3600 + (temphhmm[next] % 100) * 60;
         if (snext <= sprev)
            snext += 86400;
         int snow = t.tm_hour * 3600 + t.tm_min * 60 + t.tm_sec;
         if (snow < sprev)
            snow += 86400;
         float min = NAN,
             max = NAN;
         int a,
          b;
         if ((a = (tempheatmC[prev] ? : tempcoolmC[prev])) && (b = (tempheatmC[next] ? : tempcoolmC[next])))
         {                      // Heat valid
            heat_target = a + (b - a) * (snow - sprev) / (snext - sprev);
            min = ((float) heat_target) / 1000.0;
         }
         if ((a = (tempcoolmC[prev] ? : tempheatmC[prev])) && (b = (tempcoolmC[next] ? : tempheatmC[next])))
            max = (float) (a + (b - a) * (snow - sprev) / (snext - sprev)) / 1000.0;    // Cool valid
         else
            max = min;          // same as heat
         temptargetmin = min;
         temptargetmax = max;
      }
      // Report
      if (up > 60 || sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED)
         reportall(now);        // Don't report right away if clock may be duff
      static uint32_t fanwait = 0;
      if (!isnan(thisco2) && !isnan(thisrh) && fanwait < up && (fanco2on || fanco2off || fanrhon || fanrhoff))
      {                         /* Fan control */
         const char *fan = NULL;
         if (((fanco2on && thisco2 > fanco2on) || (fanrhon && thisrh > fanrhon)))
         {                      // Fan on
            if (fanlast != 1)
            {                   // Change
               if (fanco2gpio >= 0)
                  gpio_set_level(fanco2gpio, 1);
               fan = fanon;
               fanlast = 1;
            }
         } else if (fanlast != 0)
         {                      // Fan off, change
            if (fanco2gpio >= 0)
               gpio_set_level(fanco2gpio, 0);
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
      static uint32_t heatwait = 0;
      if (!isnan(thistemp) && heatwait < up && (heatnightmC || heatdaymC || tempheatmC[0] || heat_target || heatgpio >= 0 || heaton || heatoff))
      {                         /* Heat control */
         if (heat_target || heatlast == 1)
         {                      /* We have a reference temp to work with or we left on */
            if (heatresend && heattime < up)
               heatlast = -1;
            const char *heat = NULL;
            int32_t thismC = thistemp * 1000;
            if (!heat_target || thismC > heat_target)
            {                   // Heat off
               if (heatlast != 0)
               {                // Change
                  if (heatgpio >= 0)
                     gpio_set_level(heatgpio, 0);
                  heat = heatoff;
                  heatlast = 0;
               }
            } else if (heatlast != 1)
            {                   // Heat on, change
               if (heatgpio >= 0)
                  gpio_set_level(heatgpio, 0);
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
      char s[30];
      /* Display */
      if (gfx_msg_time)
      {                         // Fixed message
         gfx_set_contrast(gfxlight);
         if (gfx_msg_time < uptime())
         {                      // Time up, clear and drop through
            gfx_msg_time = 0;
            gfx_lock();
            reset();
            gfx_unlock();
         } else
         {
            gfx_message(gfx_msg);
            continue;
         }
      }
      gfx_lock();
      if (gfx_dark)
      {                         // Night mode
         gfx_set_contrast(gfxdark);     // Dark/dim
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
            int x = gfx_width() / 2 + (d - 30) * 2 / 6; // Adjusted to fit display
            gfx_pos(x, y, GFX_M | GFX_C);
            gfx_text(4, s);
         }
         gfx_unlock();
         continue;
      }
      // Normal
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
      if (scd41 && isnan(thisco2) && scd41_settled >= up)
      {
         sprintf(s, "%d:%02d", (scd41_settled - up) / 60, (scd41_settled - up) % 60);
         gfx_colour('O');
         gfx_pos(4, 0, GFX_T | GFX_L);
         gfx_text(4, s);
      }
      int y = 0,
          space = (gfx_height() - 28 - 35 - 21 - 9) / 3;
      int32_t reftemp = heat_target ? : 21000;
      int32_t thismC = thistemp * 1000;
      char co2col = (isnan(thisco2) ? 'K' : thisco2 > (fanco2on ? : 1000) ? 'R' : thisco2 > (fanco2off ? : 750) ? 'Y' : 'G');
      char tempcol = (isnan(thistemp) ? 'K' : thismC > reftemp + 500 ? 'R' : thismC > reftemp - 500 ? 'G' : 'B');
      char rhcol = (isnan(thisrh) ? 'K' : 'C');
      {                         // Colours for LED
         static char cols[4];
         char *c = cols;
         if (co2col != 'K')
            *c++ = co2col;
         if (tempcol != 'K')
            *c++ = tempcol;
         if (rhcol != 'K')
            *c++ = rhcol;
         *c = 0;
         revk_blink(0, 0, cols);
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
         gfx_pos(gfx_x(), gfx_y(), GFX_T | GFX_L | GFX_V);
         gfx_text(1, "CO2");
         gfx_text(-1, "ppm");
         if (fanlast >= 0)
         {
            gfx_pos(gfx_width() - LOGOW * 2 - 2, gfx_height() - 12, GFX_B | GFX_L);
            gfx_icon16(LOGOW, LOGOH, fanlast ? fan : NULL);
         }
      }
      y += 28 + space;
      if (thistemp != showtemp && !isnan(thistemp))
      {
         showtemp = thistemp;
         gfx_colour(tempcol);
         gfx_pos(10, y, GFX_T | GFX_L | GFX_H);
         if (f)
         {                      /* Fahrenheit */
            int fh = (showtemp + 40.0) * 1.8 - 40.0;
            if (fh <= -100)
               strcpy(s, "___");
            else if (fh >= 1000)
               strcpy(s, "^^^");
            else
               sprintf(s, "%3d", fh);
         } else
         {                      /* Celsius */
            if (showtemp <= -10)
               strcpy(s, "__._");
            else if (showtemp >= 100)
               strcpy(s, "^^.^");
            else
               sprintf(s, "%4.1f", showtemp);
         }
         gfx_text(5, s);
         gfx_text(1, "o");
         gfx_pos(gfx_x(), gfx_y(), GFX_T | GFX_L | GFX_V);
         gfx_text(2, f ? "F" : "C");
         if (!num_owb && !scd41)
            gfx_text(2, "~");
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
      gfx_unlock();
   }
}

// CO2 + other sensors all
/* Copyright(c) 2019-21 Adrian Kennard, Andrews & Arnold Limited, see LICENSE file(GPL) */
const char TAG[] = "Env";

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

// Note a heat target of 0 is considered "off", so if actually targeting 0, use 1 or -1

#define settings	\
	u32(reporting,300)	\
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
<<<<<<< HEAD
	s8(gfxdin,)	\
	s8(gfxclk,)	\
	s8(gfxcs,)	\
	s8(gfxdc,)	\
	s8(gfxrst,)	\
	u8(gfxcontrast,)	\
=======
	u8(gfxmosi,)	\
	u8(gfxsck,)	\
	u8(gfxcs,)	\
	u8(gfxdc,)	\
	u8(gfxrst,)	\
	u8(gfxflip,)	\
	u8(gfxcontrast,255)	\
>>>>>>> 1e5f1e65ae1198254cb8042f7ae3b9c6d782c953
	u32(gfxmsgtime,30)	\
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
	s32(heatratemC,0)\
	s32a(temphourmC,24)\
	u8(heatswitch,30)	\
	u32(heatresend,600)	\
	s8(heatgpio,-1)	\
	s32(heatdaymC,0)	\
	s32(heatnightmC,0)	\
	s32(heatminmC,0)	\
	u16(hhmmnight,0)	\
	u16(hhmmday,0)		\
	b(nologo)	\
	b(notime)	\

#define u32(n,d)	uint32_t n;
#define u16(n,d)	uint16_t n;
#define s32(n,d)	int32_t n;
#define s32a(n,q)	int32_t n[q];
#define s8(n,d)	int8_t n;
#define u8(n,d)	uint8_t n;
#define b(n) uint8_t n;
#define s(n) char * n;
settings
#undef u32
#undef u16
#undef s32
#undef s32a
#undef s8
#undef u8
#undef b
#undef s
#define	NOTSET	-10000.0
static uint8_t scd41 = 0;
static uint8_t logo[LOGOW * LOGOH / 2];
static float lastco2 = NOTSET;
static float lastrh = NOTSET;
static float lasttemp = NOTSET;
static float lastotemp = NOTSET;
static float thisco2 = NOTSET;
static float thistemp = NOTSET;
static float thisrh = NOTSET;
static int8_t co2port = -1;
static int8_t num_owb = 0;
static volatile uint16_t do_co2_cmd = 0;
static volatile int do_co2_value = -1;
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

static const char *co2_setting(uint16_t cmd, int val);

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
         if (v->value > NOTSET)
            add(v->tag, v->value, v->places);
      if (heatmax >= 0)
         jo_bool(j, "heat", heatmax);
      if (fanmax >= 0)
         jo_bool(j, "fan", fanmax);
      fanmax = fanlast;
      heatmax = heatlast;
      revk_state("data", &j);
   }
   reportlast = now;
   reportchange = 0;
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
   if (last > NOTSET)
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
   if (this > NOTSET)
      this = roundf(this / mag) * mag;  // Rounding
   if (last > NOTSET && this != last && !reportchange)
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
      fanlast = -1;
      heatlast = -1;
      sendall();
      return "";
   }
   if (!strcmp(suffix, "message"))
   {
      jo_strncpy(j, gfx_msg, sizeof(gfx_msg));
      if (gfxmsgtime)
         gfx_msg_time = (esp_timer_get_time() / 1000000) + gfxmsgtime;
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

static i2c_cmd_handle_t co2_cmd(uint16_t c)
{
   i2c_cmd_handle_t i = i2c_cmd_link_create();
   i2c_master_start(i);
   i2c_master_write_byte(i, (co2address << 1), ACK_CHECK_EN);
   i2c_master_write_byte(i, c >> 8, ACK_CHECK_EN);
   i2c_master_write_byte(i, c, ACK_CHECK_EN);
   return i;
}

static void co2_add(i2c_cmd_handle_t i, uint16_t v)
{
   i2c_master_write_byte(i, v >> 8, true);
   i2c_master_write_byte(i, v, true);
   i2c_master_write_byte(i, co2_crc(v >> 8, v), true);
}

static esp_err_t co2_stop(void)
{
   ESP_LOGI(TAG, "CO2 stop");
   i2c_cmd_handle_t i = co2_cmd(0x3f86);        // Stop measurement (SCD41)
   i2c_master_stop(i);
   esp_err_t err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete(i);
   if (!err)
      usleep(500000);           // Takes time
   return err;
}

static esp_err_t co2_start(void)
{
   ESP_LOGI(TAG, "CO2 start");
   i2c_cmd_handle_t i = co2_cmd(0x21b1);        // Start measurement (SCD41)
   i2c_master_stop(i);
   esp_err_t err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete(i);
   return err;
}

static const char *co2_setting(uint16_t cmd, int val)
{
   do_co2_cmd = 0;
   do_co2_value = val;
   do_co2_cmd = cmd;
   return "";
}

void co2_task(void *p)
{
   p = p;
   ESP_LOGI(TAG, "CO2 start");
   int try = 10;
   esp_err_t err = 0;
   i2c_cmd_handle_t i;
   while (try--)
   {
      if (scd41)
      {
         err = co2_stop();
         if (!err)
         {
            i = co2_cmd(0x3646);        // Re init
            i2c_master_stop(i);
            err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(i);
            if (!err)
               usleep(20000);
         }
         if (!err)
         {
            i = co2_cmd(0x3682);        // Get serial number
            i2c_master_stop(i);
            err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(i);
         }
         if (!err)
         {                      // Read
            uint8_t buf[9];
            i = i2c_cmd_link_create();
            i2c_master_start(i);
            i2c_master_write_byte(i, (co2address << 1) + 1, ACK_CHECK_EN);
            i2c_master_read(i, buf, sizeof(buf) - 1, ACK_VAL);
            i2c_master_read_byte(i, buf + sizeof(buf) - 1, NACK_VAL);
            i2c_master_stop(i);
            err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(i);
            if (!err)
            {                   // OK
               if (co2_crc(buf[0], buf[1]) == buf[2] && co2_crc(buf[3], buf[4]) == buf[5] && co2_crc(buf[6], buf[7]) == buf[8])
               {
                  ESP_LOGI(TAG, "CO2 serial %02X%02X%02X%02X%02X%02X", buf[0], buf[1], buf[3], buf[4], buf[6], buf[7]);
                  jo_t j = jo_object_alloc();
                  jo_stringf(j, "serial", "%02X%02X%02X%02X%02X%02X", buf[0], buf[1], buf[3], buf[4], buf[6], buf[7]);
                  revk_info("CO2", &j);
                  break;
               }
            }

         }
      } else
      {                         // Just try starting measurement
         i = co2_cmd(0x0010);   // Start measurement
         co2_add(i, 0);         /* 0 = unknown */
         i2c_master_stop(i);
         err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
         i2c_cmd_link_delete(i);
         if (!err)
            break;
      }
      ESP_LOGI(TAG, "CO2 retry");
      sleep(1);
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
      {                         // Ready status
         i = co2_cmd(scd41 ? 0xe4b8 : 0x0202);
         /* Get ready state */
         i2c_master_stop(i);
         err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
         i2c_cmd_link_delete(i);
         if (err)
         {
            ESP_LOGI(TAG, "Tx GetReady %s", esp_err_to_name(err));
            continue;
         }
         {
            uint8_t buf[3];
            i = i2c_cmd_link_create();
            i2c_master_start(i);
            i2c_master_write_byte(i, (co2address << 1) + 1, ACK_CHECK_EN);
            i2c_master_read(i, buf, sizeof(buf) - 1, ACK_VAL);
            i2c_master_read_byte(i, buf + sizeof(buf) - 1, NACK_VAL);
            i2c_master_stop(i);
            err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(i);
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
               co2_start();     // Undocumented but top bit 8 if not running
            if (!scd41 && (buf[0] << 8) + buf[1] != 1)
               continue;        // Not ready (1 means ready)
            if (scd41 && !(buf[0] & 7) && !buf[1])
               continue;        // Not ready (least 11 bits 0 means not ready)
         }
      }
      if (do_co2_cmd)
      {                         // Do a command from mqtt
         if (scd41)
            co2_stop();
         ESP_LOGI(TAG, "CO2 cmd %04X", do_co2_cmd);
         int val = do_co2_value;
         i2c_cmd_handle_t i = co2_cmd(do_co2_cmd);
         do_co2_cmd = 0;
         do_co2_value = -1;
         if (val >= 0)
            co2_add(i, val);
         i2c_master_stop(i);
         err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
         i2c_cmd_link_delete(i);
         if (err)
            ESP_LOGI(TAG, "CMD failed %s", esp_err_to_name(err));
         continue;
      }
      // Data
      if (scd41)
      {
         i = co2_cmd(0xec05);   // read data
         i2c_master_stop(i);
         err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
         i2c_cmd_link_delete(i);
         if (err)
         {
            ESP_LOGI(TAG, "Tx GetData %s", esp_err_to_name(err));
            continue;
         }
         uint8_t buf[9];
         i = i2c_cmd_link_create();
         i2c_master_start(i);
         i2c_master_write_byte(i, (co2address << 1) + 1, ACK_CHECK_EN);
         i2c_master_read(i, buf, sizeof(buf) - 1, ACK_VAL);
         i2c_master_read_byte(i, buf + sizeof(buf) - 1, NACK_VAL);
         i2c_master_stop(i);
         err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
         i2c_cmd_link_delete(i);
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
         thisco2 = (float) ((buf[0] << 8) + buf[1]);
         float t = -45.0 + 175.0 * (float) ((buf[3] << 8) + buf[4]) / 65536.0;
         thisrh = 100.0 * (float) ((buf[6] << 8) + buf[7]) / 65536.0;
         if (thisco2)
            lastco2 = report("co2", lastco2, thisco2, co2places);
         if (thisrh)
            lastrh = report("rh", lastrh, thisrh, rhplaces);
         if (!num_owb)
            lasttemp = report("temp", lasttemp, thistemp = t, tempplaces);      // Treat as temp not itemp as we trust the SCD41 to be sane
      } else
      {                         // Wait for data to be ready
         i = co2_cmd(0x0300);
         /* Read data */
         i2c_master_stop(i);
         err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
         i2c_cmd_link_delete(i);
         if (err)
         {
            ESP_LOGI(TAG, "Tx GetData %s", esp_err_to_name(err));
            continue;
         }
         {
            uint8_t buf[18];
            i = i2c_cmd_link_create();
            i2c_master_start(i);
            i2c_master_write_byte(i, (co2address << 1) + 1, ACK_CHECK_EN);
            i2c_master_read(i, buf, sizeof(buf) - 1, ACK_VAL);
            i2c_master_read_byte(i, buf + sizeof(buf) - 1, NACK_VAL);
            i2c_master_stop(i);
            err = i2c_master_cmd_begin(co2port, i, 10 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(i);
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
            {
               /* Sanity check */
               if (thisco2 < 0)
                  thisco2 = co2;        /* First */
               else
                  thisco2 = (thisco2 * co2damp + co2) / (co2damp + 1);
            }
            if (rh > 0)
            {
               if (thisrh < 0)
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
#define s32(n,d) revk_register(#n,0,sizeof(n),&n,#d,SETTING_SIGNED|SETTING_LIVE);
#define s32a(n,q) revk_register(#n,q,sizeof(*n),&n,NULL,SETTING_SIGNED|SETTING_LIVE);
#define s8(n,d) revk_register(#n,0,sizeof(n),&n,#d,SETTING_SIGNED);
#define u8(n,d) revk_register(#n,0,sizeof(n),&n,#d,0);
#define s(n) revk_register(#n,0,0,&n,NULL,0);
   settings
#undef u32
#undef u16
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
   gfx_set_contrast(gfxcontrast);
   gfx_colour('B');
   gfx_box(CONFIG_GFX_WIDTH, CONFIG_GFX_HEIGHT, 255);
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
   float showco2 = NOTSET;
   float showtemp = NOTSET;
   float showrh = NOTSET;
   void reset(void) {           /* re display all */
      gfx_clear(0);
      showlogo = 1;
      showtime = 0;
      showco2 = NOTSET;
      showtemp = NOTSET;
      showrh = NOTSET;
   };
   while (1)
   {
      usleep(100000LL - (esp_timer_get_time() % 100000LL));     /* wait a bit */
      time_t now = time(0);
      struct tm t;
      localtime_r(&now, &t);
      int sec = t.tm_min * 60 + t.tm_sec;
      uint32_t hhmm = t.tm_hour * 100 + t.tm_min;
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
      // References
      // Temp should either use a day/night temp, or a temphourmC (setting temp on the hour for 00:00 to 23:00). Zero meaning not set
      // heatratemC allows for getting to some future temphourmC. heatminmC applies anyway
      int32_t temp_target = (gfx_dark ? heatnightmC : heatdaymC);
      if (!temp_target)
      {
         if (temphourmC[t.tm_hour] && temphourmC[(t.tm_hour + 1) % 24])
            temp_target = (temphourmC[t.tm_hour] * (3600 - sec) + temphourmC[(t.tm_hour + 1) % 24] * sec) / 3600;
      }
      if (heatratemC)
      {                         // Allow time to get to a target temperature if any hour temps are set
         int32_t min;
         for (int h = 1; h < 23; h++)
            if (temphourmC[(t.tm_hour + h) % 24] && (min = temphourmC[(t.tm_hour + h) % 24] - heatratemC * (h * 3600 - sec) / 3600) > temp_target)
               temp_target = min;
      }
      if (temp_target < heatminmC)
         temp_target = heatminmC;
      if (temp_target)
         report("temp-target", NOTSET, ((float) temp_target) / 1000.0, 3);
      else
         report("temp-target", NOTSET, NOTSET, 3);      // No target
      // Report
      if (up > 60 || sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED)
         reportall(now);        // Don't report right away if clock may be duff
      static uint32_t fanwait = 0;
      if (fanwait < up && (fanco2on || fanco2off || fanrhon || fanrhoff))
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
      if (heatwait < up && (heatnightmC || heatdaymC || heatratemC || temp_target || heatgpio >= 0 || heaton || heatoff))
      {                         /* Heat control */
         if (temp_target || heatlast == 1)
         {                      /* We have a reference temp to work with or we left on */
            if (heatresend && heattime < up)
               heatlast = -1;
            const char *heat = NULL;
            int32_t thismC = thistemp * 1000;
            if (!temp_target || thismC > temp_target)
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
      /* Display */
      if (gfx_msg_time)
      {                         /* display fixed message */
         if (gfx_msg_time < (esp_timer_get_time() / 1000000))
         {                      /* time up */
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
      char s[30];
      if (gfx_dark)
      {                         /* Night mode, just time */
         gfx_colour('r');
         reset();
         if (!notime)
         {
            strftime(s, sizeof(s), "%T", &t);
            int y = CONFIG_GFX_HEIGHT - 1 - t.tm_sec * 2;
            if (t.tm_min & 1)
               y = 6 + t.tm_sec * 2;
            int x = t.tm_hour + t.tm_min;
            if (t.tm_hour & 1)
               x = t.tm_hour + 60 - t.tm_min;
            gfx_pos(x, y, GFX_B | GFX_L);
            gfx_text(1, s);
         }
         gfx_unlock();
         continue;
      }
      if (showlogo)
      {
         showlogo = 0;
         gfx_clear(0);
         if (!nologo)
         {
            gfx_pos(CONFIG_GFX_WIDTH - LOGOW, CONFIG_GFX_WIDTH - 12, GFX_B | GFX_L);
            gfx_icon16(LOGOW, LOGOH, logo);
         }
      }
      if (now != showtime && !notime)
      {
         showtime = now;
         if (t.tm_year > 100)
         {
            strftime(s, sizeof(s), "%F\004%T %Z", &t);
            gfx_pos(0, CONFIG_GFX_HEIGHT - 1, GFX_B | GFX_L);
            gfx_text(1, s);
         }
      }
      int y = 0,
          space = (CONFIG_GFX_HEIGHT - 28 - 35 - 21 - 9) / 3;
      if (thisco2 != showco2)
      {
         showco2 = thisco2;
         gfx_colour(showco2 < 200 ? 'K' : showco2 > (fanco2on ? : 1000) ? 'R' : showco2 > (fanco2off ? : 750) ? 'Y' : 'G');
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
            gfx_pos(CONFIG_GFX_WIDTH - LOGOW * 2 - 2, CONFIG_GFX_HEIGHT - 12, GFX_B | GFX_L);
            gfx_icon16(LOGOW, LOGOH, fanlast ? fan : NULL);
         }
      }
      y += 28 + space;
      if (thistemp != showtemp)
      {
         showtemp = thistemp;
         int32_t reftemp = temp_target ? : 21000;
         int32_t thismC = thistemp * 1000;
         if (showtemp == NOTSET)
            gfx_colour('K');
         else if (reftemp)
            gfx_colour(thismC > reftemp + 500 ? 'R' : thismC > reftemp - 500 ? 'G' : 'B');
         else
            gfx_colour('W');
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
      if (thisrh != showrh)
      {
         showrh = thisrh;
         gfx_colour(showrh < 0 ? 'K' : 'C');
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

/* CO2 + other sensors all */
/* Copyright(c) 2019-21 Adrian Kennard, Andrews & Arnold Limited, see LICENSE file(GPL) */
const char TAG[] = "Env";

/* #define       DEBUGTEMP */

#define LOGOW   32
#define LOGOH   32

#include "revk.h"
#include <driver/i2c.h>
#include <hal/spi_types.h>
#include <math.h>
#include <esp_sntp.h>
#include "esp_http_server.h"
#include <onewire_bus.h>
#include <ds18b20.h>
#include "gfx.h"
#include "led_strip.h"
#include "icons.h"
#include "bleenv.h"
#include "halib.h"

#define ACK_CHECK_EN 0x1        /* !< I2C master will check ack from slave */
#define ACK_CHECK_DIS 0x0       /* !< I2C master will not check ack from slave */
#define ACK_VAL 0x0             /* !< I2C ack value */
#define NACK_VAL 0x1            /* !< I2C nack value */
#define	MAX_OWB	8
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)

/* temp targets of 0 mean off, so use 1 or - 1 if really targeting 0 */

static uint8_t ha_send = 0;
static uint8_t scd41 = 0;
static uint32_t scd41_settled = 1;      /* uptime when started measurements */

#ifdef  CONFIG_REVK_LED_STRIP
led_strip_handle_t strip = NULL;
#endif

static uint8_t airconpower = 0;
static uint8_t airconslave = 0;
static uint8_t airconantifreeze = 0;
static char airconmode = 0;
static uint32_t airconlast = 0;

#ifndef CONFIG_GFX_BUILD_SUFFIX_GFXNONE
static char lasticon = 0;
#endif
static float lastco2 = NAN;
static float lastals = NAN;
static float lastalsr = NAN;
static float lastalsg = NAN;
static float lastalsb = NAN;
static float lastrh = NAN;
static float lasttemp = NAN;
static float lastotemp = NAN;
static float thisco2 = NAN;
static float thistemp = NAN;
#ifdef	ELA
static bleenv_t *bletemp = NULL;
#endif
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
static uint32_t sht_serial = 0;
static int8_t i2cport = -1;
static volatile uint32_t do_co2 = 0;
static int8_t num_ds18b20 = 0;
static ds18b20_device_handle_t adr_ds18b20[2];
static char co2_found = 0;
static char als_found = 0;      // 1 is VEML3235SL, 2 is VEML6040A3OG
static char sht_found = 0;
static char sendinfo = 0;

static uint32_t fantime = 0;
static uint32_t heattime = 0;
static int8_t fanlast = -1;
static int8_t heatlast = -1;

static volatile uint8_t gfx_update = 0;
static volatile uint8_t gfx_changed = 1;
static volatile uint8_t gfx_dark = 0;
static volatile uint32_t gfx_msg_time = 0;      /* message timer */
static volatile uint32_t menu_time = 0; /* menu timer */
static volatile uint32_t countdown = 0; // Countdown clock
static char gfx_msg[100];       /* message text */

static httpd_handle_t webserver = NULL;

static const char *co2_setting (uint16_t cmd, uint16_t val);

typedef struct value_s value_t;
struct value_s
{
   value_t *next;
   const char *tag;
   int8_t places;
   float value;
};
value_t *values = NULL;
time_t reportlast = 0,
   reportchange = 0;

void
revk_state_extra (jo_t j)
{                               /* Do reporting of values */
   time_t now = time (0);
   value_t *v;
   void add (const char *tag, float value, int8_t places)
   {
      if (places <= 0)
         jo_litf (j, tag, "%d", (int) value);
      else
         jo_litf (j, tag, "%.*f", places, value);
   }
   for (v = values; v; v = v->next)
      if (!isnan (v->value))
         add (v->tag, v->value, v->places);
   if (heatlast >= 0)
      jo_bool (j, "heat", heatlast);
   if (fanlast >= 0)
      jo_bool (j, "fan", fanlast);
   if (
#ifndef  CONFIG_GFX_BUILD_SUFFIX_GFXNONE
         gfxmosi.set ||
#endif
         als_found)
      jo_bool (j, "dark", gfx_dark);
   if (!isnan (temptargetmin) && !isnan (temptargetmax))
   {
      if (temptargetmin == temptargetmax)
         jo_litf (j, "temp-target", "%.3f", temptargetmin);
      else
      {
         jo_array (j, "temp-target");
         jo_litf (j, NULL, "%.3f", temptargetmin);
         jo_litf (j, NULL, "%.3f", temptargetmax);
         jo_close (j);
      }
   }
#ifdef	ELA
   if (bletemp && !bletemp->missing)
   {
      jo_string (j, "source", bletemp->name);
      if (bletemp->bat)
         jo_int (j, "bat", bletemp->bat);
      if (bletemp->volt)
         jo_litf (j, "voltage", "%d.%03d", bletemp->volt / 1000, bletemp->volt % 1000);
   }
#endif
#ifndef  CONFIG_GFX_BUILD_SUFFIX_GFXNONE
   if (lasticon)
      jo_stringf (j, "icon", "%c", lasticon);
#endif
   if (*aircon && !isnan (lasttemp))
   {                            /* Aircon control */
      static float last = NAN;
      if (isnan (last) || last != lasttemp || !reporting || now / reporting != reportlast / reporting)
      {
         char topic[100];
         snprintf (topic, sizeof (topic), "command/%s/control", aircon);
         jo_t j = jo_object_alloc ();
         if (tempplaces <= 0)
            jo_litf (j, "env", "%d", (int) lasttemp);
         else
            jo_litf (j, "env", "%.*f", tempplaces, lasttemp);
         if (!heatmonitor)
         {
            if (!isnan (temptargetmin) && temptargetmin == temptargetmax)
               jo_litf (j, "target", "%.3f", temptargetmin);
            else
            {
               jo_array (j, "target");
               jo_litf (j, NULL, "%.3f", temptargetmin);
               jo_litf (j, NULL, "%.3f", temptargetmax);
               jo_close (j);
            }
         }
         revk_mqtt_send_clients (NULL, 0, topic, &j, 1);
         last = lasttemp;
      }
   }
}

static float
report (const char *tag, float last, float this, int places)
{
   value_t *v;
   for (v = values; v && v->tag != tag; v = v->next);
   if (!v)
   {
      v = mallocspi (sizeof (*v));
      v->tag = tag;
      v->places = places;
      v->next = values;
      values = v;
   }
   float mag = powf (10.0, -places);
   if (!isnan (last))
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
   if (!isnan (this))
      this = roundf (this / mag) * mag; /* Rounding */
   if (!isnan (last) && this != last && !reportchange)
      reportchange = time (0);
   v->value = this;
   return this;
}

static void
sendall (void)
{
   reportlast = 0;
   heattime = 0;
   fantime = 0;
}

static void
send_ha_config (void)
{
   ha_send = 0;
   if (ds18b20.set || i2cport >= 0)
    ha_config_sensor ("temp", name: "Temp", type: "temperature", unit:"°C");
   if (i2cport >= 0)
    ha_config_sensor ("rh", name: "R/H", type: "humidity", unit:"%");
   if (i2cport >= 0)
    ha_config_sensor ("co2", name: "CO₂", type: "carbon_dioxide", unit:"ppm");
}

const char *
app_callback (int client, const char *prefix, const char *target, const char *suffix, jo_t j)
{
   if (*aircon && prefix && !strcmp (prefix, "Faikin") && target && airconlast)
   {
      airconlast = uptime ();
      return "";
   }
   if (*aircon && prefix && !strcmp (prefix, "state") && target && !strcmp (target, aircon) && jo_here (j) == JO_OBJECT)
   {                            // Aircon state
      airconlast = uptime ();
      jo_type_t t = jo_next (j);        // Start object
      while (t == JO_TAG)
      {
         char tag[10] = "",
            val[10];
         jo_strncpy (j, tag, sizeof (tag));
         t = jo_next (j);
         if (!strcmp (tag, "mode"))
         {
            jo_strncpy (j, val, sizeof (val));
            airconmode = toupper ((int) (uint8_t) * val);
         } else if (!strcmp (tag, "power"))
            airconpower = (t == JO_TRUE);
         else if (!strcmp (tag, "slave"))
            airconslave = (t == JO_TRUE);
         else if (!strcmp (tag, "antifreeze"))
            airconantifreeze = (t == JO_TRUE);
         t = jo_skip (j);
      }
      return "";
   }
   if (client || !prefix || target || strcmp (prefix, "command") || !suffix)
      return NULL;
   if (!strcmp (suffix, "override"))
   {                            // Expects array of min/max
      if (jo_here (j) == JO_ARRAY)
      {
         if (jo_next (j) == JO_NUMBER)
            tempoverridemin = jo_read_float (j);
         else
            tempoverridemin = NAN;
         if (jo_next (j) == JO_NUMBER)
            tempoverridemax = jo_read_float (j);
         else
            tempoverridemax = tempoverridemin;
      } else
         tempoverridemin = tempoverridemax = NAN;       // Cancelled
      return "";
   }
   if (!strcmp (suffix, "send"))
   {
      sendall ();
      return "";
   }
   if (!strcmp (suffix, "connect"))
   {
      sendinfo = 1;
      fanlast = -1;
      heatlast = -1;
      sendall ();
      if (*aircon)
      {
         char temp[100];
         snprintf (temp, sizeof (temp), "state/%s/status", aircon);
         lwmqtt_subscribe (revk_mqtt (0), temp);
         snprintf (temp, sizeof (temp), "Faikin/%s", aircon);
         lwmqtt_subscribe (revk_mqtt (0), temp);
      }
      if (ha)
         ha_send = 1;
      return "";
   }
   if (!strcmp (suffix, "message"))
   {
      jo_strncpy (j, gfx_msg, sizeof (gfx_msg));
      if (msgtime)
         gfx_msg_time = uptime () + msgtime;
      return "";
   }
   if (!strcmp (suffix, "night"))
      gfx_dark = (jo_here (j) == JO_FALSE ? 0 : 1);     // Sending false means day
   if (!strcmp (suffix, "day"))
      gfx_dark = 0;
   if (!strcmp (suffix, "night") || !strcmp (suffix, "day") || !strcmp (suffix, "countdown"))
   {                            // Can set countdown timer with night/day/countdown
      if (jo_here (j) == JO_NUMBER)
         countdown = uptime () + jo_read_int (j);
      else
         countdown = 0;
      return "";
   }
   if (!strcmp (suffix, "contrast"))
   {
      gfx_set_contrast (jo_read_int (j));
      return "";                /* OK */
   }
   if (!strcmp (suffix, "co2factory") && scd41)
      return co2_setting (0x3632, -1);
   if (!strcmp (suffix, "co2reload") && scd41)
      return co2_setting (0x3646, -1);
   if (!strcmp (suffix, "co2autocal"))
      return co2_setting (scd41 ? 0x2416 : 0x5306, 1);
   if (!strcmp (suffix, "co2nocal"))
      return co2_setting (scd41 ? 0x2416 : 0x5306, 0);
   if (!strcmp (suffix, "co2cal"))
      return co2_setting (scd41 ? 0x362f : 0x5204, jo_read_int (j));    /* ppm */
   if (!strcmp (suffix, "co2tempoffset"))
      return co2_setting (scd41 ? 0x241d : 0x5403, scd_tempoffset = jo_read_int (j));   /* Use T * 65536 / 175 */
   if (!strcmp (suffix, "co2tempcal"))
   {                            // Set the current temp in C
      if (isnan (lasttemp))
         return "No temp now";
      return co2_setting (scd41 ? 0x241d : 0x5403, scd_tempoffset = (scd_tempoffset - (jo_read_float (j) - lasttemp) * 65536.0 / 175.0));       // Oddly the offset seems to be negative
   }
   if (!strcmp (suffix, "co2alt"))
      return co2_setting (scd41 ? 0x2427 : 0x5102, jo_read_int (j));    /* m */
   return NULL;
}

static uint8_t
co2_crc (uint8_t b1, uint8_t b2)
{
   uint8_t crc = 0xFF;
   void b (uint8_t v)
   {
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
   b (b1);
   b (b2);
   return crc;
}

static i2c_cmd_handle_t
co2_setup (uint16_t c)
{                               /* Set up command */
   /* ESP_LOGE(TAG, "CO2 cmd %04X", c); */
   i2c_cmd_handle_t i = i2c_cmd_link_create ();
   i2c_master_start (i);
   i2c_master_write_byte (i, (co2address << 1), ACK_CHECK_EN);
   i2c_master_write_byte (i, c >> 8, ACK_CHECK_EN);
   i2c_master_write_byte (i, c, ACK_CHECK_EN);
   return i;
}

static void
co2_add (i2c_cmd_handle_t i, uint16_t v)
{                               /* Add word to command */
   /* ESP_LOGE(TAG, "CO2 val %04X", v); */
   i2c_master_write_byte (i, v >> 8, ACK_CHECK_EN);
   i2c_master_write_byte (i, v, ACK_CHECK_EN);
   i2c_master_write_byte (i, co2_crc (v >> 8, v), ACK_CHECK_EN);
}

static esp_err_t
co2_done (i2c_cmd_handle_t * i)
{                               /* Finish command */
   i2c_master_stop (*i);
   esp_err_t err = i2c_master_cmd_begin (i2cport, *i, 1000 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete (*i);
   *i = NULL;
   return err;
}

static esp_err_t
co2_command (uint16_t c)
{
   i2c_cmd_handle_t i = co2_setup (c);
   return co2_done (&i);
}

static esp_err_t
co2_read (int len, uint8_t * buf)
{
   i2c_cmd_handle_t i = i2c_cmd_link_create ();
   i2c_master_start (i);
   i2c_master_write_byte (i, (co2address << 1) + 1, ACK_CHECK_EN);
   i2c_master_read (i, buf, len - 1, ACK_VAL);
   i2c_master_read_byte (i, buf + len - 1, NACK_VAL);
   return co2_done (&i);
}

static esp_err_t
co2_scd41_stop_measure (void)
{
   esp_err_t err = co2_command (0x3f86);        /* Stop measurement(SCD41) */
   sleep (1);
   return err;
}

static esp_err_t
co2_scd41_start_measure (void)
{
   scd41_settled = uptime () + co2startup;      /* Time for temp to settle */
   return co2_command (0x21b1); /* Start measurement(SCD41) */
}

static const char *
co2_setting (uint16_t cmd, uint16_t val)
{
   do_co2 = (cmd << 16) + val;
   return "";
}

static int
als_read (uint8_t cmd)
{
   uint8_t h = 0,
      l = 0;
   i2c_cmd_handle_t t = i2c_cmd_link_create ();
   i2c_master_start (t);
   i2c_master_write_byte (t, (alsaddress << 1) | I2C_MASTER_WRITE, true);
   i2c_master_write_byte (t, cmd, true);
   i2c_master_start (t);
   i2c_master_write_byte (t, (alsaddress << 1) | I2C_MASTER_READ, true);
   i2c_master_read_byte (t, &l, I2C_MASTER_ACK);
   i2c_master_read_byte (t, &h, I2C_MASTER_LAST_NACK);
   i2c_master_stop (t);
   esp_err_t err = i2c_master_cmd_begin (i2cport, t, 10 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete (t);
   if (err)
      return -1;
   return (h << 8) + l;
}

static void
als_write (uint8_t cmd, uint16_t val)
{
   i2c_cmd_handle_t t = i2c_cmd_link_create ();
   i2c_master_start (t);
   i2c_master_write_byte (t, (alsaddress << 1) | I2C_MASTER_WRITE, true);
   i2c_master_write_byte (t, cmd, true);
   i2c_master_write_byte (t, val & 0xFF, true);
   i2c_master_write_byte (t, val >> 8, true);
   i2c_master_stop (t);
   i2c_master_cmd_begin (i2cport, t, 10 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete (t);
}

static uint32_t
sht_read (uint8_t cmd)
{
   uint8_t Th = 0,
      Tl = 0,
      Tc = 0,
      Hh = 0,
      Hl = 0,
      Hc = 0;
   i2c_cmd_handle_t t = i2c_cmd_link_create ();
   i2c_master_start (t);
   i2c_master_write_byte (t, (shtaddress << 1) | I2C_MASTER_WRITE, true);
   i2c_master_write_byte (t, cmd, true);
   i2c_master_stop (t);
   esp_err_t err = i2c_master_cmd_begin (i2cport, t, 10 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete (t);
   if (err)
   {
      ESP_LOGE (TAG, "SHT cmd %02X: %s", cmd, esp_err_to_name (err));
      return 0;
   }
   if (cmd == 0x94)
      return 0;                 // soft restart does not have data
   usleep (100000);
   t = i2c_cmd_link_create ();
   i2c_master_start (t);
   i2c_master_write_byte (t, (shtaddress << 1) | I2C_MASTER_READ, true);
   i2c_master_read_byte (t, &Th, I2C_MASTER_ACK);
   i2c_master_read_byte (t, &Tl, I2C_MASTER_ACK);
   i2c_master_read_byte (t, &Tc, I2C_MASTER_ACK);
   i2c_master_read_byte (t, &Hh, I2C_MASTER_ACK);
   i2c_master_read_byte (t, &Hl, I2C_MASTER_ACK);
   i2c_master_read_byte (t, &Hc, I2C_MASTER_LAST_NACK);
   i2c_master_stop (t);
   err = i2c_master_cmd_begin (i2cport, t, 10 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete (t);
   if (err)
   {
      ESP_LOGE (TAG, "SHT read %02X: %s", cmd, esp_err_to_name (err));
      return 0;
   }
   if (co2_crc (Th, Tl) != Tc)
      Th = Tl = 0;
   if (co2_crc (Hh, Hl) != Hc)
      Hh = Hl = 0;
   return (Th << 24) + (Tl << 16) + (Hh << 8) + Hl;
}

void
i2c_task (void *p)
{
   p = p;
   ESP_LOGI (TAG, "I2C start");
   int try = 10;
   esp_err_t err = 0;
   if (co2address)
   {                            // CO2
      while (try--)
      {
         sleep (1);
         if (scd41)
         {
            err = co2_scd41_stop_measure ();
            if (!err)
               err = co2_command (0x3646);      /* Reinit */
            if (!err)
               break;
         } else
         {                      /* Just try starting measurement */
            err = co2_command (0x0010); /* Start measurement */
            if (!err)
               break;
         }
         ESP_LOGI (TAG, "CO2 retry");
      }
      if (err)
      {                         /* failed */
         ESP_LOGE (TAG, "No CO2 %02X", co2address);
         jo_t j = jo_object_alloc ();
         jo_string (j, "error", "No CO2 sensor");
         jo_string (j, "device", scd41 ? "SCD41" : "SCD30");
         jo_int (j, "sda", sda.num);
         jo_int (j, "scl", scl.num);
         jo_int (j, "adr", co2address);
         jo_int (j, "code", err);
         jo_string (j, "description", esp_err_to_name (err));
         revk_error ("CO2", &j);
      } else
      {
         co2_found = 1;
         if (scd41)
            co2_setting (0x3682, 0);    // Get serial, etc
      }
   }
   if (alsaddress)
   {
      int id = als_read (0x09);
      if ((id & 0xFF) == 0x35)
      {
         als_found = 1;
         als_write (0x00, 0x0040);
      } else if (id >= 0)
      {                         // It read, assume VELM6040
         als_found = 2;
         als_write (0x00, 0x0040);      // IT=4 TRIG=0 AF=0 SD=0
      } else
      {
         gfx_dark = 0;
         ESP_LOGE (TAG, "No ALS %02X (%04X)", alsaddress, id);
         jo_t j = jo_object_alloc ();
         jo_string (j, "error", "No ALS");
         jo_int (j, "sda", sda.num);
         jo_int (j, "scl", scl.num);
         jo_int (j, "adr", alsaddress);
         if (id >= 0)
            jo_stringf (j, "09", "%04X", id);
         if (id)
            jo_int (j, "id", id);
         revk_error ("ALS", &j);
      }
   }
   if (shtaddress)
   {
      sht_serial = sht_read (0x89);
      if (!sht_serial)
      {
         sleep (1);
         sht_serial = sht_read (0x89);
      }
      if (!sht_serial)
      {
         ESP_LOGE (TAG, "No SHT %02X", shtaddress);
         jo_t j = jo_object_alloc ();
         jo_string (j, "error", "No SHT");
         jo_int (j, "sda", sda.num);
         jo_int (j, "scl", scl.num);
         jo_int (j, "sht", shtaddress);
         revk_error ("SHT", &j);
      } else
         sht_found = 1;
   }
   if (!co2_found && !als_found && !sht_found)
   {                            // No I2C to do
      vTaskDelete (NULL);
      return;
   }
   /* Get measurements */
   while (1)
   {
      sleep (1);
      if (co2_found)
      {
         if (do_co2)
         {                      /* Do a command from mqtt */
            uint16_t cmd = (do_co2 >> 16);
            uint16_t val = do_co2;
            do_co2 = 0;
            if (scd41)
               co2_scd41_stop_measure ();
            i2c_cmd_handle_t i = co2_setup (cmd);
            if (cmd != 0x3632 && cmd != 0x3615 && cmd != 0x3646 && cmd != 0x3682)
               co2_add (i, val);
            err = co2_done (&i);
            if (err)
               ESP_LOGI (TAG, "CMD failed %s", esp_err_to_name (err));
            if (cmd == 0x362f || cmd == 0x3615 || cmd == 0x3632)
               sleep (1);
            else if (cmd == 0x3639)
               sleep (10);
            if ((cmd == 0x241d || cmd == 0x2416 || cmd == 0x2427) && !err)
            {
               err = co2_command (0x3615);      /* Persist */
               sleep (2);
               if (!err)
                  err = co2_command (0x3615);   /* Re init */
               sleep (5);
               if (!err)
                  err = co2_command (cmd = 0x3682);     /* Serial */
            }
            if (cmd == 0x3682 && !err)
            {                   /* Get serial/etc */
               uint8_t buf[9];
               err = co2_read (9, buf);
               if (!err && co2_crc (buf[0], buf[1]) == buf[2] && co2_crc (buf[3], buf[4]) == buf[5]
                   && co2_crc (buf[6], buf[7]) == buf[8])
                  scd_serial =
                     ((unsigned long long) buf[0] << 40) + ((unsigned long long) buf[1] << 32) +
                     ((unsigned long long) buf[3] << 24) + ((unsigned long long) buf[4] << 16) +
                     ((unsigned long long) buf[6] << 8) + ((unsigned long long) buf[7]);
               if (!err)
                  err = co2_command (0x2318);
               if (!err)
                  err = co2_read (3, buf);
               if (!err && co2_crc (buf[0], buf[1]) == buf[2])
                  scd_tempoffset = (buf[0] << 8) + buf[1];
               if (!err)
                  err = co2_command (0x2322);
               if (!err)
                  err = co2_read (3, buf);
               if (!err && co2_crc (buf[0], buf[1]) == buf[2])
                  scd_altitude = (buf[0] << 8) + buf[1];
               if (!err)
                  err = co2_command (0x2313);
               if (!err)
                  err = co2_read (3, buf);
               if (!err && co2_crc (buf[0], buf[1]) == buf[2])
                  scd_selfcal = (buf[0] << 8) + buf[1];
               sendinfo = 1;
            }
            if (scd41)
               co2_scd41_start_measure ();
            continue;
         }
         {                      /* Ready status */
            err = co2_command (scd41 ? 0xe4b8 : 0x0202);
            if (err)
            {
               ESP_LOGI (TAG, "Tx GetReady %s", esp_err_to_name (err));
               continue;
            }
            {                   /* Read status */
               uint8_t buf[3];
               err = co2_read (sizeof (buf), buf);
               if (err)
               {
                  ESP_LOGI (TAG, "Rx GetReady %s", esp_err_to_name (err));
                  continue;
               }
               if (co2_crc (buf[0], buf[1]) != buf[2])
               {
                  ESP_LOGI (TAG, "Rx GetReady CRC error %02X %02X", co2_crc (buf[0], buf[1]), buf[2]);
                  continue;
               }
               if (scd41 && !(buf[0] & 0x80))
                  co2_scd41_start_measure ();   /* Undocumented but top bit 8 if not running */
               if (!scd41 && (buf[0] << 8) + buf[1] != 1)
                  continue;     /* Not ready(1 means ready) */
               if (scd41 && !(buf[0] & 7) && !buf[1])
                  continue;     /* Not ready(least 11 bits 0 means not ready) */
            }
         }
         /* Data */
         if (scd41)
         {
            err = co2_command (0xec05); /* read data */
            if (err)
            {
               ESP_LOGI (TAG, "Tx GetData %s", esp_err_to_name (err));
               continue;
            }
            uint8_t buf[9];
            err = co2_read (sizeof (buf), buf);
            if (err)
            {
               ESP_LOGI (TAG, "Rx Data %s", esp_err_to_name (err));
               continue;
            }
            if (co2_crc (buf[0], buf[1]) != buf[2] || co2_crc (buf[3], buf[4]) != buf[5] || co2_crc (buf[6], buf[7]) != buf[8])
            {
               ESP_LOGI (TAG, "Rx bad CRC");
               continue;
            }
            float c = (float) ((buf[0] << 8) + buf[1]);
            float t = -45.0 + 175.0 * (float) ((buf[3] << 8) + buf[4]) / 65536.0;
            float r = 100.0 * (float) ((buf[6] << 8) + buf[7]) / 65536.0;
            if (c > 0)
            {
               if (isnan (thisco2))
                  thisco2 = c;  /* First */
               else
                  thisco2 = (thisco2 * co2damp + c) / (co2damp + 1);
               lastco2 = report ("co2", lastco2, thisco2, co2places);
            }
            if (!scd41_settled)
            {                   // Settled, so get temp
               if (r > 0)
               {
                  if (isnan (thisrh))
                     thisrh = r;        /* First */
                  else
                     thisrh = (thisrh * rhdamp + r) / (rhdamp + 1);
                  lastrh = report ("rh", lastrh, thisrh, rhplaces);
               }
               if (!num_ds18b20
#ifdef	ELA
                   && (!bletemp || bletemp->missing)
#endif
                  )
                  lasttemp = report ("temp", lasttemp, thistemp = t, tempplaces);       /* Treat as temp not itemp as we trust the SCD41 to * be sane */
            }
         } else
         {                      /* Wait for data to be ready */
            err = co2_command (0x0300);
            if (err)
            {
               ESP_LOGI (TAG, "Tx GetData %s", esp_err_to_name (err));
               continue;
            }
            {
               uint8_t buf[18];
               err = co2_read (sizeof (buf), buf);
               if (err)
               {
                  ESP_LOGI (TAG, "Rx Data %s", esp_err_to_name (err));
                  continue;
               }
               /* ESP_LOG_BUFFER_HEX_LEVEL(TAG, buf, 18, ESP_LOG_INFO); */
               uint8_t d[4];
               d[3] = buf[0];
               d[2] = buf[1];
               d[1] = buf[3];
               d[0] = buf[4];
               float co2 = *(float *) d;
               if (co2_crc (buf[0], buf[1]) != buf[2] || co2_crc (buf[3], buf[4]) != buf[5])
                  co2 = -1;
               d[3] = buf[6];
               d[2] = buf[7];
               d[1] = buf[9];
               d[0] = buf[10];
               float t = *(float *) d;
               if (co2_crc (buf[6], buf[7]) != buf[8] || co2_crc (buf[9], buf[10]) != buf[11])
                  t = -1000;
               d[3] = buf[12];
               d[2] = buf[13];
               d[1] = buf[15];
               d[0] = buf[16];
               float rh = *(float *) d;
               if (co2_crc (buf[12], buf[13]) != buf[14] || co2_crc (buf[15], buf[16]) != buf[17])
                  rh = -1000;
               if (co2 > 100)
               {                /* Have a reading */
                  if (isnan (thisco2))
                     thisco2 = co2;     /* First */
                  else
                     thisco2 = (thisco2 * co2damp + co2) / (co2damp + 1);
               }
               if (rh > 0)
               {                /* Have a reading */
                  if (isnan (thisrh))
                     thisrh = rh;       /* First */
                  else
                     thisrh = (thisrh * rhdamp + rh) / (rhdamp + 1);
               }
               if (!num_ds18b20 && t >= -1000
#ifdef	ELA
                   && (!bletemp || bletemp->missing)
#endif
                  )
                  lasttemp = report ("itemp", lasttemp, thistemp = t, tempplaces);
               /* Use temp here as no DS18B20 */
               lastco2 = report ("co2", lastco2, thisco2, co2places);
               lastrh = report ("rh", lastrh, thisrh, rhplaces);
            }
         }
      } else if (sht_found)
      {
         uint32_t v = sht_read (0xFD);
         if (v)
         {
            float t = -45.0 + 175 * (float) (v >> 16) / 65535.0 + 0.1 * shtoffset;
            float h = -6.0 + 125 * (float) (v & 65535) / 65535.0;
            lasttemp = report ("temp", lasttemp, thistemp = t, tempplaces);
            lastrh = report ("rh", lastrh, thisrh = h, rhplaces);
            //ESP_LOGI (TAG, "SHT T/H read %08lX T=%.2f H=%.2f", v, t, h);
         }
      }
      if (als_found == 1)
         //lastals = report ("als", lastals, (float) als_read (0x04) * 1117 / 65535, alsplaces);   // VEML3235 IT=4
         lastals = report ("als", lastals, als_read (0x04), alsplaces); // What we seen to see is closer to just lux
      else if (als_found == 2)
      {                         // VEML6040 IT=4
         lastals = report ("als", lastals, (float) als_read (0x0B) * 1031 / 65535, alsplaces);
         lastalsb = report ("b", lastalsb, (float) als_read (0x0A) * 1031 / 65535, alsplaces);
         lastalsg = report ("g", lastalsg, (float) als_read (0x09) * 1031 / 65535, alsplaces);
         lastalsr = report ("r", lastalsr, (float) als_read (0x08) * 1031 / 65535, alsplaces);
      }
   }
}

void
ds18b20_task (void *p)
{
   p = p;
   ESP_LOGI (TAG, "DS18B20 start (%d sensor%s)", num_ds18b20, num_ds18b20 == 1 ? "" : "s");
   onewire_bus_config_t bus_config = { ds18b20.num };
   onewire_bus_rmt_config_t rmt_config = { 20 };
   onewire_bus_handle_t bus_handle = { 0 };
   REVK_ERR_CHECK (onewire_new_bus_rmt (&bus_config, &rmt_config, &bus_handle));
   void init (void)
   {
      onewire_device_iter_handle_t iter = { 0 };
      REVK_ERR_CHECK (onewire_new_device_iter (bus_handle, &iter));
      onewire_device_t dev = { };
      while (!onewire_device_iter_get_next (iter, &dev) && num_ds18b20 < sizeof (adr_ds18b20) / sizeof (*adr_ds18b20))
      {
         ESP_LOGE (TAG, "Found %llX", dev.address);
         ds18b20_config_t config = { };
         REVK_ERR_CHECK (ds18b20_new_device (&dev, &config, &adr_ds18b20[num_ds18b20]));
         REVK_ERR_CHECK (ds18b20_set_resolution (adr_ds18b20[num_ds18b20], DS18B20_RESOLUTION_12B));
         num_ds18b20++;
      }
   }
   init ();
   if (!num_ds18b20)
   {
      usleep (100000);
      init ();
   }
   if (!num_ds18b20)
   {
      jo_t j = jo_object_alloc ();
      jo_string (j, "error", "No DS18B20 devices");
      jo_int (j, "port", ds18b20.num);
      revk_error ("temp", &j);
      ESP_LOGE (TAG, "No DS18B20 port %d", ds18b20.num);
      vTaskDelete (NULL);
      return;
   }
   sendinfo = 1;
   while (1)
   {
      usleep (250000);
#ifdef	ELA
      if (bletemp && !bletemp->missing)
         continue;
#endif
      float c[num_ds18b20];
      for (int i = 0; i < num_ds18b20; ++i)
      {
         REVK_ERR_CHECK (ds18b20_trigger_temperature_conversion (adr_ds18b20[i]));
         REVK_ERR_CHECK (ds18b20_get_temperature (adr_ds18b20[i], &c[i]));
      }
      if (!isnan (c[0]))
      {
#define N 10
         static float last[N] = { 0 }, tot = 0;
         static int p = 0;
         c[0] += ((float) ds18b20offset) / ds18b20offset_scale; // Offset compensation
         // Moving average
         if (++p >= N)
            p = 0;
         tot -= last[p];
         last[p] = c[0];
         tot += last[p];
         thistemp = tot / N;
         lasttemp = report ("temp", lasttemp, thistemp, tempplaces);
#undef N
      }
      if (num_ds18b20 > 1 && !isnan (c[1]))
         lastotemp = report ("otemp", lastotemp, c[1], tempplaces);
   }
}

void
menuinit (void)
{                               /* Common menu stuff */
#ifndef	CONFIG_GFX_BUILD_SUFFIX_GFXNONE
   gfx_set_contrast (gfxlight);
   gfx_clear (0);
#endif
}

void
gfx_temp (float t)
{
   char s[30];                  /* Temp string */
   if (fahrenheit)
   {                            /* Fahrenheit */
      int fh = (t + 40.0) * 1.8 - 40.0;
      if (fh <= -100)
         strcpy (s, "___");
      else if (fh >= 1000)
         strcpy (s, "^^^");
      else
         sprintf (s, "%3d", fh);
   } else
   {                            /* Celsius */
      if (t <= -10)
         strcpy (s, "__._");
      else if (t >= 100)
         strcpy (s, "^^.^");
      else
         sprintf (s, "%4.1f", t);
   }
   gfx_text (0, 5, s);
   gfx_text (0, 1, "o");
   gfx_pos (gfx_x (), gfx_y (), GFX_T | GFX_L | GFX_V);
   gfx_text (0, 2, fahrenheit ? "F" : "C");
#ifdef	ELA
   if (bletemp)
      gfx_text (0, 1, bletemp->missing ? "~~" : "BT");
   else
#endif
   if (num_ds18b20 || scd41)
      gfx_text (0, 2, " ");
   else
      gfx_text (0, 2, "~");
}

void
gfx_colour (char c)
{
   gfx_foreground (gfx_rgb (c));
}

uint8_t
menufunc1 (char key)
{                               /* Initial menu: Temp control - eventually menu structure somehow... */
   if (key == '2')
      return 0;
   int space = (gfx_height () - 35 * 3) / 2,
      y = 0;
   menuinit ();
   float d = 0;
   if (key == '1')
      d = 0.1;
   if (key == '3')
      d = -0.1;
   gfx_pos (10, y, GFX_T | GFX_L | GFX_H);
   gfx_colour ('R');
   if (!isnan (temptargetmax))
      gfx_temp (temptargetmax + d);
   y += 35 + space;
   gfx_pos (10, y, GFX_T | GFX_L | GFX_H);
   if (isnan (thistemp))
   {
      gfx_colour ('O');
      gfx_text (0, 5, "WAIT");
   } else
   {
      gfx_colour (thistemp < temptargetmin + d ? 'B' : thistemp > temptargetmax + d ? 'R' : 'G');
      gfx_temp (thistemp);
   }
   y += 35 + space;
   gfx_pos (10, y, GFX_T | GFX_L | GFX_H);
   gfx_colour ('B');
   if (!isnan (temptargetmin))
      gfx_temp (temptargetmin + d);
   if (temptimeprev < 0)
      return 0;
   if (key)
   {                            /* store settings */
      jo_t j = jo_object_alloc ();
      char s[30];
      if (!isnan (temptargetmin))
      {
         sprintf (s, "tempmin%d", temptimeprev + 1);
         jo_int (j, s, tempmin_scale * d + tempmin[temptimeprev]);
         sprintf (s, "tempmin%d", temptimenext + 1);
         jo_int (j, s, tempmin_scale * d + tempmin[temptimenext]);
      }
      if (!isnan (temptargetmax))
      {
         sprintf (s, "tempmax%d", temptimeprev + 1);
         jo_int (j, s, tempmax_scale * d + tempmax[temptimeprev]);
         sprintf (s, "tempmax%d", temptimenext + 1);
         jo_int (j, s, tempmax_scale * d + tempmax[temptimenext]);
      }
      revk_setting (j);
      jo_free (&j);
   }
   return 1;
}

uint8_t
menufunc2 (char key)
{                               /* Time */
   if (key)
      return 1;                 // Menu 1
   menuinit ();
   gfx_colour ('r');
   char s[10];
   time_t now = time (0);
   struct tm tm;
   localtime_r (&now, &tm);
   sprintf (s, "%02d", tm.tm_hour);
   gfx_pos (gfx_width () / 2, 0, GFX_C | GFX_T | GFX_V);
   gfx_text (0, 6, s);
   sprintf (s, "%02d", tm.tm_min);
   gfx_text (0, 5, s);
   sprintf (s, "%02d", tm.tm_sec);
   gfx_text (0, 4, s);
   return 2;
}

typedef uint8_t menufunc_t (char);
menufunc_t *menufunc[] = {
   menufunc1,
   menufunc2,
};

jo_t
env_status (void)
{
   jo_t j = jo_object_alloc ();
   if (!isnan (thisco2))
      jo_int (j, "CO2", thisco2);
   if (!isnan (thisrh))
      jo_int (j, "RH", thisrh);
   if (!isnan (thistemp))
      jo_litf (j, "TEMP", "%.1f", thistemp);
   return j;
}

static void
web_head (httpd_req_t * req, const char *title)
{
   httpd_resp_set_type (req, "text/html; charset=utf-8");
   httpd_resp_sendstr_chunk (req, "<meta name='viewport' content='width=device-width, initial-scale=1'>");
   httpd_resp_sendstr_chunk (req, "<html><head><title>");
   if (title)
      httpd_resp_sendstr_chunk (req, title);
   httpd_resp_sendstr_chunk (req, "</title></head><style>"      //
                             "body{font-family:sans-serif;background:#8cf;}"    //
                             ".on{opacity:1;transition:1s;}"    //
                             ".off{opacity:0;transition:1s;}"   //
                             ".switch,.box{position:relative;display:inline-block;width:64px;height:34px;margin:3px;}"  //
                             ".switch input,.box input{opacity:0;width:0;height:0;}"    //
                             ".slider,.button{position:absolute;cursor:pointer;top:0;left:0;right:0;bottom:0;background-color:#ccc;-webkit-transition:.4s;transition:.4s;}"     //
                             ".slider:before{position:absolute;content:\"\";height:26px;width:26px;left:4px;bottom:3px;background-color:white;-webkit-transition:.4s;transition:.4s;}"  //
                             "input:checked+.slider,input:checked+.button{background-color:#12bd20;}"   //
                             "input:checked+.slider:before{-webkit-transform:translateX(30px);-ms-transform:translateX(30px);transform:translateX(30px);}"      //
                             "span.slider:before{border-radius:50%;}"   //
                             "span.slider,span.button{border-radius:34px;padding-top:8px;padding-left:10px;border:1px solid gray;box-shadow:3px 3px 3px #0008;}"        //
                             "</style><body><h1>");
   if (title)
      httpd_resp_sendstr_chunk (req, title);
   httpd_resp_sendstr_chunk (req, "</h1>");
}

static esp_err_t
web_foot (httpd_req_t * req)
{
   httpd_resp_sendstr_chunk (req, "<hr><address>");
   char temp[20];
   snprintf (temp, sizeof (temp), "%012llX", revk_binid);
   httpd_resp_sendstr_chunk (req, temp);
   httpd_resp_sendstr_chunk (req, "</address></body></html>");
   httpd_resp_sendstr_chunk (req, NULL);
   return ESP_OK;
}

static esp_err_t
web_icon (httpd_req_t * req)
{                               // serve image -  maybe make more generic file serve
   extern const char start[] asm ("_binary_apple_touch_icon_png_start");
   extern const char end[] asm ("_binary_apple_touch_icon_png_end");
   httpd_resp_set_type (req, "image/png");
   httpd_resp_send (req, start, end - start);
   return ESP_OK;
}

static esp_err_t
web_root (httpd_req_t * req)
{
   // TODO cookies
   // webcontrol=0 means no web
   // webcontrol=1 means user settings, not wifi settings
   // webcontrol=2 means all
   if (revk_link_down () && webcontrol >= 2)
      return revk_web_settings (req);   // Direct to web set up
   web_head (req, *hostname ? hostname : appname);
   httpd_resp_sendstr_chunk (req, "<table id=top>");
   httpd_resp_sendstr_chunk (req, "<tr><td>CO₂</td><td id=CO2 align=right></td><td>ppm</td></tr>");
   httpd_resp_sendstr_chunk (req, "<tr><td>RH</td><td id=RH align=right></td><td>%</td></tr>");
   httpd_resp_sendstr_chunk (req, "<tr><td>Temp</td><td id=TEMP align=right></td><td>℃</td></tr>");
   httpd_resp_sendstr_chunk (req, "</table>");
   if (webcontrol >= 2)
      httpd_resp_sendstr_chunk (req, "<p><a href='revk-settings'>Settings</a></p>");
   httpd_resp_sendstr_chunk (req, "<script>"    //
                             "var ws=0;"        //
                             "var temp=0;"      //
                             "function g(n){return document.getElementById(n);};"       //
                             "function s(n,v){var d=g(n);if(d)d.textContent=v;}"        //
                             "function c(){"    //
                             "ws=new WebSocket('ws://'+window.location.host+'/status');"        //
                             "ws.onopen=function(v){g('top').className='on';};" //
                             "ws.onclose=function(v){g('top').className='off';setTimeout(function() {c();},1000);};"    //
                             "ws.onerror=function(v){g('top').className='off';setTimeout(function() {c();},10000);};"   //
                             "ws.onmessage=function(v){"        //
                             "o=JSON.parse(v.data);"    //
                             "s('CO2',o.CO2);"  //
                             "s('RH',o.RH);"    //
                             "s('TEMP',o.TEMP);"        //
                             "};};c();" //
                             "setInterval(function() {ws.send('');},1000);"     //
                             "</script>");
   return web_foot (req);
}


static esp_err_t
web_status (httpd_req_t * req)
{                               // Web socket status report
   // TODO cookies
   int fd = httpd_req_to_sockfd (req);
   void wsend (jo_t * jp)
   {
      char *js = jo_finisha (jp);
      if (js)
      {
         httpd_ws_frame_t ws_pkt;
         memset (&ws_pkt, 0, sizeof (httpd_ws_frame_t));
         ws_pkt.payload = (uint8_t *) js;
         ws_pkt.len = strlen (js);
         ws_pkt.type = HTTPD_WS_TYPE_TEXT;
         httpd_ws_send_frame_async (req->handle, fd, &ws_pkt);
         free (js);
      }
   }
   esp_err_t status (void)
   {
      jo_t j = env_status ();
      wsend (&j);
      return ESP_OK;
   }
   if (req->method == HTTP_GET)
      return status ();         // Send status on initial connect
   // received packet
   httpd_ws_frame_t ws_pkt;
   uint8_t *buf = NULL;
   memset (&ws_pkt, 0, sizeof (httpd_ws_frame_t));
   ws_pkt.type = HTTPD_WS_TYPE_TEXT;
   esp_err_t ret = httpd_ws_recv_frame (req, &ws_pkt, 0);
   if (ret)
      return ret;
   if (!ws_pkt.len)
      return status ();         // Empty string
   buf = calloc (1, ws_pkt.len + 1);
   if (!buf)
      return ESP_ERR_NO_MEM;
   ws_pkt.payload = buf;
   ret = httpd_ws_recv_frame (req, &ws_pkt, ws_pkt.len);
   if (!ret)
   {
      jo_t j = jo_parse_mem (buf, ws_pkt.len);
      if (j)
      {
         // TODO
         jo_free (&j);
      }
   }
   free (buf);
   return status ();
}

void
app_main ()
{
   revk_boot (&app_callback);
   revk_start ();
   for (int i = 0; i < sizeof (gpiofixed) / sizeof (*gpiofixed); i++)
      if (gpiofixed[i].set)
         revk_gpio_output (gpiofixed[i], 1);
   revk_gpio_output (fanco2gpio, 0);
   revk_gpio_output (heatgpio, 0);
#ifdef CONFIG_REVK_LED_STRIP
   if (ledrgb.set && lednum)
   {
      led_strip_config_t strip_config = {
         .strip_gpio_num = (ledrgb.num),
         .max_leds = lednum,    // The number of LEDs in the strip,
#ifdef	LED_STRIP_COLOR_COMPONENT_FMT_GRB
         .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
#else
         .led_pixel_format = LED_PIXEL_FORMAT_GRB,
#endif
         .led_model = LED_MODEL_WS2812, // LED strip model
         .flags.invert_out = ledrgb.invert,     // whether to invert the output signal (useful when your hardware has a level inverter)
      };
      led_strip_rmt_config_t rmt_config = {
         .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
         .resolution_hz = 10 * 1000 * 1000,     // 10MHz
#ifdef  CONFIG_IDF_TARGET_ESP32S3
         .flags.with_dma = true,
#endif
      };
      REVK_ERR_CHECK (led_strip_new_rmt_device (&strip_config, &rmt_config, &strip));
   }
#endif
#ifndef CONFIG_GFX_BUILD_SUFFIX_GFXNONE
   {
      int p;
      for (p = 0; p < sizeof (logo) && !logo[p]; p++);
      if (p == sizeof (logo))
         memcpy (logo, icon_logo, sizeof (icon_logo));  /* default */
   }
#endif
   if (sda.set && scl.set)
   {
      scd41 = (co2address == 0x62 ? 1 : 0);
      i2cport = 0;
      if (i2c_driver_install (i2cport, I2C_MODE_MASTER, 0, 0, 0))
      {
         jo_t j = jo_object_alloc ();
         jo_string (j, "error", "Install fail");
         jo_int (j, "sda", sda.num);
         jo_int (j, "scl", scl.num);
         revk_error ("I2C", &j);
         i2cport = -1;
      } else
      {
         i2c_config_t config = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = sda.num,
            .scl_io_num = scl.num,
            .sda_pullup_en = true,
            .scl_pullup_en = true,
            .master.clk_speed = 100000,
         };
         if (i2c_param_config (i2cport, &config))
         {
            i2c_driver_delete (i2cport);
            jo_t j = jo_object_alloc ();
            jo_string (j, "error", "Config fail");
            jo_int (j, "sda", sda.num);
            jo_int (j, "scl", scl.num);
            revk_error ("I2C", &j);
            i2cport = -1;
         } else
            i2c_set_timeout (i2cport, 80000 * 5);       /* 5 ms ? allow for clock stretching */
      }
   }
#ifndef	CONFIG_GFX_BUILD_SUFFIX_GFXNONE
   if (gfxmosi.set)
   {
    const char *e = gfx_init (cs: gfxcs.num, sck: gfxsck.num, mosi: gfxmosi.num, dc: gfxdc.num, rst: gfxrst.num, flip:gfxflip);
      if (e)
      {
         jo_t j = jo_object_alloc ();
         jo_string (j, "error", "Failed to start");
         jo_string (j, "description", e);
         revk_error ("GFX", &j);
      }
   }
#endif
   for (int i = 0; i < sizeof (button) / sizeof (*button); i++)
      revk_gpio_input (button[i]);
   gfx_lock ();
   gfx_colour ('B');
   gfx_box (gfx_width (), gfx_height (), 255);
   gfx_unlock ();
   if (i2cport >= 0)
      revk_task ("I2C", i2c_task, NULL, 4);
   if (ds18b20.set)
      revk_task ("DS18B20", ds18b20_task, NULL, 4);
   gfx_lock ();
   gfx_clear (0);
   gfx_unlock ();
   /* Main task... */
#ifndef CONFIG_GFX_BUILD_SUFFIX_GFXNONE
   time_t showtime = 0;
   char showlogo = 1;
   float showco2 = NAN;
   float showtemp = NAN;
   float showrh = NAN;
   void reset (void)
   {                            /* re display all */
      gfx_clear (0);
      showlogo = 1;
      showtime = 0;
      showco2 = NAN;
      showtemp = NAN;
      showrh = NAN;
      lasticon = 0;
   };
#endif
   if (alsdark && sda.set && scl.set && alsaddress)
      gfx_dark = 1;             // Start dark

   httpd_config_t config = HTTPD_DEFAULT_CONFIG ();
   if (!httpd_start (&webserver, &config))
   {
      if (webcontrol >= 2)
         revk_web_settings_add (webserver);
      if (webcontrol)
      {
         {
            httpd_uri_t uri = {
               .uri = "/",
               .method = HTTP_GET,
               .handler = web_root,
            };
            REVK_ERR_CHECK (httpd_register_uri_handler (webserver, &uri));
         }
         {
            httpd_uri_t uri = {
               .uri = "/apple-touch-icon.png",
               .method = HTTP_GET,
               .handler = web_icon,
            };
            REVK_ERR_CHECK (httpd_register_uri_handler (webserver, &uri));
         }
         {
            httpd_uri_t uri = {
               .uri = "/status",
               .method = HTTP_GET,
               .handler = web_status,
               .is_websocket = true,
            };
            REVK_ERR_CHECK (httpd_register_uri_handler (webserver, &uri));
         }
      }
   }

#ifdef	ELA
   if (*ble)
      bleenv_run ();
#endif

   while (1)
   {                            /* Main loop - handles display and UI, etc. */
      usleep (10000LL - (esp_timer_get_time () % 10000LL));     /* wait a bit */
      time_t now = time (0);
      struct tm t;
      localtime_r (&now, &t);
      uint16_t hhmm = t.tm_hour * 100 + t.tm_min;
      uint32_t up = uptime ();
      uint32_t lastup = 0;
      if (up != lastup)
      {                         // every second
         lastup = up;
#ifdef	ELA
         if (*ble)
         {                      // BLE working
            bleenv_expire (120);
            if (!bletemp)
               for (bleenv_t * e = bleenv; e; e = e->next)
                  if (!strcmp (e->name, ble))
                  {
                     ESP_LOGI (TAG, "Found BLE %s", ble);
                     bletemp = e;
                     break;
                  }
            if (bletemp && !bletemp->missing)
               lasttemp = report ("temp", lasttemp, thistemp = bletemp->temp / 100.0, tempplaces);      // use BLE temp
         }
#endif
      }
      if (!airconlast || airconlast + 300 < uptime ())
      {                         // Not seen aircon for a while (should update every 60 seconds, so 5 mins is plenty)
         airconmode = 0;
         airconpower = 0;
         airconlast = 0;
      }
#ifndef CONFIG_GFX_BUILD_SUFFIX_GFXNONE
      char icon = 0;
      if (airconpower)
      {
         if ((airconslave || airconantifreeze) && (airconmode == 'H' || airconmode == 'C'))
            icon = tolower (airconmode);        // Display icon in slave mode
         else
            icon = airconmode;  // Display icon
      }
#endif
      if (sendinfo && (co2_found || num_ds18b20 || sht_found) && !do_co2)
      {                         /* Send device info */
         sendinfo = 0;
         jo_t j = jo_object_alloc ();
         if (co2_found)
         {
            if (!scd_serial && !scd_tempoffset && !scd_altitude && !scd41)
               jo_bool (j, "SCD30", 1);
            else
            {
               jo_object (j, scd41 ? "SCD41" : "SCD30");
               if (scd_serial)
                  jo_stringf (j, "serial", "%012llX", scd_serial);
               if (scd_tempoffset)
                  jo_int (j, "temperature-offset", scd_tempoffset);
               if (scd_altitude)
                  jo_int (j, "sensor-altitude", scd_altitude);
               if (scd41)
                  jo_bool (j, "automatic-self-calibration", scd_selfcal);
               jo_close (j);
            }
         }
#if 0                           // FFS library does not expose address any more
         if (num_ds18b20)
         {
            jo_array (j, "DS18B20");
            for (int i = 0; i < num_ds18b20; i++)
               jo_stringf (j, NULL, "%016llX", *(unsigned long long *) &adr_ds18b20[i].address);
            jo_close (j);
         }
#else
         if (num_ds18b20)
            jo_bool (j, "DS18B20", 1);
#endif
         if (sht_found)
            jo_stringf (j, "SHT40", "%08lX", sht_serial);
         revk_info ("info", &j);
      }

      if (ha_send)
         send_ha_config ();
      if (alsaddress)
      {                         // ALS based night mode
         if (!isnan (lastals))
         {
            if (lastals > alsdark)      // LUX*100
               gfx_dark = 0;
            else
               gfx_dark = 1;
         }
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
      /* heatday or heatnight take priority.If not set(0) then temphhmm / tempmin / tempmax apply */
      /*
       * The temp are a set, with hhmm points(in order, can start 0000) and heating and cooling settings, and 0 means same as other
       * setting
       */
      int32_t heat_target = (gfx_dark ? heatnight : heatday);
      if (!tempmin[0] && !tempmax[0])
      {                         /* Temp is set based on night / day, use that as heating basis(min) and no cooling set - legacy */
         if (heat_target)
         {
            temptargetmin = ((float) heat_target) / 1000.0;
            temptargetmax = 32.0;
         }
         temptimeprev = temptimenext = -1;
      } else
      {
         /* Setting from temphhmm / tempmin / tempmax */
#define	TIMES	(sizeof(temphhmm)/sizeof(*temphhmm))
         int i;
         temptimeprev = 0;
         temptimenext = 0;
         for (i = 0; i < TIMES && (tempmin[i] || tempmax[i]) && temphhmm[i] <= hhmm; i++);
         if (!i)
         {                      /* wrap as first entry is later */
            for (i = 1; i < TIMES && (tempmin[i] || tempmax[i]); i++);
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
         if ((a = (tempmin[temptimeprev] ? : tempmax[temptimeprev])) && (b = (tempmin[temptimenext] ? : tempmax[temptimenext])))
         {                      /* Heat valid */
            heat_target = a + (b - a) * (snow - sprev) / (snext - sprev);
            min = ((float) heat_target) / 1000.0;
         }
         if ((a = (tempmax[temptimeprev] ? : tempmin[temptimeprev])) && (b = (tempmax[temptimenext] ? : tempmin[temptimenext])))
            max = (float) (a + (b - a) * (snow - sprev) / (snext - sprev)) / 1000.0;    /* Cool valid */
         else
            max = min;          /* same as heat */
         temptargetmin = min;
         temptargetmax = max;
      }
      if (!isnan (tempoverridemin))
      {
         temptargetmin = tempoverridemin;
         heat_target = temptargetmin * 1000;
         temptimeprev = temptimenext = -1;
      }
      if (!isnan (tempoverridemax))
      {
         temptargetmax = tempoverridemax;
         temptimeprev = temptimenext = -1;
      }
      {                         // Per minute heat and fan
         static uint32_t lastmin = 0;
         if (now / 60 != lastmin)
         {
            lastmin = now / 60;
            if (!isnan (thisco2) && !isnan (thisrh) && (fanco2on || fanco2off || fanrhon || fanrhoff))
            {                   /* Fan control */
               const char *fan = NULL;
               if (((fanco2on && thisco2 > fanco2on) || (fanrhon && thisrh > fanrhon)))
               {                /* Fan on */
                  if (fanlast != 1)
                  {             /* Change */
                     revk_gpio_set (fanco2gpio, 1);
                     fan = fanon;
                     fanlast = 1;
                  }
               } else if (fanlast != 0)
               {                /* Fan off, change */
                  revk_gpio_set (fanco2gpio, 0);
                  fan = fanoff;
                  fanlast = 0;
               }
               if (!fan && fanresend && fantime < up && fanlast > 0)
                  fan = (fanlast ? fanon : fanoff);
               if (fan && *fan)
               {
                  fantime = up + fanresend;
                  revk_mqtt_send_str (fan);
               }
            }
#ifndef CONFIG_GFX_BUILD_SUFFIX_GFXNONE
            if (fanon && *fanon && fanlast == 1)
               icon = 'E';      // Extractor fan icon
#endif
            if (!isnan (thistemp) && (heatnight || heatday || tempmin[0] || heat_target || heatgpio.set || heaton || heatoff))
            {                   /* Heat control */
               static int32_t last1 = 0,
                  last2 = 0;
               int32_t thismC = thistemp * 1000;
               if (heat_target || heatlast == 1)
               {                /* We have a reference temp to work with or we left on */
                  if (heatresend && heattime < up)
                     heatlast = -1;
                  const char *heat = NULL;
                  int32_t predict = thismC;
                  if (heatahead && ((last2 <= last1 && last1 <= thismC) ||      // going up - turn off early if predict above target
                                    ((last2 >= last1 && last1 >= thismC) &&     // going down - turn on early in 10 min stages if predict is below target
                                     (!heatfade || !heatfadem
                                      || (lastmin % heatfadem) < (heat_target + heatfade - thismC) * heatfadem / heatfade))))
                     predict += heatahead * (thismC - last2) / 2;       // Use predicted value, i.e. turn on/off early
                  if (!heat_target || predict > heat_target || (airconpower && airconmode != 'H'))
                  {             /* Heat off */
                     if (heatlast != 0)
                     {          /* Change */
                        revk_gpio_set (heatgpio, 0);
                        heat = heatoff;
                        heatlast = 0;
                     }
                  } else if (heatlast != 1)
                  {             /* Heat on, change */
                     revk_gpio_set (heatgpio, 1);
                     heat = heaton;
                     heatlast = 1;
                  }
                  if (heat && *heat)
                  {
                     heattime = up + heatresend;
                     revk_mqtt_send_str (heat);
                  }
               }
               last2 = last1;
               last1 = thismC;
            }
         }
      }
#ifndef CONFIG_GFX_BUILD_SUFFIX_GFXNONE
      if (heaton && *heaton && heatlast == 1)
         icon = 'R';            // Radiator icon
#endif
      static uint8_t menu = 0;  /* Menu selection - 0 if idle */
      /* Handle key presses */
      char key = 0;
      {                         /* Simple debounce */
         uint8_t last0 = 0;
         static uint8_t last1 = 0;
         static uint8_t last2 = 0;
         for (int i = 0; i < sizeof (button) / sizeof (*button); i++)
            if (revk_gpio_get (button[i]))
               last0 |= (1 << i);
         if (last0 == last1)
         {                      // stable
            for (int i = 0; i < sizeof (button) / sizeof (*button); i++)
               if (!(last2 & (1 << i)) && (last1 & (1 << i)))
                  key = '1' + i;
            last2 = last1;
         }
         last1 = last0;
      }
      if (!menu && key)
      {
         menu_time = uptime () + 5;
         menu = (gfx_dark ? 2 : 1);     // Base menu, if dark then time first
         key = 0;               // Don't pass initial key, used just to wake up...
      }
      if (scd41_settled && scd41_settled < up)
         scd41_settled = 0;     // Settled
      /* Report */
      if (values && (up > 60 || sntp_get_sync_status () == SNTP_SYNC_STATUS_COMPLETED)
          && !((!reportchange || now < reportchange + lag) && (!reporting || now / reporting == reportlast / reporting)))
      {
         revk_command ("status", NULL);
         reportlast = now;
         reportchange = 0;
      }
#ifndef	CONFIG_GFX_BUILD_SUFFIX_GFXNONE
      /* Display */
      char s[30];               /* Temp string */
      if (gfx_msg_time)
      {                         /* Fixed message */
         gfx_set_contrast (gfxlight);
         if (menu || gfx_msg_time < uptime ())
         {                      /* Time up, clear and drop through */
            gfx_msg_time = 0;
            gfx_lock ();
            reset ();
            gfx_unlock ();
         } else
         {
            gfx_message (gfx_msg);      /* does own locking */
            continue;
         }
      }
      if (countdown)
      {                         // Countdown timer
         gfx_set_contrast (gfxlight);
         gfx_lock ();
         reset ();
         int32_t t = countdown - uptime ();
         if (t <= 0)
            countdown = 0;
         else
         {
            revk_blink (0, 0, gfx_dark ? "K" : "R");
            gfx_colour ('r');
            if (t > gfx_height () / 2)
            {
               gfx_pos (gfx_width () / 2, gfx_height () / 2, GFX_M | GFX_C);
               sprintf (s, "%ld", t);
               gfx_text (0, 6, s);
            } else
            {
               gfx_pos (gfx_width () / 2 - t, gfx_height () / 2 - t, 0);
               gfx_fill (t * 2, t * 2, 255);
               gfx_colour ('K');
               gfx_pos (gfx_width () / 2 - t + 1, gfx_height () / 2 - t + 1, 0);
               gfx_fill (t * 2 - 2, t * 2 - 2, 255);
            }
         }
         gfx_unlock ();
         continue;
      }
      gfx_lock ();
      if (menu)
      {                         /* Display menu selection */
         if (key)
            menu_time = uptime () + 10;
         if (menu_time < uptime ())
            menu = 0;
         else if (menu > sizeof (menufunc) / sizeof (*menufunc))
            menu = 0;
         else if (menu)
            menu = menufunc[menu - 1] (key);
         if (!menu)
            reset ();           // Exit menu
         else
         {
            gfx_unlock ();
            continue;
         }
      }
      if (gfx_dark)
      {                         /* Night mode */
         gfx_set_contrast (gfxdark);    /* Dark / dim */
         revk_blink (0, 0, "K");
         gfx_colour ('b');
         reset ();
         if (!notime)
         {
            strftime (s, sizeof (s), "%H:%M", &t);
            int d = t.tm_sec;
            if (t.tm_min & 1)
               d = 60 - d;
            int y = gfx_height () / 2 + (d - 30);
            d = t.tm_min;
            if (t.tm_hour & 1)
               d = 60 - d;
            int x = gfx_width () / 2 + (d - 30) * 2 / 6;        /* Adjusted to fit display */
            gfx_pos (x, y, GFX_M | GFX_C);
            gfx_text (0, 4, s);
         }
         gfx_unlock ();
         continue;
      }
      /* Normal - idle temp status */
      gfx_set_contrast (gfxlight);
      if (showlogo)
      {
         showlogo = 0;
         gfx_clear (0);
         if (!nologo)
         {
            gfx_pos (gfx_width () - LOGOW, gfx_width () - 12, GFX_B | GFX_L);
            gfx_icon16 (LOGOW, LOGOH, logo);
         }
      }
      if (now != showtime && !notime)
      {
         showtime = now;
         if (t.tm_year > 100)
         {
            gfx_colour ('w');
            strftime (s, sizeof (s), "%F\004%T %Z", &t);
            gfx_pos (0, gfx_height () - 1, GFX_B | GFX_L);
            gfx_text (0, 1, s);
         }
      }
      int y = 0,
         space = (gfx_height () - 28 - 35 - 21 - 9) / 3;
      char co2col = (isnan (thisco2) ? 'K' : thisco2 > (fanco2on ? : 1000) ? 'R' : thisco2 > (fanco2off ? : 750) ? 'Y' : 'G');
      char tempcol = (isnan (thistemp) ? 'K' : thistemp > temptargetmax + 0.25 ? 'R' : thistemp < temptargetmin - 0.25 ? 'B' : 'G');
      char rhcol = (isnan (thisrh) ? 'K' : 'C');
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
         revk_blink (0, 0, *cols ? cols : NULL);
      }
      if (thisco2 != showco2 && !isnan (thisco2))
      {
         showco2 = thisco2;
         gfx_colour (co2col);
         if (showco2 < 200)
            strcpy (s, "?LOW");
         else if (showco2 >= 10000.0)
            strcpy (s, "HIGH");
         else
            sprintf (s, "%4d", (int) showco2);
         gfx_pos (4, y, GFX_T | GFX_L | GFX_H);
         gfx_text (0, 4, s);
         int x;
         gfx_pos (x = gfx_x (), gfx_y (), GFX_T | GFX_L | GFX_H);
         gfx_text (0, 1, "CO");
         gfx_pos (gfx_x (), gfx_y () + 3, GFX_T | GFX_L | GFX_V);
         gfx_text (0, 0, "2");
         gfx_pos (x, gfx_y (), GFX_T | GFX_L | GFX_V);
         gfx_text (1, 1, "ppm");
      }
      y += 28 + space;
      gfx_pos (10, y, GFX_T | GFX_L | GFX_H);
      if (!num_ds18b20 && scd41 && scd41_settled)
      {                         // Waiting
         sprintf (s, "%ld:%02ld ", (scd41_settled - up) / 60, (scd41_settled - up) % 60);
         gfx_colour ('O');
         gfx_text (0, 5, scd41_settled <= up ? "-:--" : s);
         showtemp = NAN;
      } else
      {
         if (!isnan (thistemp) && thistemp != showtemp)
         {
            showtemp = thistemp;
            gfx_colour (tempcol);
            gfx_temp (showtemp);
         }
      }
      y += 35 + space;
      if (thisrh != showrh && !isnan (thisrh))
      {
         showrh = thisrh;
         gfx_colour (rhcol);
         gfx_pos (3, y, GFX_T | GFX_L | GFX_H);
         if (showrh <= 0)
            strcpy (s, "__");
         else if (showrh >= 100)
            strcpy (s, "^^");
         else
            sprintf (s, "%2d", (int) showrh);
         gfx_text (0, 3, s);
         gfx_text (0, 2, "%%");
         gfx_text (0, 1, "R");
         gfx_text (0, 1, "H");
      }
      y += 21 + space;
      if (!icon && airconlast)
         icon = 'P';            // power
      if (!icon && revk_link_down ())
         icon = 'N';
      if (icon != lasticon)
      {                         // Status icon
         lasticon = icon;
         gfx_pos (gfx_width () - LOGOW * 2 - 2, gfx_height () - 12, GFX_B | GFX_L);
         gfx_colour (icon == 'R' ? 'r' : icon == 'F' ? 'C' : icon == 'E' ? 'g' : icon == 'C' ? 'B' : icon == 'c' ? 'Y' : icon ==
                     'H' ? 'R' : icon == 'h' ? 'Y' : icon == 'D' ? 'Y' : icon == 'A' ? 'G' : icon == 'P' ? 'w' : icon ==
                     'N' ? 'M' : 'W');
         gfx_icon16 (LOGOW, LOGOH,
                     icon == 'R' ? icon_rad : icon == 'F' ? icon_modeF : icon == 'E' ? icon_fan : icon == 'C' ? icon_modeC : icon ==
                     'c' ? icon_modeCS : icon == 'H' ? icon_modeH : icon == 'h' ? icon_modeHS : icon == 'D' ? icon_modeD : icon ==
                     'A' ? icon_modeA : icon == 'P' ? icon_power : icon == 'N' ? icon_nowifi : NULL);
      }
      gfx_unlock ();
#endif
#ifdef  CONFIG_REVK_LED_STRIP
      if (strip)
      {                         // LED status
         for (int i = 0; i < lednum; i++)
            led_strip_set_pixel (strip, i, 0, 0, 0);
         if (ledco2)
         {                      // Show CO2 on LEDs
            int n = 0;
            char c = 'K';
            if (!isnan (lastco2) && co2high)
            {
               if (lastco2 > co2high)
                  c = 'R';
               else if (lastco2 > co2ok)
                  c = 'O';
               else if (lastco2 > co2good)
                  c = 'Y';
               else
                  c = dark ? 'K' : 'G';
               n = (lednum * lastco2 + co2high / 2) / co2high;
            }
            if (n > lednum)
               n = lednum;
            if (!n)
            {
               n = 1;
               c = 'b';
            }
            for (int i = 0; i < n; i++)
               revk_led (strip, i, 255, revk_rgb (c));
         }
         REVK_ERR_CHECK (led_strip_refresh (strip));
      }
#endif
   }
}

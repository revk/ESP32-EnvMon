Environmental monitoring with OLED display working over MQTT

* Uses 128x128 1.5" RGB OLED display (readily available from Amazon)
* Uses SCD41 sensor, and can use this for temp or external DS18B20
* Older board design included for SCD30 and external DS18B20

The design makes for a small unit powered by a USB cable, or 12V DC, with
an RGB OLED display showing CO2, temperature, and humidity.

It can also be set with CO2 level and temperature levels which can trigger
MQTT messages allowing control of fans or heating systems.

The CO2 sensor used is an expensive NDIR module providing accurate CO2
measurement which is not fooled by farts, etc.

Also included are some linux tools to log data from this module to sql and graph it as svg.
Additional tools are also included to log and graph power data from tasmota.

![IMG_8093](https://user-images.githubusercontent.com/996983/114298107-a1878e00-9aac-11eb-95fa-5be9127406a7.jpg)

(c) 2019-21 Andrews & Arnold Ltd, Adrian Kennard. See LICENSE file (GPL).

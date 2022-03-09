#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := Env
SUFFIX := $(shell components/ESP32-RevK/suffix)

all:
	@echo Make: $(PROJECT_NAME)$(SUFFIX).bin
	@idf.py build
	@cp build/$(PROJECT_NAME).bin $(PROJECT_NAME)$(SUFFIX).bin
	@echo Done: $(PROJECT_NAME)$(SUFFIX).bin

tools: envlog envgraph taspowerlog taspowersvg

set:    wroom pico

pico:
	@sed -e 's/# CONFIG_ESP32_SPIRAM_SUPPORT is not set/CONFIG_ESP32_SPIRAM_SUPPORT=y/' -e 's/# CONFIG_ESPTOOLPY_FLASHSIZE_8MB is not set/CONFIG_ESPTOOLPY_FLASHSIZE_8MB=y/' -e 's/CONFIG_ESPTOOLPY_FLASHSIZE_4MB=y/# CONFIG_ESPTOOLPY_FLASHSIZE_4MB is not set/' -e 's/CONFIG_FREERTOS_UNICORE=y/# CONFIG_FREERTOS_UNICORE is not set/' sdkconfig > sdkconfig.new
	@mv -f sdkconfig.new sdkconfig
	@make

wroom:
	@sed -e 's/CONFIG_ESP32_SPIRAM_SUPPORT=y/# CONFIG_ESP32_SPIRAM_SUPPORT is not set/' -e 's/# CONFIG_ESPTOOLPY_FLASHSIZE_4MB is not set/CONFIG_ESPTOOLPY_FLASHSIZE_4MB=y/' -e 's/CONFIG_ESPTOOLPY_FLASHSIZE_8MB=y/# CONFIG_ESPTOOLPY_FLASHSIZE_8MB is not set/' -e 's/CONFIG_FREERTOS_UNICORE=y/# CONFIG_FREERTOS_UNICORE is not set/' sdkconfig > sdkconfig.new
	@mv -f sdkconfig.new sdkconfig
	@make

flash:
	idf.py flash

monitor:
	idf.py monitor

clean:
	idf.py clean

menuconfig:
	idf.py menuconfig

#include $(IDF_PATH)/make/project.mk

pull:
	git pull
	git submodule update --recursive

update:
	git submodule update --init --remote --merge --recursive
	git commit -a -m "Library update"

# Set GPIO low (whichever CBUS is set to mode 8/GPIO)
bootmode: ftdizap/ftdizap
	./ftdizap/ftdizap --cbus=0

# Flash with GPIO control using CBUS0 (FT230X design)
zap:    bootmode flash
	./ftdizap/ftdizap --cbus=1 --reset

# Program the FTDI
ftdi: ftdizap/ftdizap
	./ftdizap/ftdizap --serial="RevK" --description="ESP32-EnvMon" --cbus3-mode=17 --self-powered=1

ftdizap/ftdizap: ftdizap/ftdizap.c
	make -C ftdizap

SQLlib/sqllib.o: SQLlib/sqllib.c
	make -C SQLlib
AXL/axl.o: AXL/axl.c
	make -C AXL
AJL/ajl.o: AJL/ajl.c
	make -C AJL

SQLINC=$(shell mariadb_config --include)
SQLLIB=$(shell mariadb_config --libs)
SQLVER=$(shell mariadb_config --version | sed 'sx\..*xx')
CCOPTS=${SQLINC} -I. -I/usr/local/ssl/include -D_GNU_SOURCE -g -Wall -funsigned-char -lm
OPTS=-L/usr/local/ssl/lib ${SQLLIB} ${CCOPTS}

envlog: envlog.c SQLlib/sqllib.o AJL/ajl.o
	cc -O -o $@ $< -lpopt -lmosquitto -ISQLlib SQLlib/sqllib.o -IAJL AJL/ajl.o ${OPTS}

envgraph: envgraph.c SQLlib/sqllib.o AXL/axl.o
	cc -O -o $@ $< -lpopt -lmosquitto -ISQLlib SQLlib/sqllib.o -IAXL AXL/axl.o -lcurl ${OPTS}

taspowerlog: taspowerlog.c SQLlib/sqllib.o AJL/ajl.o
	cc -O -o $@ $< -lpopt -lmosquitto -ISQLlib SQLlib/sqllib.o -IAJL AJL/ajl.o ${OPTS}

taspowersvg: taspowersvg.c SQLlib/sqllib.o AXL/axl.o
	cc -O -o $@ $< -lpopt -lmosquitto -ISQLlib SQLlib/sqllib.o -IAXL AXL/axl.o -lcurl ${OPTS}

PCBCase/case: PCBCase/case.c
	make -C PCBCase

scad: KiCad/EnvMon2.scad KiCad/EnvMon2-noco2.scad KiCad/EnvMon.scad KiCad/EnvMon-noco2.scad KiCad/EnvMon-nooled.scad KiCad/EnvMon-flush.scad KiCad/EnvMon-flush-noco2.scad KiCad/EnvMon-flush-nooled.scad KiCad/EnvMon-nooled-noco2.scad
stl: KiCad/EnvMon2.stl KiCad/EnvMon2-noco2.stl KiCad/EnvMon.stl KiCad/EnvMon-noco2.stl KiCad/EnvMon-nooled.stl KiCad/EnvMon-flush.stl KiCad/EnvMon-flush-noco2.stl KiCad/EnvMon-flush-nooled.stl KiCad/EnvMon-nooled-noco2.stl

%.stl: %.scad
	echo "Making $@"
	/Applications/OpenSCAD.app/Contents/MacOS/OpenSCAD $< -o $@
	echo "Made $@"

KiCad/EnvMon2.scad: KiCad/EnvMon2.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=8 --top=10.4 --base=2 --ignore=D3

KiCad/EnvMon2-noco2.scad: KiCad/EnvMon2.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=8 --top=10.4 --base=2 --ignore=D3 --edge2

KiCad/EnvMon.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=8 --top=10.4 --ignore=M1.2,M2.1,D1

KiCad/EnvMon-noco2.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=2.5 --top=10.4 --ignore=M2.1,M1,D1

KiCad/EnvMon-nooled.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=8 --top=6 --ignore=M1.2,M2

KiCad/EnvMon-nooled-noco2.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=2.5 --top=6 --ignore=M1.2,M2,M2.1,M1,D1

KiCad/EnvMon-flush.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=8 --top=10.4 --ignore=M2.1,M1.2,D1 --spacing=80
	echo 'translate([62,-23.75,0])difference(){' >> $@
	cat PCBCase/models/blankplate.scad >> $@
	echo 'translate([50,10,5])rotate([0,90,0])hull(){cylinder(d=4,h=100,$$fn=48);translate([-10,0,0])cylinder(d=4,h=100,$$fn=48);}' >> $@
	echo 'translate([20,25.75,-1])cube([46,38,10]);' >> $@
	echo 'for(x=[12.85:10.05:73.15])translate([x,15,-1])cylinder(d=3,h=10,$$fn=48);' >> $@
	echo '}' >> $@

KiCad/EnvMon-flush-noco2.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=2.5 --top=10.4 --ignore=M2.1,M1,D1 --spacing=80
	echo 'translate([62,-23.75,0])difference(){' >> $@
	cat PCBCase/models/blankplate.scad >> $@
	echo 'translate([50,10,5])rotate([0,90,0])hull(){cylinder(d=4,h=100,$$fn=48);translate([-10,0,0])cylinder(d=4,h=100,$$fn=48);}' >> $@
	echo 'translate([20,25.75,-1])cube([46,38,10]);' >> $@
	echo '}' >> $@

KiCad/EnvMon-flush-nooled.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=8 --top=8 --ignore=M1.2,M2,D1 --spacing=80
	echo 'translate([62,-18.75,0])difference(){' >> $@
	cat PCBCase/models/blankplate.scad >> $@
	echo 'translate([50,10,5])rotate([0,90,0])hull(){cylinder(d=4,h=100,$$fn=48);translate([-10,0,0])cylinder(d=4,h=100,$$fn=48);}' >> $@
	echo 'translate([20,20.75,-1])cube([46,38,10]);' >> $@
	echo 'for(x=[12.85:10.05:73.15])translate([x,15,-1])cylinder(d=3,h=10,$$fn=48);' >> $@
	echo '}' >> $@


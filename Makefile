#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := Env
SUFFIX := $(shell components/ESP32-RevK/buildsuffix)
MODELS := EnvMon2 EnvMon2-noco2 EnvMon2-nooled EnvMon2-flush EnvMon2-flush-nooled

all:
	@echo Make: $(PROJECT_NAME)$(SUFFIX).bin
	@idf.py build
	@cp build/$(PROJECT_NAME).bin $(PROJECT_NAME)$(SUFFIX).bin
	@echo Done: $(PROJECT_NAME)$(SUFFIX).bin

tools: envlog envgraph taspowerlog taspowersvg

set:    wroom pico

pico:
	components/ESP32-RevK/setbuildsuffix -S1-PICO-SSD1351
	@make

wroom:
	components/ESP32-RevK/setbuildsuffix -S1-SSD1351
	@make

solo:
	components/ESP32-RevK/setbuildsuffix -S1-SOLO-SSD1351
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

scad:	$(patsubst %,KiCad/%.scad,$(MODELS))
stl:	$(patsubst %,KiCad/%.stl,$(MODELS))

%.stl: %.scad
	echo "Making $@"
	/Applications/OpenSCAD.app/Contents/MacOS/OpenSCAD $< -o $@
	echo "Made $@"

KiCad/EnvMon2.scad: KiCad/EnvMon2.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=10.4 --base=2 --ignore=D3,M1.1

KiCad/EnvMon2-noco2.scad: KiCad/EnvMon2.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=10.4 --base=2 --ignore=D3,M1.1 --edge2

KiCad/EnvMon2-nooled.scad: KiCad/EnvMon2.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=5 --base=2 --ignore=M1 --edge2

KiCad/EnvMon2-flush.scad: KiCad/EnvMon2.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=10.4 --base=2 --ignore=D3,M1.1 --spacing=100
	@echo 'difference(){translate([spacing+86-4,-24,0])rotate([0,0,90])' >> $@
	@cat PCBCase/models/blankplate.scad >> $@
	@echo 'translate([spacing,0,0])translate([casewall,pcblength+casewall,height])rotate([180,0,0])hull()pcb(height+1,0);' >> $@
	@echo '}' >> $@

KiCad/EnvMon2-flush-nooled.scad: KiCad/EnvMon2.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=7 --base=2 --ignore=M1 --spacing=100
	@echo 'difference(){translate([spacing-6,-8,0])' >> $@
	@cat PCBCase/models/blankplate.scad >> $@
	@echo 'translate([spacing,0,0])translate([casewall,pcblength+casewall,height])rotate([180,0,0])hull()pcb(height+1,0);' >> $@
	@echo '}' >> $@

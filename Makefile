#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := EnvMon

include $(IDF_PATH)/make/project.mk

update:
	git submodule update --init --remote --merge
	git commit -a -m "Library update"

# Set GPIO low (whichever CBUS is set to mode 8/GPIO)
bootmode: ftdizap/ftdizap
	./ftdizap/ftdizap --cbus=0

# Flash with GPIO control
zap:    bootmode flash
	./ftdizap/ftdizap --cbus=1 --reset

# Program the FTDI
ftdi: ftdizap/ftdizap
	./ftdizap/ftdizap --serial="RevK" --description="ESP32-EnvMon" --cbus0-mode=8 --cbus1-mode=4 --cbus2-mode=13 --cbus3-mode=5

ftdizap/ftdizap: ftdizap/ftdizap.c
	make -C ftdizap

SQLlib/sqllib.o: SQLlib/sqllib.c
	make -C SQLlib
AXL/axl.o: AXL/axl.c
	make -C AXL

SQLINC=$(shell mariadb_config --include)
SQLLIB=$(shell mariadb_config --libs)
SQLVER=$(shell mariadb_config --version | sed 'sx\..*xx')
CCOPTS=${SQLINC} -I. -I/usr/local/ssl/include -D_GNU_SOURCE -g -Wall -funsigned-char -lm
OPTS=-L/usr/local/ssl/lib ${SQLLIB} ${CCOPTS}

envlog: envlog.c SQLlib/sqllib.o
	cc -O -o $@ $< ${OPTS} -lpopt -lmosquitto -ISQLlib SQLlib/sqllib.o

envgraph: envgraph.c SQLlib/sqllib.o AXL/axl.o
	cc -O -o $@ $< ${OPTS} -lpopt -lmosquitto -ISQLlib SQLlib/sqllib.o -IAXL AXL/axl.o -lcurl

PCBCase/case: PCBCase/case.c
	make -C PCBCase

case: KiCad/EnvMon.scad KiCad/EnvMon-noco2.scad KiCad/EnvMon-nooled.scad KiCad/EnvMon-flush.scad KiCad/EnvMon-flush-noco2.scad

KiCad/EnvMon.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=8 --top=10.2 --ignore=M3.2,M2.1

KiCad/EnvMon-noco2.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=2.5 --top=10.2 --ignore=M2.1,M3

KiCad/EnvMon-nooled.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=8 --top=5.5 --ignore=M3.2,M2

KiCad/EnvMon-flush.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=8 --top=10.2 --ignore=M2.1,M3.2 --spacing=80
	echo "translate([62,-23.75,0])difference(){" >> $@
	cat PCBCase/models/blankplate.scad >> $@
	echo "translate([20,25.75,-1])cube([46,38,10]);" >> $@
	echo 'for(x=[12.85:10.05:73.15])translate([x,15,-1])cylinder(d=3,h=10,$$fn=48);' >> $@
	echo "}" >> $@

KiCad/EnvMon-flush-noco2.scad: KiCad/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --base=2.5 --top=10.2 --ignore=M2.1,M3 --spacing=80
	echo "translate([62,-23.75,0])difference(){" >> $@
	cat PCBCase/models/blankplate.scad >> $@
	echo "translate([20,25.75,-1])cube([46,38,10]);" >> $@
	echo "}" >> $@



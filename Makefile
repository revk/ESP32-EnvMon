#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := Env

include $(IDF_PATH)/make/project.mk

update:
	git submodule update --init --remote --merge
	git commit -a -m "Library update"

bootmode: ftdizap/ftdizap
	./ftdizap/ftdizap --cbus0=0

zap:    bootmode flash
	./ftdizap/ftdizap --cbus0=1 --reset

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



#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := Env
SUFFIX := $(shell components/ESP32-RevK/buildsuffix)

all:	
	@echo Make: $(PROJECT_NAME)$(SUFFIX).bin
	@idf.py build
	@cp build/$(PROJECT_NAME).bin $(PROJECT_NAME)$(SUFFIX).bin
	@echo Done: $(PROJECT_NAME)$(SUFFIX).bin

tools: envlog envgraph taspowerlog taspowersvg taspowerse glowlog

main/icons.h: $(patsubst %.svg,%.h,$(wildcard icons/*.svg))
	cat icons/*.h > main/icons.h

icons/%.png:	icons/%.svg
	inkscape --export-background=WHITE --export-type=png --export-filename=$@ $<

icons/%.gray:	icons/%.png
	convert $< -resize 32x32 -depth 8 -grayscale average -negate $@

icons/%.h:	icons/%.gray
	echo "const uint8_t icon_$(patsubst icons/%.h,%,$@)[]={" > $@
	od -Anone -tx1 -v -w64 $< | sed 's/ \(.\). \(.\)./0x\1\2,/g' >> $@
	echo "};" >> $@

issue:  
	-git pull
	-git submodule update --recursive
	-git commit -a -m checkpoint
	@make set
	cp Env*.bin release
	git commit -a -m release
	git push

set:    wroom wroom-mono wroom-blind pico-blind pico s3

s3:
	components/ESP32-RevK/setbuildsuffix -S3-MINI-N4-R2-SSD1351
	@make

pico:
	components/ESP32-RevK/setbuildsuffix -S1-PICO-SSD1351
	@make

pico-blind:
	components/ESP32-RevK/setbuildsuffix -S1-PICO-GFXNONE
	@make

wroom:
	components/ESP32-RevK/setbuildsuffix -S1-V0-SSD1351
	@make

wroom-blind:
	components/ESP32-RevK/setbuildsuffix -S1-V0-GFXNONE
	@make

wroom-mono:
	components/ESP32-RevK/setbuildsuffix -S1-V0-SSD1680
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
	-git commit -a -m "Library update"


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

taspowerse: taspowerse.c SQLlib/sqllib.o AJL/ajlcurl.o
	cc -O -o $@ $< -lpopt -ISQLlib SQLlib/sqllib.o -IAJL AJL/ajlcurl.o -lcurl ${OPTS}

taspowersvg: taspowersvg.c SQLlib/sqllib.o AXL/axl.o
	cc -O -o $@ $< -lpopt -lmosquitto -ISQLlib SQLlib/sqllib.o -IAXL AXL/axl.o -lcurl ${OPTS}

glowlog: glowlog.c SQLlib/sqllib.o AJL/ajl.o
	cc -O -o $@ $< -lpopt -lmosquitto -ISQLlib SQLlib/sqllib.o -IAJL AJL/ajl.o ${OPTS}

all:	png stl

stl:	USBC/USBC.stl EnvHT/EnvHT.stl EnvMon/EnvMon.stl EnvMon/EnvMon-noco2.stl EnvMon/EnvMon-nooled.stl EnvMon/EnvMon-flush.stl EnvMon/EnvMon-flush-nooled.stl EnvMon/EnvMon-flush-noco2.scad Env2/EnvMon.stl

PCBCase/clean: PCBCase/clean.c
	make -C PCBCase

PCBCase/case: PCBCase/case.c
	make -C PCBCase

%.png:	%.kicad_pcb PCBCase/clean PCBCase/render Makefile
	PCBCase/render $<

png:	$(patsubst %.kicad_pcb,%.png,$(wildcard */*.kicad_pcb))

%.stl: %.scad
	echo "Making $@"
	/Applications/OpenSCAD.app/Contents/MacOS/OpenSCAD $< -o $@
	echo "Made $@"

USBC/USBC.scad: USBC/USBC.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=7 --bottom=1 --pcb=2

EnvHT/EnvHT.scad: EnvHT/EnvHT.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=6 --bottom=1 --ignore=D3,M1.1 --pcb=2 --case=3

EnvMon/EnvMon.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=10.4 --bottom=2 --ignore=D3,M1.1

EnvMon/EnvMon-noco2.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=10.4 --bottom=2 --ignore=D3,M1.1 --case=2

EnvMon/EnvMon-nooled.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=7.5 --bottom=2 --ignore=M1,SW1,SW2,SW3 --case=1

EnvMon/EnvMon-flush.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=10.4 --bottom=2 --ignore=D3,M1.1 --spacing=100
	@echo 'difference(){translate([spacing+86-4,-24,0])rotate([0,0,90])' >> $@
	@cat PCBCase/models/blankplate.scad >> $@
	@echo 'translate([spacing,0,0])translate([casewall,pcblength+casewall,height])rotate([180,0,0])hull()pcb(height+1,0);' >> $@
	@echo '}' >> $@

EnvMon/EnvMon-flush-noco2.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=10.4 --bottom=2 --ignore=D3,M1.1,SW1,SW2,SW3 --spacing=100 --case=1
	@echo 'difference(){translate([spacing+86-4,-24,0])rotate([0,0,90])' >> $@
	@cat PCBCase/models/blankplate.scad >> $@
	@echo 'translate([spacing,0,0])translate([15,pcblength+casewall,height])rotate([180,0,0])cube([45,37,height+1]);' >> $@
	@echo '}' >> $@

EnvMon/EnvMon-flush-nooled.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=7 --bottom=2 --ignore=M1,SW1,SW2,SW3 --spacing=100
	@echo 'difference(){translate([spacing-6,-8,0])' >> $@
	@cat PCBCase/models/blankplate.scad >> $@
	@echo 'translate([spacing,0,0])translate([casewall,pcblength+casewall,height])rotate([180,0,0])hull()pcb(height+1,0);' >> $@
	@echo '}' >> $@

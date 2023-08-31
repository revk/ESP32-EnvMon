stl:	EnvHT/EnvHT.stl EnvMon/EnvMon.stl EnvMon/EnvMon-noco2.stl EnvMon/EnvMon-nooled.stl EnvMon/EnvMon-flush.stl EnvMon/EnvMon-flush-nooled.stl

PCBCase/case: PCBCase/case.c
	make -C PCBCase

%.stl: %.scad
	echo "Making $@"
	/Applications/OpenSCAD.app/Contents/MacOS/OpenSCAD $< -o $@
	echo "Made $@"

EnvHT/EnvHT.scad: EnvHT/EnvHT.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=6 --base=1 --ignore=D3,M1.1 --pcb=2 --case=3

EnvMon/EnvMon.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=10.4 --base=2 --ignore=D3,M1.1

EnvMon/EnvMon-noco2.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=10.4 --base=2 --ignore=D3,M1.1 --edge2

EnvMon/EnvMon-nooled.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=7.5 --base=2 --ignore=M1,SW1,SW2,SW3 --edge1

EnvMon/EnvMon-flush.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=10.4 --base=2 --ignore=D3,M1.1 --spacing=100
	@echo 'difference(){translate([spacing+86-4,-24,0])rotate([0,0,90])' >> $@
	@cat PCBCase/models/blankplate.scad >> $@
	@echo 'translate([spacing,0,0])translate([casewall,pcblength+casewall,height])rotate([180,0,0])hull()pcb(height+1,0);' >> $@
	@echo '}' >> $@

EnvMon/EnvMon-flush-nooled.scad: EnvMon/EnvMon.kicad_pcb PCBCase/case Makefile
	PCBCase/case -o $@ $< --top=7 --base=2 --ignore=M1,SW1,SW2,SW3 --spacing=100
	@echo 'difference(){translate([spacing-6,-8,0])' >> $@
	@cat PCBCase/models/blankplate.scad >> $@
	@echo 'translate([spacing,0,0])translate([casewall,pcblength+casewall,height])rotate([180,0,0])hull()pcb(height+1,0);' >> $@
	@echo '}' >> $@

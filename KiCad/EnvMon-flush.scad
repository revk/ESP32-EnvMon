// Generated case design for KiCad/EnvMon.kicad_pcb
// By https://github.com/revk/PCBCase
// Generated 2022-03-09 15:37:28
// title:	EnvMon
// date:	${DATE}
// rev:	4
// comment:	www.me.uk
// comment:	@TheRealRevK
// comment:	Adrian Kennard Andrews & Arnold Ltd
//

// Globals
margin=0.500000;
overlap=2.000000;
casebase=8.000000;
casetop=10.400000;
casewall=3.000000;
fit=0.000000;
edge=1.000000;
pcbthickness=1.600000;
nohull=false;
hullcap=1.000000;
useredge=false;

module pcb(h=pcbthickness){linear_extrude(height=h)polygon(points=[[0.000000,37.000000],[11.250000,37.000000],[11.250000,30.900000],[39.250000,30.900000],[39.250000,37.000000],[44.500000,37.000000],[44.500000,0.000000],[0.000000,0.000000]],paths=[[0,1,2,3,4,5,6,7,0]]);}

module outline(h=pcbthickness){linear_extrude(height=h)polygon(points=[[0.000000,37.000000],[11.250000,37.000000],[11.250000,30.900000],[39.250000,30.900000],[39.250000,37.000000],[44.500000,37.000000],[44.500000,0.000000],[0.000000,0.000000]],paths=[[0,1,2,3,4,5,6,7,0]]);}
spacing=80.000000;
pcbwidth=44.500000;
pcblength=37.000000;
// Populated PCB
module board(pushed=false,hulled=false){
translate([9.750000,0.005000,1.600000])translate([0.000000,3.385000,0.000000])rotate([-90.000000,0.000000,0.000000])m0(pushed,hulled); // RevK:USC16-TR-Round CSP-USC16-TR
translate([27.800000,6.850000,1.600000])translate([0.000000,-3.600000,2.500000])rotate([0.000000,0.000000,180.000000])m1(pushed,hulled); // RevK:Molex_MiniSPOX_H8RA 22057085
translate([44.500000,37.000000,1.600000])rotate([0,0,180.000000])translate([42.000000,10.840000,6.500000])rotate([180.000000,0.000000,0.000000])m2(pushed,hulled); // RevK:OLED1.5-RGB PinHeader_1x07_P2.54mm_Vertical
translate([44.500000,37.000000,1.600000])rotate([0,0,180.000000])translate([44.500000,37.000000,0.000000])rotate([0.000000,0.000000,180.000000])m3(pushed,hulled); // RevK:OLED1.5-RGB OLED15
translate([9.975000,11.875000,1.600000])m4(pushed,hulled); // RevK:QFN-20-1EP_4x4mm_P0.5mm_EP2.5x2.5mm QFN-20-1EP_4x4mm_P0.5mm_EP2.5x2.5mm
translate([3.300000,7.500000,1.600000])rotate([0,0,-90.000000])m5(pushed,hulled); // Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder R_0603_1608Metric
translate([1.800000,7.500000,1.600000])rotate([0,0,-90.000000])m5(pushed,hulled); // Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder R_0603_1608Metric
translate([9.200000,35.300000,1.600000])rotate([0,0,180.000000])translate([0.000000,-0.400000,0.000000])m6(pushed,hulled); // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric
translate([9.200000,35.300000,1.600000])rotate([0,0,180.000000])translate([0.000000,0.400000,0.000000])m6(pushed,hulled); // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric
translate([9.200000,31.100000,1.600000])rotate([0,0,180.000000])m5(pushed,hulled); // Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder R_0603_1608Metric
translate([9.200000,33.500000,1.600000])rotate([0,0,180.000000])m5(pushed,hulled); // Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder R_0603_1608Metric
translate([9.200000,32.300000,1.600000])rotate([0,0,180.000000])m5(pushed,hulled); // Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder R_0603_1608Metric
translate([4.900000,9.000000,1.600000])rotate([0,0,90.000000])m7(pushed,hulled); // RevK:C_0603 C_0603_1608Metric
translate([7.400000,21.500000,1.600000])rotate([0.000000,0.000000,90.000000])m8(pushed,hulled); // RevK:RegulatorBlockFB SOT-23-6
translate([7.400000,21.500000,1.600000])translate([-2.100000,-0.250000,0.000000])rotate([0.000000,0.000000,90.000000])m7(pushed,hulled); // RevK:C_0603 C_0603_1608Metric
translate([7.400000,21.500000,1.600000])translate([-0.800000,-3.550000,0.000000])rotate([0.000000,0.000000,90.000000])m9(pushed,hulled); // RevK:RegulatorBlockFB C_0805_2012Metric
translate([7.400000,21.500000,1.600000])translate([5.400000,-3.550000,0.000000])rotate([0.000000,0.000000,90.000000])m9(pushed,hulled); // RevK:RegulatorBlockFB C_0805_2012Metric
translate([7.400000,21.500000,1.600000])translate([3.900000,0.000000,0.000000])rotate([0.000000,0.000000,90.000000])m10(pushed,hulled); // RevK:RegulatorBlockFB TYA4020
translate([7.400000,21.500000,1.600000])translate([2.275000,-3.200000,0.000000])m11(pushed,hulled); // RevK:RegulatorBlockFB D_1206_3216Metric
translate([4.900000,25.200000,1.600000])rotate([0,0,-90.000000])m5(pushed,hulled); // Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder R_0603_1608Metric
translate([6.200000,25.200000,1.600000])rotate([0,0,-90.000000])m5(pushed,hulled); // Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder R_0603_1608Metric
translate([4.900000,12.800000,1.600000])rotate([0,0,-90.000000])m11(pushed,hulled); // RevK:RegulatorBlockFB D_1206_3216Metric
translate([25.250000,21.000000,1.600000])m12(pushed,hulled); // RevK:ESP32-WROOM-32 ESP32-WROOM-32
translate([40.200000,30.600000,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])translate([11.500000,17.500000,5.500000])rotate([90.000000,0.000000,90.000000])m13(pushed,hulled); // RevK:SCD30-4pin SCD30
translate([40.200000,30.600000,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])translate([1.400000,33.600000,4.000000])rotate([0.000000,180.000000,0.000000])m14(pushed,hulled); // RevK:SCD30-4pin PinHeader_1x04_P2.54mm_Vertical
translate([21.550000,3.050000,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])m15(pushed,hulled); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder R_1206_3216Metric
translate([24.050000,3.050000,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])m15(pushed,hulled); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder R_1206_3216Metric
translate([26.550000,3.050000,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])m15(pushed,hulled); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder R_1206_3216Metric
translate([29.050000,3.050000,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])m15(pushed,hulled); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder R_1206_3216Metric
translate([31.550000,3.050000,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])m15(pushed,hulled); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder R_1206_3216Metric
translate([34.050000,3.050000,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])m15(pushed,hulled); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder R_1206_3216Metric
translate([36.550000,3.050000,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])m15(pushed,hulled); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder R_1206_3216Metric
translate([16.025000,4.000000,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])m16(pushed,hulled); // Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical PinHeader_1x02_P2.54mm_Vertical
}

module b(cx,cy,z,w,l,h){translate([cx-w/2,cy-l/2,z])cube([w,l,h]);}
module m0(pushed=false,hulled=false)
{ // RevK:USC16-TR-Round CSP-USC16-TR
rotate([90,0,0])translate([-4.47,-3.84,0])
{
	translate([1.63,0,1.63])
	rotate([-90,0,0])
	hull()
	{
		cylinder(d=3.26,h=7.75,$fn=24);
		translate([5.68,0,0])
		cylinder(d=3.26,h=7.75,$fn=24);
	}
	translate([0,6.65,0])cube([8.94,1.1,1.63]);
	translate([0,2.2,0])cube([8.94,1.6,1.63]);
	// Plug
	translate([1.63,-20,1.63])
	rotate([-90,0,0])
	hull()
	{
		cylinder(d=2.5,h=21,$fn=24);
		translate([5.68,0,0])
		cylinder(d=2.5,h=21,$fn=24);
	}
	translate([1.63,-22.5,1.63])
	rotate([-90,0,0])
	hull()
	{
		cylinder(d=7,h=21,$fn=24);
		translate([5.68,0,0])
		cylinder(d=7,h=21,$fn=24);
	}
}

}

module m1(pushed=false,hulled=false)
{ // RevK:Molex_MiniSPOX_H8RA 22057085
N=8;
A=2.4+N*2.5;
rotate([0,0,180])
translate([-A/2,-2.94,-2.5])
{
	cube([A,4.9,4.9]);
	cube([A,5.9,3.9]);
	hull()
	{
		cube([A,7.4,1]);
		cube([A,7.9,0.5]);
	}
	translate([1,6,-2])cube([A-2,1.2,4.5]); // Assumes cropped pins
	// Plug
	translate([0.5,-20,0.6])cube([A-1,21,4.1]);
	translate([0,-23,0])cube([A,20,4.9]);
}

}

module m2(pushed=false,hulled=false)
{ // RevK:OLED1.5-RGB PinHeader_1x07_P2.54mm_Vertical
translate([-1.27,-16.51,-3])cube([2.54,17.78,hulled?12.7:100]);

}

module m3(pushed=false,hulled=false)
{ // RevK:OLED1.5-RGB OLED15
translate([44.5,37,0])rotate([0,0,180])
{
	translate([0,0,6])hull()for(x=[1.5,44.5-1.5])for(y=[1.5,37-1.5])translate([x,y,0])cylinder(r=1.4995,h=1.599,$fn=24);
	for(x=[2.5,44.5-2.5])for(y=[2.5,37-2.5])translate([x,y,0])cylinder(d=4.98,h=6,$fn=6);
	for(x=[2.5,44.5-2.5])for(y=[2.5,37-2.5])translate([x,y,7])cylinder(d=4.99,h=1.6,$fn=24);
	for(x=[2.5,44.5-2.5])for(y=[2.5,37-2.5])translate([x,y,-2.6])cylinder(d=4.99,h=1.6,$fn=24);
	translate([5.25,0,6])cube([34,37,3.2]);
	translate([40.73,9.61,7.5])cube([2.54,7*2.54,1.5]); // pins
	if(!hulled)hull()
	{
		translate([8.25,2,9.199]) cube([28,28,20]);
		if(!pushed) translate([4.25,-2,13.5]) cube([36,36,20]);
	}
}
}

module m4(pushed=false,hulled=false)
{ // RevK:QFN-20-1EP_4x4mm_P0.5mm_EP2.5x2.5mm QFN-20-1EP_4x4mm_P0.5mm_EP2.5x2.5mm
cube([4,4,1],center=true);
}

module m5(pushed=false,hulled=false)
{ // Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder R_0603_1608Metric
b(0,0,0,2.8,0.95,0.5); // Pad size
}

module m6(pushed=false,hulled=false)
{ // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric
b(0,0,0,1.6,0.8,0.25);
b(0,0,0,1.2,0.8,0.55);
b(0,0,0,0.8,0.8,0.95);
if(pushed)b(0,0,0,1,1,20);
}

module m7(pushed=false,hulled=false)
{ // RevK:C_0603 C_0603_1608Metric
b(0,0,0,2.8,0.95,1); // Pad size
}

module m8(pushed=false,hulled=false)
{ // RevK:RegulatorBlockFB SOT-23-6
b(0,0,0,3.05,3.05,0.5);
b(0,0,0,1.45,3.05,1.1);
}

module m9(pushed=false,hulled=false)
{ // RevK:RegulatorBlockFB C_0805_2012Metric
b(0,0,0,3.2,1.45,1); // Pad size
}

module m10(pushed=false,hulled=false)
{ // RevK:RegulatorBlockFB TYA4020
b(0,0,0,4,4,2.1);
}

module m11(pushed=false,hulled=false)
{ // RevK:RegulatorBlockFB D_1206_3216Metric
b(0,0,0,4.4,1.75,1.2); // Pad size
}

module m12(pushed=false,hulled=false)
{ // RevK:ESP32-WROOM-32 ESP32-WROOM-32
translate([-9,-9.75,0])
{
	cube([18,25.50,0.80]);
	translate([1.1,1.05,0])cube([15.80,17.60,3.10]);
	if(!hulled)translate([-0.5,0.865,0])cube([19,17.4,0.8]);
	translate([2.65,-0.5,0])cube([12.7,2,0.8]);

}
}

module m13(pushed=false,hulled=false)
{ // RevK:SCD30-4pin SCD30
rotate([-90,0,0])
rotate([0,0,-90])
translate([-11.5,-17.5,-5.5])
{
	translate([0,0,4])cube([23,35,1.6]); // Board
	translate([0.1,1.5,0])cube([22.8,33.4,7]); // Parts
	translate([0,24.92,-4])cube([2.54,10.08,11]); // Header
	// Hole for air
	if(!pushed)for(x=[-5,5])translate([14.5+x,-134.9,3.5])rotate([-90,0,0])cylinder(h=234.9,d=2,$fn=24);
}

}

module m14(pushed=false,hulled=false)
{ // RevK:SCD30-4pin PinHeader_1x04_P2.54mm_Vertical
translate([-1.27,-8.89,-3])
cube([2.54,10.16,100]);

}

module m15(pushed=false,hulled=false)
{ // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder R_1206_3216Metric
b(0,0,0,4.85,1.75,0.5); // Pad size
}

module m16(pushed=false,hulled=false)
{ // Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical PinHeader_1x02_P2.54mm_Vertical
translate([-1.27,-3.81,-3])cube([2.54,5.08,hulled?12.7:100]);

}

height=casebase+pcbthickness+casetop;

module boardh(pushed=false)
{ // Board with hulled parts
	union()
	{
		if(!nohull)intersection()
		{
			translate([0,0,hullcap-casebase])outline(casebase+pcbthickness+casetop-hullcap*2);
			hull()board(pushed,true);
		}
		board(pushed,false);
		pcb();
	}
}

module boardf()
{ // This is the board, but stretched up to make a push out in from the front
	render()
	{
		intersection()
		{
			translate([-casewall-1,-casewall-1,-casebase-1]) cube([pcbwidth+casewall*2+2,pcblength+casewall*2+2,height+2]);
			minkowski()
			{
				boardh(true);
				cylinder(h=height+100,d=margin,$fn=8);
			}
		}
	}
}

module boardb()
{ // This is the board, but stretched down to make a push out in from the back
	render()
	{
		intersection()
		{
			translate([-casewall-1,-casewall-1,-casebase-1]) cube([pcbwidth+casewall*2+2,pcblength+casewall*2+2,height+2]);
			minkowski()
			{
				boardh(true);
				translate([0,0,-height-100])
				cylinder(h=height+100,d=margin,$fn=8);
			}
		}
	}
}

module boardm()
{
	render()
	{
 		minkowski()
 		{
			translate([0,0,-margin/2])cylinder(d=margin,h=margin,$fn=8);
 			boardh(false);
		}
		intersection()
    		{
        		translate([0,0,-(casebase-1)])pcb(pcbthickness+(casebase-1)+(casetop-1));
        		translate([0,0,-(casebase-1)])outline(pcbthickness+(casebase-1)+(casetop-1));
			boardh(false);
    		}
 	}
}

module pcbh()
{ // PCB shape for case
	if(useredge)outline();
	else hull()outline();
}

module pyramid()
{ // A pyramid
 polyhedron(points=[[0,0,0],[-height,-height,height],[-height,height,height],[height,height,height],[height,-height,height]],faces=[[0,1,2],[0,2,3],[0,3,4],[0,4,1],[4,3,2,1]]);
}

module wall(d=0)
{ // The case wall
    	translate([0,0,-casebase-1])
    	minkowski()
    	{
    		pcbh();
	        cylinder(d=margin+d*2,h=height+2-pcbthickness,$fn=8);
   	}
}

module cutf()
{ // This cut up from base in the wall
	intersection()
	{
		boardf();
		difference()
		{
			translate([-casewall+0.01,-casewall+0.01,-casebase+0.01])cube([pcbwidth+casewall*2-0.02,pcblength+casewall*2-0.02,casebase+overlap]);
			wall();
			boardb();
		}
	}
}

module cutb()
{ // The cut down from top in the wall
	intersection()
	{
		boardb();
		difference()
		{
			translate([-casewall+0.01,-casewall+0.01,0.01])cube([pcbwidth+casewall*2-0.02,pcblength+casewall*2-0.02,casetop+pcbthickness]);
			wall();
			boardf();
		}
	}
}

module cutpf()
{ // the push up but pyramid
	render()
	intersection()
	{
		minkowski()
		{
			pyramid();
			cutf();
		}
		difference()
		{
			translate([-casewall-0.01,-casewall-0.01,-casebase-0.01])cube([pcbwidth+casewall*2+0.02,pcblength+casewall*2+0.02,casebase+overlap+0.02]);
			wall();
			boardh(true);
		}
		translate([-casewall,-casewall,-casebase])case();
	}
}

module cutpb()
{ // the push down but pyramid
	render()
	intersection()
	{
		minkowski()
		{
			scale([1,1,-1])pyramid();
			cutb();
		}
		difference()
		{
			translate([-casewall-0.01,-casewall-0.01,-0.01])cube([pcbwidth+casewall*2+0.02,pcblength+casewall*2+0.02,casetop+pcbthickness+0.02]);
			wall();
			boardh(true);
		}
		translate([-casewall,-casewall,-casebase])case();
	}
}


module case()
{ // The basic case
        minkowski()
        {
            pcbh();
            hull()
		{
			translate([edge,0,edge])
			cube([casewall*2-edge*2,casewall*2,height-edge*2-pcbthickness]);
			translate([0,edge,edge])
			cube([casewall*2,casewall*2-edge*2,height-edge*2-pcbthickness]);
			translate([edge,edge,0])
			cube([casewall*2-edge*2,casewall*2-edge*2,height-pcbthickness]);
		}
        }
}

module cut(d=0)
{ // The cut point in the wall
	minkowski()
	{
        	pcbh();
		hull()
		{
			translate([casewall/2-d/2-margin/4+casewall/3,casewall/2-d/2-margin/4,casebase])
				cube([casewall+d+margin/2-2*casewall/3,casewall+d+margin/2,casetop+pcbthickness+1]);
			translate([casewall/2-d/2-margin/4,casewall/2-d/2-margin/4+casewall/3,casebase])
				cube([casewall+d+margin/2,casewall+d+margin/2-2*casewall/3,casetop+pcbthickness+1]);
		}
	}
}

module base()
{ // The base
	difference()
	{
		case();
		difference()
		{
			union()
			{
				translate([-1,-1,casebase+overlap])cube([pcbwidth+casewall*2+2,pcblength+casewall*2+2,casetop+1]);
				cut(fit);
			}
		}
		translate([casewall,casewall,casebase])boardf();
		translate([casewall,casewall,casebase])boardm();
		translate([casewall,casewall,casebase])cutpf();
	}
	translate([casewall,casewall,casebase])cutpb();
}

module top()
{
	translate([0,pcblength+casewall*2,height])rotate([180,0,0])
	{
		difference()
		{
			case();
			difference()
			{
				translate([-1,-1,-1])cube([pcbwidth+casewall*2+2,pcblength+casewall*2+2,casebase+overlap-margin+1]);
				cut(-fit);
			}
			translate([casewall,casewall,casebase])boardb();
			translate([casewall,casewall,casebase])boardm();
			translate([casewall,casewall,casebase])cutpb();
		}
		translate([casewall,casewall,casebase])cutpf();
	}
}

module test()
{
	translate([0*spacing,0,0])base();
	translate([1*spacing,0,0])top();
	translate([2*spacing,0,0])pcb();
	translate([3*spacing,0,0])outline();
	translate([4*spacing,0,0])wall();
	translate([5*spacing,0,0])board();
	translate([6*spacing,0,0])board(false,true);
	translate([7*spacing,0,0])board(true);
	translate([8*spacing,0,0])boardh();
	translate([9*spacing,0,0])boardf();
	translate([10*spacing,0,0])boardb();
	translate([11*spacing,0,0])cutpf();
	translate([12*spacing,0,0])cutpb();
	translate([13*spacing,0,0])cutf();
	translate([14*spacing,0,0])cutb();
	translate([15*spacing,0,0])case();
}

module parts()
{
	base();
	translate([spacing,0,0])top();
}
base(); translate([spacing,0,0])top();
translate([62,-23.75,0])difference(){
difference()
{
	union()
	{
		difference()
		{
			hull()
			{
				translate([2,2,0])cube([82,82,1]);
				translate([0,0,7])cube([86,86,1]);
			}
			translate([3,3,2])cube([80,80,10]);
		}
		for(x=[12.85,73.15])translate([x,43,0.5])cylinder(d1=10,d2=8,h=4.5,$fn=48);
	}
	for(x=[12.85,73.15])translate([x,43,-0.01])
	{
		cylinder(d=4,h=10,$fn=48);
		cylinder(d=7.5,h=1,$fn=48);
		translate([0,0,0.999])cylinder(d1=7.5,d2=0,h=3.275,$fn=48);
	}
}
translate([50,10,5])rotate([0,90,0])hull(){cylinder(d=4,h=100,$fn=48);translate([-10,0,0])cylinder(d=4,h=100,$fn=48);}
translate([20,25.75,-1])cube([46,38,10]);
for(x=[12.85:10.05:73.15])translate([x,15,-1])cylinder(d=3,h=10,$fn=48);
}

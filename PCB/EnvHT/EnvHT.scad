// Generated case design for PCB/EnvHT/EnvHT.kicad_pcb
// By https://github.com/revk/PCBCase
// Generated 2023-05-03 11:14:25
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
lip=0.000000;
casebase=1.000000;
casetop=6.000000;
casewall=3.000000;
fit=0.000000;
edge=1.000000;
pcbthickness=0.800000;
nohull=false;
hullcap=1.000000;
hulledge=1.000000;
useredge=true;

module outline(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[3.499999,18.500000],[42.500001,18.500000],[42.998103,18.464375],[43.486065,18.358225],[43.953953,18.183712],[44.392243,17.944387],[44.792013,17.645123],[45.145124,17.292013],[45.444388,16.892243],[45.683713,16.453953],[45.858226,15.986065],[45.964376,15.498103],[46.000001,15.000001],[46.000001,11.000001],[45.964376,10.501899],[45.858226,10.013937],[45.683713,9.546049],[45.444388,9.107759],[45.145124,8.707989],[44.792014,8.354878],[44.392244,8.055614],[43.953954,7.816289],[43.486066,7.641776],[42.998104,7.535626],[42.500002,7.500001],[38.000000,7.500000],[38.000000,0.000000],[8.000000,0.000000],[8.000000,7.500000],[3.500000,7.500001],[3.001898,7.535626],[2.513936,7.641776],[2.046048,7.816289],[1.607758,8.055614],[1.207988,8.354878],[0.854877,8.707988],[0.555613,9.107758],[0.316288,9.546048],[0.141775,10.013936],[0.035625,10.501898],[0.000000,11.000000],[0.000000,15.000000],[0.035625,15.498102],[0.141775,15.986064],[0.316288,16.453952],[0.555613,16.892242],[0.854877,17.292012],[1.207987,17.645123],[1.607757,17.944387],[2.046047,18.183712],[2.513935,18.358225],[3.001897,18.464375]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51]]);}

module pcb(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[35.800000,18.200000],[36.800000,18.200000],[37.058819,18.165926],[37.300000,18.066026],[37.507107,17.907107],[37.666026,17.700000],[37.765926,17.458819],[37.800000,17.200000],[37.800000,0.000000],[8.300000,0.000000],[8.300000,17.700000],[8.338061,17.891342],[8.446447,18.053553],[8.608658,18.161939],[8.800000,18.200000],[32.900000,18.200000],[33.091342,18.161939],[33.253553,18.053553],[33.361939,17.891342],[33.400000,17.700000],[33.400000,7.100000],[33.546447,6.746447],[33.900000,6.600000],[35.650000,6.600000],[36.003553,6.746447],[36.150000,7.100000],[36.150000,13.150000],[35.857107,13.857107],[35.092893,14.542893],[34.933974,14.750000],[34.834073,14.991181],[34.800000,15.250000],[34.800000,17.200000],[34.834074,17.458819],[34.933974,17.700000],[35.092893,17.907107],[35.300000,18.066026],[35.541181,18.165926]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37]]);}
spacing=62.000001;
pcbwidth=46.000001;
pcblength=18.500000;
// Populated PCB
module board(pushed=false,hulled=false){
translate([9.800000,8.350000,0.800000])rotate([0,0,90.000000])m2(pushed,hulled); // RevK:C_0603 C_0603_1608Metric (back)
translate([26.250000,8.300000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([31.550000,10.750000,0.800000])m8(pushed,hulled); // RevK:C_0805_ C_0805_2012Metric (back)
translate([15.500000,3.250000,0.800000])rotate([0,0,-90.000000])m10(pushed,hulled,2); // RevK:PTSM-HH-2-RA PTSM-HH-2-RA (back)
translate([26.250000,7.050000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([28.000000,11.600000,0.800000])rotate([0,0,90.000000])rotate([-0.000000,-0.000000,-90.000000])m13(pushed,hulled); // RevK:SOT-23-Thin-6-Reg SOT-23-6 (back)
translate([10.500000,14.900000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([28.000000,15.500000,0.800000])rotate([0,0,90.000000])rotate([-0.000000,-0.000000,-90.000000])m16(pushed,hulled); // RevK:L_4x4_ TYA4020 (back)
translate([36.300000,15.550000,0.800000])rotate([0,0,90.000000])m19(pushed,hulled); // RevK:Sensirion_DFN-4_1.5x1.5mm_P0.5mm_SHT4x LX5563LL-TR (back)
translate([4.500000,13.000000,0.800000])translate([0.000000,0.000000,5.000000])rotate([-0.000000,90.000000,-0.000000])m21(pushed,hulled); // RevK:Screw M3.5CSK (back)
translate([28.250000,9.500000,0.800000])m2(pushed,hulled); // RevK:C_0603 C_0603_1608Metric (back)
translate([31.550000,17.000000,0.800000])m8(pushed,hulled); // RevK:C_0805_ C_0805_2012Metric (back)
translate([10.500000,13.350000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([31.170000,13.880000,0.800000])rotate([0,0,90.000000])m25(pushed,hulled); // RevK:D_1206_ D_1206_3216Metric (back)
translate([11.000000,8.350000,0.800000])rotate([0,0,90.000000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([41.500000,13.000000,0.800000])translate([0.000000,0.000000,5.000000])rotate([-0.000000,90.000000,-0.000000])m21(pushed,hulled); // RevK:Screw M3.5CSK (back)
translate([36.300000,17.200000,0.800000])rotate([0,0,180.000000])m2(pushed,hulled); // RevK:C_0603 C_0603_1608Metric (back)
translate([30.557500,3.250000,0.800000])rotate([0,0,90.000000])m10(pushed,hulled,2); // RevK:PTSM-HH-2-RA PTSM-HH-2-RA (back)
translate([18.250000,12.556750,0.800000])m26(pushed,hulled); // RevK:ESP32-PICO-MINI-02 ESP32-PICO-MINI-02 (back)
translate([10.500000,11.700000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([31.000000,7.750000,0.800000])m25(pushed,hulled); // RevK:D_1206_ D_1206_3216Metric (back)
}

module b(cx,cy,z,w,l,h){translate([cx-w/2,cy-l/2,z])cube([w,l,h]);}
module m2(pushed=false,hulled=false)
{ // RevK:C_0603 C_0603_1608Metric
b(0,0,0,1.6,0.95,0.2); // Pad size
b(0,0,0,1.6,0.8,1); // Chip
}

module m5(pushed=false,hulled=false)
{ // RevK:R_0603 R_0603_1608Metric
b(0,0,0,2.8,0.95,0.5); // Pad size
}

module m8(pushed=false,hulled=false)
{ // RevK:C_0805_ C_0805_2012Metric
b(0,0,0,2,1.45,0.2); // Pad size
b(0,0,0,2,1.2,1); // Chip
}

module m10(pushed=false,hulled=false,n=0)
{ // RevK:PTSM-HH-2-RA PTSM-HH-2-RA
// Socket
hull()
{
	b(0,-7.5/2+0.3,0,1.7+n*2.5,7.5,4);
	b(0,-7.5/2+0.3,0,1.7+n*2.5-2,7.5,5);
}
if(!hulled)
{
	// Plug
	b(0,-10.5/2-7.5+0.3,0,1.1+n*2.5,10.5,5);
	// Pins
	for(p=[0:n-1])translate([-2.5*(n-1)/2+p*2.5,0,-2.1])cylinder(r=0.3,h=2.1);
}
}

module m13(pushed=false,hulled=false)
{ // RevK:SOT-23-Thin-6-Reg SOT-23-6
b(0,0,0,3.05,3.05,0.5);
b(0,0,0,1.45,3.05,1.1);
}

module m16(pushed=false,hulled=false)
{ // RevK:L_4x4_ TYA4020
b(0,0,0,4,4,2.1);
}

module m19(pushed=false,hulled=false)
{ // RevK:Sensirion_DFN-4_1.5x1.5mm_P0.5mm_SHT4x LX5563LL-TR
b(0,0,0,1.5,1.5,0.5);
if(!pushed&&!hulled)cylinder(d=1.5,h=20);
}

module m21(pushed=false,hulled=false)
{ // RevK:Screw M3.5CSK
// Screw M3.5 countersunk
if(!hulled)
rotate([0,90,0],$fn=24)
{
	if(!pushed)
	{
		translate([0,0,-10])cylinder(d=6.5,h=10);
		cylinder(d1=6.5,d2=3.5,h=2);
	}
	cylinder(d=3.5,h=100);
}

}

module m25(pushed=false,hulled=false)
{ // RevK:D_1206_ D_1206_3216Metric
b(0,0,0,4.4,1.75,1.2); // Pad size
}

module m26(pushed=false,hulled=false)
{ // RevK:ESP32-PICO-MINI-02 ESP32-PICO-MINI-02
translate([-13.2/2,-16.6/2+2.7,0])
{
	if(!hulled)cube([13.2,16.6,0.8]);
	cube([13.2,11.2,2.4]);
}
}

height=casebase+pcbthickness+casetop;
$fn=48;

module boardh(pushed=false)
{ // Board with hulled parts
	union()
	{
		if(!nohull)intersection()
		{
			translate([0,0,hullcap-casebase])outline(casebase+pcbthickness+casetop-hullcap*2,-hulledge);
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
			union()
			{
				minkowski()
				{
					boardh(true);
					cylinder(h=height+100,d=margin,$fn=8);
				}
				board(false,false);
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
			union()
			{
				minkowski()
				{
					boardh(true);
					translate([0,0,-height-100])
					cylinder(h=height+100,d=margin,$fn=8);
				}
				board(false,false);
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
		//intersection()
    		//{
        		//translate([0,0,-(casebase-hullcap)])pcb(pcbthickness+(casebase-hullcap)+(casetop-hullcap));
        		//translate([0,0,-(casebase-hullcap)])outline(pcbthickness+(casebase-hullcap)+(casetop-hullcap));
			boardh(false);
    		//}
 	}
}

module pcbh(h=pcbthickness,r=0)
{ // PCB shape for case
	if(useredge)outline(h,r);
	else hull()outline(h,r);
}

module pyramid()
{ // A pyramid
 polyhedron(points=[[0,0,0],[-height,-height,height],[-height,height,height],[height,height,height],[height,-height,height]],faces=[[0,1,2],[0,2,3],[0,3,4],[0,4,1],[4,3,2,1]]);
}

module wall(d=0)
{ // The case wall
	translate([0,0,-casebase-d])
	{
		if(useredge)
			intersection()
			{
				pcb(height+d*2,margin/2+d);
				pcbh(height+d*2,margin/2+d);
			}
		else pcbh(height+d*2,margin/2+d);
	}
}

module cutf()
{ // This cut up from base in the wall
	intersection()
	{
		boardf();
		difference()
		{
			translate([-casewall+0.01,-casewall+0.01,-casebase+0.01])cube([pcbwidth+casewall*2-0.02,pcblength+casewall*2-0.02,casebase+overlap+lip]);
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
			translate([-casewall-0.01,-casewall-0.01,-casebase-0.01])cube([pcbwidth+casewall*2+0.02,pcblength+casewall*2+0.02,casebase+overlap+lip+0.02]);
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
	hull()
	{
		translate([casewall,casewall,0])pcbh(height,casewall-edge);
		translate([casewall,casewall,edge])pcbh(height-edge*2,casewall);
	}
}

module cut(d=0)
{ // The cut point in the wall
	translate([casewall,casewall,casebase+lip])pcbh(casetop+pcbthickness-lip+1,casewall/2+d/2+margin/4);
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
				translate([-1,-1,casebase+overlap+lip])cube([pcbwidth+casewall*2+2,pcblength+casewall*2+2,casetop+1]);
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
				translate([-1,-1,-1])cube([pcbwidth+casewall*2+2,pcblength+casewall*2+2,casebase+overlap+lip-margin+1]);
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

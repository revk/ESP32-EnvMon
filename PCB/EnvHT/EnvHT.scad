// Generated case design for PCB/EnvHT/EnvHT.kicad_pcb
// By https://github.com/revk/PCBCase
// Generated 2023-05-12 14:29:03
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

module outline(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[3.499999,18.600000],[42.500001,18.600000],[42.998103,18.564375],[43.486065,18.458225],[43.953953,18.283712],[44.392243,18.044387],[44.792013,17.745123],[45.145124,17.392013],[45.444388,16.992243],[45.683713,16.553953],[45.858226,16.086065],[45.964376,15.598103],[46.000001,15.100001],[46.000001,11.100001],[45.964376,10.601899],[45.858226,10.113937],[45.683713,9.646049],[45.444388,9.207759],[45.145124,8.807989],[44.792014,8.454878],[44.392244,8.155614],[43.953954,7.916289],[43.486066,7.741776],[42.998104,7.635626],[42.500002,7.600001],[38.000000,7.600000],[38.000000,0.100000],[8.000000,0.100000],[8.000000,7.600000],[3.500000,7.600001],[3.001898,7.635626],[2.513936,7.741776],[2.046048,7.916289],[1.607758,8.155614],[1.207988,8.454878],[0.854877,8.807988],[0.555613,9.207758],[0.316288,9.646048],[0.141775,10.113936],[0.035625,10.601898],[0.000000,11.100000],[0.000000,15.100000],[0.035625,15.598102],[0.141775,16.086064],[0.316288,16.553952],[0.555613,16.992242],[0.854877,17.392012],[1.207987,17.745123],[1.607757,18.044387],[2.046047,18.283712],[2.513935,18.458225],[3.001897,18.564375]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51]]);}

module pcb(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[35.800000,18.300000],[36.800000,18.300000],[37.058819,18.265926],[37.300000,18.166026],[37.507107,18.007107],[37.666026,17.800000],[37.765926,17.558819],[37.800000,17.300000],[37.800000,0.000000],[8.300000,0.000000],[8.300000,17.800000],[8.338061,17.991342],[8.446447,18.153553],[8.608658,18.261939],[8.800000,18.300000],[32.900000,18.300000],[33.091342,18.261939],[33.253553,18.153553],[33.361939,17.991342],[33.400000,17.800000],[33.400000,7.200000],[33.546447,6.846447],[33.900000,6.700000],[35.650000,6.700000],[36.003553,6.846447],[36.150000,7.200000],[36.150000,13.250000],[35.857107,13.957107],[35.092893,14.642893],[34.933974,14.850000],[34.834073,15.091181],[34.800000,15.350000],[34.800000,17.300000],[34.834074,17.558819],[34.933974,17.800000],[35.092893,18.007107],[35.300000,18.166026],[35.541181,18.265926]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37]]);}
spacing=62.000001;
pcbwidth=46.000001;
pcblength=18.600000;
// Populated PCB
module board(pushed=false,hulled=false){
translate([9.800000,8.450000,0.800000])rotate([0,0,90.000000])m2(pushed,hulled); // RevK:C_0603 C_0603_1608Metric (back)
translate([26.250000,8.400000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([31.550000,10.850000,0.800000])m8(pushed,hulled); // RevK:C_0805_ C_0805_2012Metric (back)
translate([15.500000,3.350000,0.800000])rotate([0,0,-90.000000])m10(pushed,hulled,2); // RevK:PTSM-HH-2-RA PTSM-HH-2-RA (back)
translate([26.250000,7.150000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([28.000000,11.700000,0.800000])rotate([0,0,90.000000])rotate([-0.000000,-0.000000,-90.000000])m13(pushed,hulled); // RevK:SOT-23-Thin-6-Reg SOT-23-6 (back)
translate([10.500000,15.000000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([28.000000,15.600000,0.800000])rotate([0,0,90.000000])rotate([-0.000000,-0.000000,-90.000000])m16(pushed,hulled); // RevK:L_4x4_ TYA4020 (back)
translate([36.300000,15.650000,0.800000])rotate([0,0,90.000000])m19(pushed,hulled); // RevK:Sensirion_DFN-4_1.5x1.5mm_P0.5mm_SHT4x LX5563LL-TR (back)
translate([4.500000,13.100000,0.800000])translate([0.000000,0.000000,3.000000])rotate([-0.000000,90.000000,-0.000000])m21(pushed,hulled); // RevK:Screw M3.5CSK (back)
translate([28.250000,9.600000,0.800000])m2(pushed,hulled); // RevK:C_0603 C_0603_1608Metric (back)
translate([31.550000,17.100000,0.800000])m8(pushed,hulled); // RevK:C_0805_ C_0805_2012Metric (back)
translate([10.500000,13.450000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([31.170000,13.980000,0.800000])rotate([0,0,90.000000])m25(pushed,hulled); // RevK:D_1206_ D_1206_3216Metric (back)
translate([11.000000,8.450000,0.800000])rotate([0,0,90.000000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([41.500000,13.100000,0.800000])translate([0.000000,0.000000,3.000000])rotate([-0.000000,90.000000,-0.000000])m21(pushed,hulled); // RevK:Screw M3.5CSK (back)
translate([36.300000,17.300000,0.800000])rotate([0,0,180.000000])m2(pushed,hulled); // RevK:C_0603 C_0603_1608Metric (back)
translate([30.557500,3.350000,0.800000])rotate([0,0,90.000000])m10(pushed,hulled,2); // RevK:PTSM-HH-2-RA PTSM-HH-2-RA (back)
translate([18.250000,12.656750,0.800000])m26(pushed,hulled); // RevK:ESP32-PICO-MINI-02 ESP32-PICO-MINI-02 (back)
translate([10.500000,11.800000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([31.000000,7.850000,0.800000])m25(pushed,hulled); // RevK:D_1206_ D_1206_3216Metric (back)
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
		translate([0,0,-10])cylinder(d=7,h=10);
		cylinder(d1=7,d2=3.5,h=2.5);
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

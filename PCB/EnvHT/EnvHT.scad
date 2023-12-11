// Generated case design for EnvHT/EnvHT.kicad_pcb
// By https://github.com/revk/PCBCase
// Generated 2023-10-09 09:16:14
// title:	EnvMonHT
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
pcbthickness=1.000000;
nohull=false;
hullcap=1.000000;
hulledge=1.000000;
useredge=true;

module outline(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[37.774344,6.700000],[39.524344,6.700000],[39.877897,6.846447],[40.024344,7.200000],[40.024344,14.813846],[39.731450,15.471014],[38.967237,16.156801],[38.808321,16.363910],[38.708419,16.605090],[38.674344,16.863908],[38.674344,18.000000],[44.674344,18.004462],[44.674344,18.000000],[45.411639,17.960033],[46.140633,17.842686],[46.853226,17.649263],[47.541503,17.381913],[48.197818,17.043605],[48.814881,16.638098],[49.385835,16.169897],[49.904340,15.644203],[50.364634,15.066854],[50.761604,14.444266],[51.090842,13.783354],[51.348688,13.091461],[51.532279,12.376271],[51.639576,11.645731],[51.669386,10.907956],[51.621379,10.171140],[51.496087,9.443470],[51.294902,8.733029],[51.020060,8.047709],[50.674613,7.395124],[50.262400,6.782521],[49.787998,6.216708],[49.256679,5.703969],[48.674344,5.250000],[41.924344,0.000000],[9.674344,0.000000],[2.674344,5.250000],[2.092009,5.703969],[1.560690,6.216708],[1.086288,6.782521],[0.674075,7.395124],[0.328628,8.047709],[0.053786,8.733029],[-0.147399,9.443470],[-0.272691,10.171140],[-0.320698,10.907956],[-0.290888,11.645731],[-0.183591,12.376271],[-0.000000,13.091461],[0.257846,13.783354],[0.587084,14.444266],[0.984054,15.066854],[1.444348,15.644203],[1.962853,16.169897],[2.533807,16.638098],[3.150870,17.043605],[3.807185,17.381913],[4.495462,17.649263],[5.208055,17.842686],[5.937049,17.960033],[6.674344,18.000000],[37.274344,18.000000],[37.274344,7.200000],[37.420791,6.846447]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11],[12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,0]]);}

module pcb(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[37.274344,7.200000],[37.274344,15.500000],[37.127897,15.853553],[36.774344,16.000000],[10.174344,16.000000],[9.820791,15.853553],[9.674344,15.500000],[9.674344,0.000000],[41.674344,0.000000],[41.674344,18.863908],[41.598222,19.246590],[41.381449,19.571013],[41.057026,19.787786],[40.674344,19.863908],[39.674344,19.863908],[39.291662,19.787786],[38.967239,19.571013],[38.750466,19.246590],[38.674344,18.863908],[38.674344,16.913908],[38.967237,16.206801],[39.731451,15.521015],[39.890369,15.313908],[39.990269,15.072727],[40.024344,14.813908],[40.024344,7.200000],[39.986283,7.008658],[39.877897,6.846447],[39.715686,6.738061],[39.524344,6.700000],[37.774344,6.700000],[37.583002,6.738061],[37.420791,6.846447],[37.312405,7.008658]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33]]);}
spacing=67.348688;
pcbwidth=51.348688;
pcblength=19.863908;
// Populated PCB
module board(pushed=false,hulled=false){
translate([34.274344,9.700000,1.000000])rotate([0,0,-90.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([34.474344,12.800000,1.000000])rotate([0,0,-90.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([11.674344,12.100000,1.000000])rotate([0,0,90.000000])m5(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([17.274344,9.500000,1.000000])rotate([0,0,-90.000000])m5(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([16.674344,3.250000,1.000000])rotate([0,0,-90.000000])m9(pushed,hulled,2); // RevK:PTSM-HH1-2-RA PTSM-HH1-2-RA (back)
translate([25.674344,8.300000,1.000000])m10(pushed,hulled); // RevK:ESP32-S3-MINI-1 ESP32-S3-MINI-1 (back)
translate([15.974344,15.300000,1.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([40.174344,17.363908,1.000000])rotate([0,0,90.000000])m16(pushed,hulled); // RevK:Sensirion_DFN-4_1.5x1.5mm_P0.5mm_SHT4x LX5563LL-TR (back)
translate([6.674344,11.000000,1.000000])translate([0.000000,0.000000,5.500000])rotate([-0.000000,90.000000,-0.000000])m18(pushed,hulled); // RevK:Screw M3.5CSK (back)
translate([14.174344,15.300000,1.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([17.474344,7.200000,1.000000])rotate([0,0,-90.000000])m20(pushed,hulled); // RevK:C_0402 C_0402_1005Metric (back)
translate([16.674344,12.100000,1.000000])rotate([0,0,90.000000])m5(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([34.274344,7.900000,1.000000])rotate([0,0,-90.000000])m20(pushed,hulled); // RevK:C_0402 C_0402_1005Metric (back)
translate([12.374344,15.300000,1.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([36.074344,8.900000,1.000000])rotate([0,0,-90.000000])m21(pushed,hulled); // RevK:D_SOD-123 D_SOD-123 (back)
translate([35.274344,12.800000,1.000000])rotate([0,0,-90.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([44.674344,11.000000,1.000000])translate([0.000000,0.000000,5.500000])rotate([-0.000000,90.000000,-0.000000])m18(pushed,hulled); // RevK:Screw M3.5CSK (back)
translate([35.274344,14.500000,1.000000])rotate([0,0,180.000000])translate([0.000000,-0.400000,0.000000])m24(pushed,hulled); // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric (back)
translate([35.274344,14.500000,1.000000])rotate([0,0,180.000000])translate([0.000000,0.400000,0.000000])m24(pushed,hulled); // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric (back)
translate([40.174344,19.163908,1.000000])rotate([0,0,180.000000])m20(pushed,hulled); // RevK:C_0402 C_0402_1005Metric (back)
translate([34.674344,3.350000,1.000000])rotate([0,0,90.000000])m9(pushed,hulled,2); // RevK:PTSM-HH1-2-RA PTSM-HH1-2-RA (back)
translate([36.074344,12.800000,1.000000])rotate([0,0,-90.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([14.174344,11.400000,1.000000])m5(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([14.174344,13.400000,1.000000])m28(pushed,hulled); // RevK:SOT-23-6-MD8942 SOT-23-6 (back)
translate([14.174344,8.800000,1.000000])rotate([-0.000000,-0.000000,-90.000000])m31(pushed,hulled); // RevK:L_4x4_ TYA4020 (back)
}

module b(cx,cy,z,w,l,h){translate([cx-w/2,cy-l/2,z])cube([w,l,h]);}
module m2(pushed=false,hulled=false)
{ // RevK:R_0402 R_0402_1005Metric
b(0,0,0,1.5,0.65,0.2); // Pad size
b(0,0,0,1.0,0.5,0.5); // Chip
}

module m5(pushed=false,hulled=false)
{ // RevK:C_0603_ C_0603_1608Metric
b(0,0,0,1.6,0.95,0.2); // Pad size
b(0,0,0,1.6,0.8,1); // Chip
}

module m9(pushed=false,hulled=false,n=0)
{ // RevK:PTSM-HH1-2-RA PTSM-HH1-2-RA
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

module m10(pushed=false,hulled=false)
{ // RevK:ESP32-S3-MINI-1 ESP32-S3-MINI-1
translate([-15.4/2,-15.45/2,0])
{
	if(!hulled)cube([15.4,20.5,0.8]);
	translate([0.7,0.5,0])cube([14,13.55,2.4]);
}
}

module m16(pushed=false,hulled=false)
{ // RevK:Sensirion_DFN-4_1.5x1.5mm_P0.5mm_SHT4x LX5563LL-TR
b(0,0,0,1.5,1.5,0.5);
if(!pushed&&!hulled)cylinder(d=1.5,h=20);
}

module m18(pushed=false,hulled=false)
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

module m20(pushed=false,hulled=false)
{ // RevK:C_0402 C_0402_1005Metric
b(0,0,0,1.5,0.65,0.2); // Pad size
b(0,0,0,1.0,0.5,1); // Chip
}

module m21(pushed=false,hulled=false)
{ // RevK:D_SOD-123 D_SOD-123
b(0,0,0,3.85,0.65,0.7);
b(0,0,0,2.85,1.8,1.35);
}

module m24(pushed=false,hulled=false)
{ // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric
b(0,0,0,1.6,0.8,0.25);
b(0,0,0,1.2,0.8,0.55);
b(0,0,0,0.8,0.8,0.95);
if(!hulled&&pushed)b(0,0,0,1,1,20);
}

module m28(pushed=false,hulled=false)
{ // RevK:SOT-23-6-MD8942 SOT-23-6
b(0,0,0,3.05,3.05,0.5);
b(0,0,0,1.45,3.05,1.1);
}

module m31(pushed=false,hulled=false)
{ // RevK:L_4x4_ TYA4020
b(0,0,0,4,4,3);
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

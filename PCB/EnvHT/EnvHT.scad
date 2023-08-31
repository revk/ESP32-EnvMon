// Generated case design for EnvHT/EnvHT.kicad_pcb
// By https://github.com/revk/PCBCase
// Generated 2023-08-31 13:06:40
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
pcbthickness=1.600000;
nohull=false;
hullcap=1.000000;
hulledge=1.000000;
useredge=true;

module outline(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[27.600000,7.200000],[27.600000,15.500000],[27.453553,15.853553],[27.100000,16.000000],[0.500000,16.000000],[0.146447,15.853553],[0.000000,15.500000],[0.000000,0.000000],[32.000000,0.000000],[32.000000,18.863908],[31.923878,19.246590],[31.707105,19.571013],[31.382682,19.787786],[31.000000,19.863908],[30.000000,19.863908],[29.617318,19.787786],[29.292895,19.571013],[29.076122,19.246590],[29.000000,18.863908],[29.000000,16.913908],[29.292893,16.206801],[30.057107,15.521015],[30.216025,15.313908],[30.315925,15.072727],[30.350000,14.813908],[30.350000,7.200000],[30.311939,7.008658],[30.203553,6.846447],[30.041342,6.738061],[29.850000,6.700000],[28.100000,6.700000],[27.908658,6.738061],[27.746447,6.846447],[27.638061,7.008658]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33]]);}

module pcb(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[27.600000,7.200000],[27.600000,15.500000],[27.453553,15.853553],[27.100000,16.000000],[0.500000,16.000000],[0.146447,15.853553],[0.000000,15.500000],[0.000000,0.000000],[32.000000,0.000000],[32.000000,18.863908],[31.923878,19.246590],[31.707105,19.571013],[31.382682,19.787786],[31.000000,19.863908],[30.000000,19.863908],[29.617318,19.787786],[29.292895,19.571013],[29.076122,19.246590],[29.000000,18.863908],[29.000000,16.913908],[29.292893,16.206801],[30.057107,15.521015],[30.216025,15.313908],[30.315925,15.072727],[30.350000,14.813908],[30.350000,7.200000],[30.311939,7.008658],[30.203553,6.846447],[30.041342,6.738061],[29.850000,6.700000],[28.100000,6.700000],[27.908658,6.738061],[27.746447,6.846447],[27.638061,7.008658]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33]]);}
spacing=48.000000;
pcbwidth=32.000000;
pcblength=19.863908;
// Populated PCB
module board(pushed=false,hulled=false){
translate([24.600000,9.700000,1.600000])rotate([0,0,-90.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([24.800000,12.800000,1.600000])rotate([0,0,-90.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([2.000000,12.100000,1.600000])rotate([0,0,90.000000])m5(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([7.600000,9.500000,1.600000])rotate([0,0,-90.000000])m5(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([7.000000,3.250000,1.600000])rotate([0,0,-90.000000])m9(pushed,hulled,2); // RevK:PTSM-HH1-2-RA PTSM-HH1-2-RA (back)
translate([16.000000,8.300000,1.600000])m10(pushed,hulled); // RevK:ESP32-S3-MINI-1 ESP32-S3-MINI-1 (back)
translate([6.300000,15.300000,1.600000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([30.500000,17.363908,1.600000])rotate([0,0,90.000000])m16(pushed,hulled); // RevK:Sensirion_DFN-4_1.5x1.5mm_P0.5mm_SHT4x LX5563LL-TR (back)
translate([-3.000000,11.000000,1.600000])translate([0.000000,0.000000,5.500000])rotate([-0.000000,90.000000,-0.000000])m18(pushed,hulled); // RevK:Screw M3.5CSK (back)
translate([4.500000,15.300000,1.600000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([7.800000,7.200000,1.600000])rotate([0,0,-90.000000])m20(pushed,hulled); // RevK:C_0402 C_0402_1005Metric (back)
translate([7.000000,12.100000,1.600000])rotate([0,0,90.000000])m5(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([24.600000,7.900000,1.600000])rotate([0,0,-90.000000])m20(pushed,hulled); // RevK:C_0402 C_0402_1005Metric (back)
translate([2.700000,15.300000,1.600000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
// Missing D1.1 D_SOD-123
translate([25.600000,12.800000,1.600000])rotate([0,0,-90.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([35.000000,11.000000,1.600000])translate([0.000000,0.000000,5.500000])rotate([-0.000000,90.000000,-0.000000])m18(pushed,hulled); // RevK:Screw M3.5CSK (back)
translate([25.600000,14.500000,1.600000])rotate([0,0,180.000000])translate([0.000000,-0.400000,0.000000])m25(pushed,hulled); // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric (back)
translate([25.600000,14.500000,1.600000])rotate([0,0,180.000000])translate([0.000000,0.400000,0.000000])m25(pushed,hulled); // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric (back)
translate([30.500000,19.163908,1.600000])rotate([0,0,180.000000])m20(pushed,hulled); // RevK:C_0402 C_0402_1005Metric (back)
translate([25.000000,3.350000,1.600000])rotate([0,0,90.000000])m9(pushed,hulled,2); // RevK:PTSM-HH1-2-RA PTSM-HH1-2-RA (back)
translate([26.400000,12.800000,1.600000])rotate([0,0,-90.000000])m2(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([4.500000,11.400000,1.600000])m5(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([4.500000,13.400000,1.600000])m29(pushed,hulled); // RevK:SOT-23-6-MD8942 SOT-23-6 (back)
translate([4.500000,8.800000,1.600000])rotate([-0.000000,-0.000000,-90.000000])m32(pushed,hulled); // RevK:L_4x4_ TYA4020 (back)
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

module m25(pushed=false,hulled=false)
{ // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric
b(0,0,0,1.6,0.8,0.25);
b(0,0,0,1.2,0.8,0.55);
b(0,0,0,0.8,0.8,0.95);
if(!hulled&&pushed)b(0,0,0,1,1,20);
}

module m29(pushed=false,hulled=false)
{ // RevK:SOT-23-6-MD8942 SOT-23-6
b(0,0,0,3.05,3.05,0.5);
b(0,0,0,1.45,3.05,1.1);
}

module m32(pushed=false,hulled=false)
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

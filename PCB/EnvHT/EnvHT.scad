// Generated case design for PCB/EnvHT/EnvHT.kicad_pcb
// By https://github.com/revk/PCBCase
// Generated 2023-05-01 07:11:27
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
casebase=2.000000;
casetop=10.400000;
casewall=3.000000;
fit=0.000000;
edge=1.000000;
pcbthickness=0.800000;
nohull=false;
hullcap=1.000000;
hulledge=1.000000;
useredge=false;

module outline(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[27.500000,18.300000],[28.500000,18.300000],[28.758819,18.265926],[29.000000,18.166026],[29.207107,18.007107],[29.366026,17.800000],[29.465926,17.558819],[29.500000,17.300000],[29.500000,0.000000],[0.000000,0.000000],[0.000000,17.800000],[0.038061,17.991342],[0.146447,18.153553],[0.308658,18.261939],[0.500000,18.300000],[24.600000,18.300000],[24.791342,18.261939],[24.953553,18.153553],[25.061939,17.991342],[25.100000,17.800000],[25.100000,7.200000],[25.246447,6.846447],[25.600000,6.700000],[27.350000,6.700000],[27.703553,6.846447],[27.850000,7.200000],[27.850000,13.250000],[27.557107,13.957107],[26.792893,14.642893],[26.633974,14.850000],[26.534073,15.091181],[26.500000,15.350000],[26.500000,17.300000],[26.534074,17.558819],[26.633974,17.800000],[26.792893,18.007107],[27.000000,18.166026],[27.241181,18.265926]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37]]);}

module pcb(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[27.500000,18.300000],[28.500000,18.300000],[28.758819,18.265926],[29.000000,18.166026],[29.207107,18.007107],[29.366026,17.800000],[29.465926,17.558819],[29.500000,17.300000],[29.500000,0.000000],[0.000000,0.000000],[0.000000,17.800000],[0.038061,17.991342],[0.146447,18.153553],[0.308658,18.261939],[0.500000,18.300000],[24.600000,18.300000],[24.791342,18.261939],[24.953553,18.153553],[25.061939,17.991342],[25.100000,17.800000],[25.100000,7.200000],[25.246447,6.846447],[25.600000,6.700000],[27.350000,6.700000],[27.703553,6.846447],[27.850000,7.200000],[27.850000,13.250000],[27.557107,13.957107],[26.792893,14.642893],[26.633974,14.850000],[26.534073,15.091181],[26.500000,15.350000],[26.500000,17.300000],[26.534074,17.558819],[26.633974,17.800000],[26.792893,18.007107],[27.000000,18.166026],[27.241181,18.265926]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37]]);}
spacing=45.500000;
pcbwidth=29.500000;
pcblength=18.300000;
// Populated PCB
module board(pushed=false,hulled=false){
translate([1.500000,8.450000,0.800000])rotate([0,0,90.000000])m2(pushed,hulled); // RevK:C_0603 C_0603_1608Metric (back)
translate([17.950000,8.400000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([23.250000,10.850000,0.800000])m8(pushed,hulled); // RevK:C_0805_ C_0805_2012Metric (back)
translate([7.200000,3.350000,0.800000])rotate([0,0,-90.000000])m10(pushed,hulled,2); // RevK:PTSM-HH-2-RA PTSM-HH-2-RA (back)
translate([17.950000,7.150000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([19.700000,11.700000,0.800000])rotate([0,0,90.000000])rotate([-0.000000,-0.000000,-90.000000])m13(pushed,hulled); // RevK:SOT-23-Thin-6-Reg SOT-23-6 (back)
translate([2.200000,15.000000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([19.700000,15.600000,0.800000])rotate([0,0,90.000000])rotate([-0.000000,-0.000000,-90.000000])m16(pushed,hulled); // RevK:L_4x4_ TYA4020 (back)
// Missing U7.1 LX5563LL-TR
translate([-6.800000,13.100000,0.800000])translate([0.000000,0.000000,5.000000])rotate([-0.000000,-90.000000,-0.000000])m21(pushed,hulled); // RevK:Screw 653612 (back)
translate([19.950000,9.600000,0.800000])m2(pushed,hulled); // RevK:C_0603 C_0603_1608Metric (back)
translate([23.250000,17.100000,0.800000])m8(pushed,hulled); // RevK:C_0805_ C_0805_2012Metric (back)
translate([2.200000,13.450000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([22.870000,13.980000,0.800000])rotate([0,0,90.000000])m25(pushed,hulled); // RevK:D_1206_ D_1206_3216Metric (back)
translate([2.700000,8.450000,0.800000])rotate([0,0,90.000000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([36.200000,13.100000,0.800000])translate([0.000000,0.000000,5.000000])rotate([-0.000000,-90.000000,-0.000000])m21(pushed,hulled); // RevK:Screw 653612 (back)
translate([28.000000,17.300000,0.800000])rotate([0,0,180.000000])m2(pushed,hulled); // RevK:C_0603 C_0603_1608Metric (back)
translate([22.257500,3.350000,0.800000])rotate([0,0,90.000000])m10(pushed,hulled,2); // RevK:PTSM-HH-2-RA PTSM-HH-2-RA (back)
translate([9.950000,12.656750,0.800000])m26(pushed,hulled); // RevK:ESP32-PICO-MINI-02 ESP32-PICO-MINI-02 (back)
translate([2.200000,11.800000,0.800000])m5(pushed,hulled); // RevK:R_0603 R_0603_1608Metric (back)
translate([22.700000,7.850000,0.800000])m25(pushed,hulled); // RevK:D_1206_ D_1206_3216Metric (back)
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
hull()
{ // Socket
	b(0,-7.5/2+0.3,0,1.7+n*2.5,7.5,4);
	b(0,-7.5/2+0.3,0,1.7+n*2.5-2,7.5,5);
}
// Plug
b(0,-10.5/2-7.5+0.3,0,1.1+n*2.5,10.5,5);
// Pins
if(!hulled)for(p=[0:n-1])translate([-2.5*(n-1)/2+p*2.5,0,-2.1])cylinder(r=0.3,h=2.1);
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

module m21(pushed=false,hulled=false)
{ // RevK:Screw 653612
// Screw 6mm
if(!hulled&&!pushed)
rotate([0,-90,0],$fn=24)
{
	hull()for(x=[-1,1])translate([x,0,-2])cylinder(d=12,h=2);
	hull()for(x=[-1,1])translate([x,0,0])cylinder(d1=12,d2=6,h=3);
	hull()for(x=[-1,1])translate([x,0,0])cylinder(d=6,h=100);
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

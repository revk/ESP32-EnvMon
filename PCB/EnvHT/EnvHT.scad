// Generated case design for EnvHT/EnvHT.kicad_pcb
// By https://github.com/revk/PCBCase
// Generated 2025-04-26 07:06:01
// title:	EnvMonHT
// rev:	4
// comment:	www.me.uk
// comment:	@TheRealRevK
// comment:	Adrian Kennard Andrews & Arnold Ltd
//

// Globals
margin=0.200000;
lip=2.000000;
casebottom=1.000000;
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
// Parts to go on PCB (top)
module parts_top(part=false,hole=false,block=false){
translate([34.274344,9.700000,1.000000])rotate([0,0,-90.000000])m0(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([34.474344,12.800000,1.000000])rotate([0,0,-90.000000])m0(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([11.674344,12.100000,1.000000])rotate([0,0,90.000000])m1(part,hole,block,casetop); // RevK:C_0603_ C_0603_1608Metric (back)
translate([17.274344,9.500000,1.000000])rotate([0,0,-90.000000])m1(part,hole,block,casetop); // RevK:C_0603_ C_0603_1608Metric (back)
translate([16.674344,3.250000,1.000000])rotate([0,0,-90.000000])m2(part,hole,block,casetop,2); // (null) (null) (back)
translate([25.674344,8.300000,1.000000])m3(part,hole,block,casetop); // (null) (null) (back)
translate([15.974344,15.300000,1.000000])m0(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
// Missing model (null).1 LX5563LL-TR
// Missing model (null).1 M3.5CSK
translate([14.174344,15.300000,1.000000])m0(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([17.474344,7.200000,1.000000])rotate([0,0,-90.000000])m4(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([16.674344,12.100000,1.000000])rotate([0,0,90.000000])m1(part,hole,block,casetop); // RevK:C_0603_ C_0603_1608Metric (back)
translate([34.274344,7.900000,1.000000])rotate([0,0,-90.000000])m4(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([12.374344,15.300000,1.000000])m0(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([36.074344,8.900000,1.000000])rotate([0,0,-90.000000])m5(part,hole,block,casetop); // (null) (null) (back)
translate([35.274344,12.800000,1.000000])rotate([0,0,-90.000000])m0(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
// Missing model (null).1 M3.5CSK
translate([35.274344,14.500000,1.000000])rotate([0,0,180.000000])translate([0.000000,-0.400000,0.000000])m6(part,hole,block,casetop); // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric (back)
translate([35.274344,14.500000,1.000000])rotate([0,0,180.000000])translate([0.000000,0.400000,0.000000])m6(part,hole,block,casetop); // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric (back)
translate([40.174344,19.163908,1.000000])rotate([0,0,180.000000])m4(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([34.674344,3.350000,1.000000])rotate([0,0,90.000000])m2(part,hole,block,casetop,2); // (null) (null) (back)
translate([36.074344,12.800000,1.000000])rotate([0,0,-90.000000])m0(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([14.174344,11.400000,1.000000])m1(part,hole,block,casetop); // RevK:C_0603_ C_0603_1608Metric (back)
translate([14.174344,13.400000,1.000000])m7(part,hole,block,casetop); // RevK:SOT-23-6-MD8942 SOT-23-6 (back)
translate([14.174344,8.800000,1.000000])rotate([-0.000000,-0.000000,-90.000000])m8(part,hole,block,casetop); // RevK:L_4x4_ TYA4020 (back)
}

parts_top=4;
// Parts to go on PCB (bottom)
module parts_bottom(part=false,hole=false,block=false){
}

parts_bottom=0;
module b(cx,cy,z,w,l,h){translate([cx-w/2,cy-l/2,z])cube([w,l,h]);}
module m0(part=false,hole=false,block=false,height)
{ // RevK:R_0402 R_0402_1005Metric
// 0402 Resistor
if(part)
{
	b(0,0,0,1.5,0.65,0.2); // Pad size
	b(0,0,0,1.0,0.5,0.5); // Chip
}
}

module m1(part=false,hole=false,block=false,height)
{ // RevK:C_0603_ C_0603_1608Metric
// 0603 Capacitor
if(part)
{
	b(0,0,0,1.6,0.8,1); // Chip
	b(0,0,0,1.6,0.95,0.2); // Pad size
}
}

module m2(part=false,hole=false,block=false,height,N=0)
{ // (null) (null)
// PTSM socket
if(part)
{
	hull()
	{
		b(0,-7.5/2+0.3,0,1.7+N*2.5,7.5,4);
		b(0,-7.5/2+0.3,0,1.7+N*2.5-2,7.5,5);
	}
	// Pins
	for(p=[0:N-1])translate([-2.5*(N-1)/2+p*2.5,0,-2.1])cylinder(r1=0.3,r2=1,h=2.1);
}
if(hole)
{
	b(0,-10.5/2-7.5+0.3,0,1.1+N*2.5,10.5,5);
}
}

module m3(part=false,hole=false,block=false,height)
{ // (null) (null)
// ESP32-S3-MINI-1
translate([-15.4/2,-15.45/2,0])
{
	if(part)
	{
		cube([15.4,20.5,0.8]);
		translate([0.7,0.5,0])cube([14,13.55,2.4]);
	}
	if(hole)
	{
		cube([15.4,20.5,0.8]);
	}
}
}

module m4(part=false,hole=false,block=false,height)
{ // RevK:C_0402 C_0402_1005Metric
// 0402 Capacitor
if(part)
{
	b(0,0,0,1.0,0.5,1); // Chip
	b(0,0,0,1.5,0.65,0.2); // Pad size
}
}

module m5(part=false,hole=false,block=false,height)
{ // (null) (null)
// SOD-123 Diode
if(part)
{
	b(0,0,0,2.85,1.8,1.35); // part
	b(0,0,0,4.2,1.2,0.7); // pads
}
}

module m6(part=false,hole=false,block=false,height)
{ // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric
if(part)
{
	b(0,0,0,1.6,0.8,0.25);
	b(0,0,0,1.2,0.8,0.55);
	b(0,0,0,0.8,0.8,0.95);
}
if(hole)
{
	b(0,0,0,0.999,0.999,20);
}
}

module m7(part=false,hole=false,block=false,height)
{ // RevK:SOT-23-6-MD8942 SOT-23-6
// SOT-23-6
if(part)
{
	b(0,0,0,1.726,3.026,1.2); // Part
	b(0,0,0,3.6,2.5,0.5); // Pins
}
}

module m8(part=false,hole=false,block=false,height)
{ // RevK:L_4x4_ TYA4020
// 4x4 Inductor
if(part)
{
	b(0,0,0,4,4,3);
}
}

// Generate PCB casework

height=casebottom+pcbthickness+casetop;
$fn=48;

module pyramid()
{ // A pyramid
 polyhedron(points=[[0,0,0],[-height,-height,height],[-height,height,height],[height,height,height],[height,-height,height]],faces=[[0,1,2],[0,2,3],[0,3,4],[0,4,1],[4,3,2,1]]);
}


module pcb_hulled(h=pcbthickness,r=0)
{ // PCB shape for case
	if(useredge)outline(h,r);
	else hull()outline(h,r);
}

module solid_case(d=0)
{ // The case wall
	hull()
        {
                translate([0,0,-casebottom])pcb_hulled(height,casewall-edge);
                translate([0,0,edge-casebottom])pcb_hulled(height-edge*2,casewall);
        }
}

module preview()
{
	pcb();
	color("#0f0")parts_top(part=true);
	color("#0f0")parts_bottom(part=true);
	color("#f00")parts_top(hole=true);
	color("#f00")parts_bottom(hole=true);
	color("#00f8")parts_top(block=true);
	color("#00f8")parts_bottom(block=true);
}

module top_half(step=false)
{
	difference()
	{
		translate([-casebottom-100,-casewall-100,pcbthickness-lip/2+0.01]) cube([pcbwidth+casewall*2+200,pcblength+casewall*2+200,height]);
		if(step)translate([0,0,pcbthickness-lip/2-0.01])pcb_hulled(lip,casewall/2+fit);
	}
}

module bottom_half(step=false)
{
	translate([-casebottom-100,-casewall-100,pcbthickness+lip/2-height-0.01]) cube([pcbwidth+casewall*2+200,pcblength+casewall*2+200,height]);
	if(step)translate([0,0,pcbthickness-lip/2])pcb_hulled(lip,casewall/2-fit);
}

module case_wall()
{
	difference()
	{
		solid_case();
		translate([0,0,-height])pcb_hulled(height*2);
	}
}

module top_side_hole()
{
	intersection()
	{
		parts_top(hole=true);
		case_wall();
	}
}

module bottom_side_hole()
{
	intersection()
	{
		parts_bottom(hole=true);
		case_wall();
	}
}

module parts_space()
{
	minkowski()
	{
		union()
		{
			parts_top(part=true,hole=true);
			parts_bottom(part=true,hole=true);
		}
		sphere(r=margin,$fn=6);
	}
}

module top_cut()
{
	difference()
	{
		top_half(true);
		if(parts_top)difference()
		{
			minkowski()
			{ // Penetrating side holes
				top_side_hole();
				rotate([180,0,0])
				pyramid();
			}
			minkowski()
			{
				top_side_hole();
				rotate([0,0,45])cylinder(r=margin,h=height,$fn=4);
			}
		}
	}
	if(parts_bottom)difference()
	{
		minkowski()
		{ // Penetrating side holes
			bottom_side_hole();
			pyramid();
		}
			minkowski()
			{
				bottom_side_hole();
				rotate([0,0,45])translate([0,0,-height])cylinder(r=margin,h=height,$fn=4);
			}
	}
}

module bottom_cut()
{
	difference()
	{
		 translate([-casebottom-50,-casewall-50,-height]) cube([pcbwidth+casewall*2+100,pcblength+casewall*2+100,height*2]);
		 top_cut();
	}
}

module top_body()
{
	difference()
	{
		intersection()
		{
			solid_case();
			pcb_hulled(height);
			top_half();
		}
		if(parts_top)minkowski()
		{
			if(nohull)parts_top(part=true);
			else hull()parts_top(part=true);
			translate([0,0,margin-height])cylinder(r=margin,h=height,$fn=8);
		}
	}
	intersection()
	{
		solid_case();
		parts_top(block=true);
	}
}

module top_edge()
{
	intersection()
	{
		case_wall();
		top_cut();
	}
}

module top()
{
	translate([casewall,casewall+pcblength,pcbthickness+casetop])rotate([180,0,0])difference()
	{
		union()
		{
			top_body();
			top_edge();
		}
		parts_space();
		translate([0,0,pcbthickness-height])pcb(height,r=margin);
	}
}

module bottom_body()
{
	difference()
	{
		intersection()
		{
			solid_case();
			translate([0,0,-height])pcb_hulled(height);
			bottom_half();
		}
		if(parts_bottom)minkowski()
		{
			if(nohull)parts_bottom(part=true);
			else hull()parts_bottom(part=true);
			translate([0,0,-margin])cylinder(r=margin,h=height,$fn=8);
		}
	}
	intersection()
	{
		solid_case();
		parts_bottom(block=true);
	}
}

module bottom_edge()
{
	intersection()
	{
		case_wall();
		bottom_cut();
	}
}

module bottom()
{
	translate([casewall,casewall,casebottom])difference()
	{
		union()
		{
        		bottom_body();
        		bottom_edge();
		}
		parts_space();
		pcb(height,r=margin);
	}
}
bottom(); translate([spacing,0,0])top();

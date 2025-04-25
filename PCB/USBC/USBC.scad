// Generated case design for USBC/USBC.kicad_pcb
// By https://github.com/revk/PCBCase
// Generated 2025-04-25 14:43:37
// title:	PCB-USBA
// rev:	1
// comment:	www.me.uk
// comment:	@TheRealRevK
//

// Globals
margin=0.200000;
lip=2.000000;
casebottom=1.000000;
casetop=7.000000;
casewall=3.000000;
fit=0.000000;
edge=1.000000;
pcbthickness=1.200000;
nohull=false;
hullcap=1.000000;
hulledge=1.000000;
useredge=false;

module outline(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[24.500000,14.464466],[37.500000,14.464466],[37.934119,14.426482],[38.355048,14.313692],[38.749996,14.129523],[39.106964,13.879571],[39.415106,13.571430],[39.665059,13.214462],[39.849228,12.819514],[39.962018,12.398585],[40.000000,11.964466],[40.000000,3.964466],[39.962019,3.530346],[39.849232,3.109416],[39.665064,2.714466],[39.415111,2.357497],[39.106969,2.049355],[38.750000,1.799402],[38.355050,1.615234],[37.934120,1.502447],[37.500000,1.464466],[24.500000,1.464466],[23.852953,1.379279],[23.250002,1.129527],[22.732233,0.732233],[22.433928,0.477460],[22.099441,0.272487],[21.737007,0.122362],[21.355552,0.030782],[20.964466,0.000000],[0.000000,0.000000],[0.000000,15.928932],[20.964466,15.928932],[21.355553,15.898159],[21.737011,15.806581],[22.099445,15.656455],[22.433931,15.451478],[22.732233,15.196699],[23.249998,14.799400],[23.852952,14.549650]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39]]);}

module pcb(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[24.500000,14.464466],[37.500000,14.464466],[37.934119,14.426482],[38.355048,14.313692],[38.749996,14.129523],[39.106964,13.879571],[39.415106,13.571430],[39.665059,13.214462],[39.849228,12.819514],[39.962018,12.398585],[40.000000,11.964466],[40.000000,3.964466],[39.962019,3.530346],[39.849232,3.109416],[39.665064,2.714466],[39.415111,2.357497],[39.106969,2.049355],[38.750000,1.799402],[38.355050,1.615234],[37.934120,1.502447],[37.500000,1.464466],[24.500000,1.464466],[23.852953,1.379279],[23.250002,1.129527],[22.732233,0.732233],[22.433928,0.477460],[22.099441,0.272487],[21.737007,0.122362],[21.355552,0.030782],[20.964466,0.000000],[0.000000,0.000000],[0.000000,15.928932],[20.964466,15.928932],[21.355553,15.898159],[21.737011,15.806581],[22.099445,15.656455],[22.433931,15.451478],[22.732233,15.196699],[23.249998,14.799400],[23.852952,14.549650]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39]]);}
spacing=56.000000;
pcbwidth=40.000000;
pcblength=15.928932;
// Parts to go on PCB (top)
module parts_top(part=false,hole=false,block=false){
translate([23.700000,4.564466,1.200000])m0(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([36.301041,5.864466,1.200000])rotate([0,0,-45.000000])m1(part,hole,block,casetop); // D4 (back)
translate([21.720000,8.144466,1.200000])rotate([0,0,-90.000000])m2(part,hole,block,casetop); // RevK:C_0603_ C_0603_1608Metric (back)
translate([23.700000,7.964466,1.200000])rotate([0,0,135.000000])m1(part,hole,block,casetop); // D4 (back)
translate([19.225000,6.839466,1.200000])rotate([0,0,180.000000])m3(part,hole,block,casetop); // RevK:SOT-23-6-MD8942 SOT-23-6 (back)
translate([19.220000,8.844466,1.200000])rotate([0,0,180.000000])m2(part,hole,block,casetop); // RevK:C_0603_ C_0603_1608Metric (back)
translate([17.000000,14.964466,1.200000])rotate([0,0,180.000000])m0(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([23.700000,10.064466,1.200000])rotate([0,0,135.000000])m1(part,hole,block,casetop); // D4 (back)
translate([19.220000,4.944466,1.200000])rotate([0,0,180.000000])m4(part,hole,block,casetop); // RevK:R_0402_ R_0402_1005Metric (back)
translate([21.020000,4.944466,1.200000])rotate([0,0,180.000000])m4(part,hole,block,casetop); // RevK:R_0402_ R_0402_1005Metric (back)
translate([30.000000,7.964466,1.200000])m5(part,hole,block,casetop); // U3 (back)
translate([36.300000,7.964466,1.200000])rotate([0,0,-45.000000])m1(part,hole,block,casetop); // D4 (back)
translate([22.450000,3.414466,1.200000])rotate([0,0,-90.000000])m4(part,hole,block,casetop); // RevK:R_0402_ R_0402_1005Metric (back)
translate([36.301041,10.064466,1.200000])rotate([0,0,-45.000000])m1(part,hole,block,casetop); // D4 (back)
translate([16.400000,10.664466,1.200000])rotate([0,0,90.000000])m0(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([23.700000,5.864466,1.200000])rotate([0,0,135.000000])m1(part,hole,block,casetop); // D4 (back)
translate([24.300000,7.264466,1.200000])m6(part,hole,block,casetop); // REF** (back)
translate([16.720000,8.144466,1.200000])rotate([0,0,90.000000])m2(part,hole,block,casetop); // RevK:C_0603_ C_0603_1608Metric (back)
translate([18.475000,3.364466,1.200000])m7(part,hole,block,casetop); // RevK:C_1206 C_1206_3216Metric (back)
translate([37.500000,13.264466,1.200000])rotate([0,0,180.000000])m0(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([19.220000,11.444466,1.200000])rotate([0,0,180.000000])rotate([-0.000000,-0.000000,-90.000000])m8(part,hole,block,casetop); // RevK:L_4x4_ TYA4020 (back)
translate([23.700000,11.364466,1.200000])m0(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([37.500000,2.664466,1.200000])rotate([0,0,180.000000])m0(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([19.000000,14.964466,1.200000])rotate([0,0,180.000000])m4(part,hole,block,casetop); // RevK:R_0402_ R_0402_1005Metric (back)
translate([8.000000,7.964466,1.200000])rotate([0,0,90.000000])m9(part,hole,block,casetop); // U2 (back)
translate([17.420000,4.944466,1.200000])rotate([0,0,180.000000])m4(part,hole,block,casetop); // RevK:R_0402_ R_0402_1005Metric (back)
translate([16.400000,12.564466,1.200000])rotate([0,0,90.000000])m0(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
}

parts_top=9;
// Parts to go on PCB (bottom)
module parts_bottom(part=false,hole=false,block=false){
translate([37.500000,7.964466,0.000000])rotate([0,0,-90.000000])rotate([180,0,0])m10(part,hole,block,casebottom); // J1
}

parts_bottom=1;
module b(cx,cy,z,w,l,h){translate([cx-w/2,cy-l/2,z])cube([w,l,h]);}
module m0(part=false,hole=false,block=false,height)
{ // RevK:C_0402 C_0402_1005Metric
// 0402 Capacitor
if(part)
{
	b(0,0,0,1.0,0.5,1); // Chip
	b(0,0,0,1.5,0.65,0.2); // Pad size
}
}

module m1(part=false,hole=false,block=false,height)
{ // D4
// 1x1mm LED
if(part)
{
        b(0,0,0,1.2,1.2,.8);
}
if(hole)
{
        hull()
        {
                b(0,0,.8,1.2,1.2,1);
                translate([0,0,height])cylinder(d=2,h=1,$fn=16);
        }
}
if(block)
{
        hull()
        {
                b(0,0,0,2.4,2.4,1);
                translate([0,0,height])cylinder(d=4,h=1,$fn=16);
        }
}
}

module m2(part=false,hole=false,block=false,height)
{ // RevK:C_0603_ C_0603_1608Metric
// 0603 Capacitor
if(part)
{
	b(0,0,0,1.6,0.8,1); // Chip
	b(0,0,0,1.6,0.95,0.2); // Pad size
}
}

module m3(part=false,hole=false,block=false,height)
{ // RevK:SOT-23-6-MD8942 SOT-23-6
// SOT-23-6
if(part)
{
	b(0,0,0,1.726,3.026,1.2); // Part
	b(0,0,0,3.6,2.5,0.5); // Pins
}
}

module m4(part=false,hole=false,block=false,height)
{ // RevK:R_0402_ R_0402_1005Metric
// 0402 Resistor
if(part)
{
	b(0,0,0,1.5,0.65,0.2); // Pad size
	b(0,0,0,1.0,0.5,0.5); // Chip
}
}

module m5(part=false,hole=false,block=false,height)
{ // U3
if(part)
{
	b(0,0,0,10.4,10.4,1);	// Base board
	hull()
	{
		for(x=[-3.3,3.333])for(y=[-3.3,3.3])translate([x,y,0])cylinder(r=1,h=1,$fn=8);
		for(x=[-3,3])for(y=[-3,3])translate([x,y,4.8])sphere(r=2,$fn=12);
	}
}
if(hole)
{
		for(x=[-2,2])for(y=[-2,2])translate([x,y,1])cylinder(d=2,h=height,$fn=8);
}
}

module m6(part=false,hole=false,block=false,height)
{ // REF**
// Nothing
}

module m7(part=false,hole=false,block=false,height)
{ // RevK:C_1206 C_1206_3216Metric
// 1206 Capacitor
if(part)
{
	b(0,0,0,3.2,1.6,1.5); // Part
	b(0,0,0,4.1,1.8,0.2); // Pads
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

module m9(part=false,hole=false,block=false,height)
{ // U2
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

module m10(part=false,hole=false,block=false,height)
{ // J1
if(part)
{	// Do this all as hole, no point doing hull over the part

}
if(hole)
{
	hull()
	{
		translate([-2.825,0,0])cylinder(d=2.6+0.4,h=8.85); // Slightly extended as hole is not extended
		translate([2.825,0,0])cylinder(d=2.6+0.4,h=8.85);
	}
	translate([0,0,2.5])cylinder(r=10,h=100); // Plug in to something
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

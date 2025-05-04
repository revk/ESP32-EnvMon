// Generated case design for EnvMon/EnvMon.kicad_pcb
// By https://github.com/revk/PCBCase
// Generated 2025-05-04 08:09:30
// title:	EnvMon
// rev:	4
// comment:	www.me.uk
// comment:	@TheRealRevK
// comment:	Adrian Kennard Andrews & Arnold Ltd
//

// Globals
margin=0.200000;
lip=2.000000;
casebottom=2.000000;
casetop=7.500000;
casewall=3.000000;
fit=0.000000;
edge=1.000000;
pcbthickness=0.800000;
nohull=false;
hullcap=1.000000;
hulledge=1.000000;
useredge=true;

module outline(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[11.000000,7.000000],[11.000000,10.000000],[10.923880,10.382683],[10.707107,10.707106],[10.382683,10.923879],[10.000000,11.000000],[1.000000,11.000000],[0.617317,10.923880],[0.292894,10.707107],[0.076121,10.382683],[0.000000,10.000000],[0.000000,1.000000],[0.076120,0.617317],[0.292893,0.292894],[0.617317,0.076121],[1.000000,0.000000],[10.000000,0.000000],[10.382683,0.076120],[10.707106,0.292893],[10.923879,0.617317],[11.000000,1.000000],[11.034074,1.258819],[11.133975,1.500000],[11.292893,1.707107],[11.500000,1.866025],[11.741181,1.965926],[12.000000,2.000000],[12.258819,1.965926],[12.500000,1.866025],[12.707107,1.707107],[12.866025,1.500000],[12.965926,1.258819],[13.000000,1.000000],[13.076120,0.617317],[13.292893,0.292894],[13.617317,0.076121],[14.000000,0.000000],[56.500000,0.000000],[56.882683,0.076120],[57.207106,0.292893],[57.423879,0.617317],[57.500000,1.000000],[57.500000,36.000000],[57.423880,36.382683],[57.207107,36.707106],[56.882683,36.923879],[56.500000,37.000000],[14.000000,37.000000],[13.617317,36.923880],[13.292894,36.707107],[13.076121,36.382683],[13.000000,36.000000],[13.000000,7.000000],[12.965926,6.741181],[12.866025,6.500000],[12.707107,6.292893],[12.500000,6.133975],[12.258819,6.034074],[12.000000,6.000000],[11.741181,6.034074],[11.500000,6.133975],[11.292893,6.292893],[11.133975,6.500000],[11.034074,6.741181]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63]]);}

module pcb(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[13.000000,21.750000],[13.000000,28.000000],[12.951057,28.309017],[12.809017,28.587785],[12.587785,28.809017],[12.309017,28.951057],[12.000000,29.000000],[11.690983,28.951057],[11.412215,28.809017],[11.190983,28.587785],[11.048943,28.309017],[11.000000,28.000000],[11.000000,21.750000],[11.048943,21.440983],[11.190983,21.162215],[11.412215,20.940983],[11.690983,20.798943],[12.000000,20.750000],[12.309017,20.798943],[12.587785,20.940983],[12.809017,21.162215],[12.951057,21.440983],[13.000000,15.750000],[12.951057,16.059017],[12.809017,16.337785],[12.587785,16.559017],[12.309017,16.701057],[12.000000,16.750000],[11.690983,16.701057],[11.412215,16.559017],[11.190983,16.337785],[11.048943,16.059017],[11.000000,15.750000],[11.000000,14.000000],[10.965926,13.741181],[10.866026,13.500000],[10.707107,13.292893],[10.500000,13.133974],[10.258819,13.034074],[10.000000,13.000000],[1.000000,13.000000],[0.741181,13.034074],[0.500000,13.133974],[0.292893,13.292893],[0.133974,13.500000],[0.034074,13.741181],[0.000000,14.000000],[0.000000,36.000000],[0.034074,36.258819],[0.133974,36.500000],[0.292893,36.707107],[0.500000,36.866026],[0.741181,36.965926],[1.000000,37.000000],[10.000000,37.000000],[10.258819,36.965926],[10.500000,36.866026],[10.707107,36.707107],[10.866026,36.500000],[10.965926,36.258819],[11.000000,36.000000],[11.000000,34.000000],[11.048943,33.690983],[11.190983,33.412215],[11.412215,33.190983],[11.690983,33.048943],[12.000000,33.000000],[12.309017,33.048943],[12.587785,33.190983],[12.809017,33.412215],[12.951057,33.690983],[13.000000,34.000000],[13.000000,36.000000],[13.034074,36.258819],[13.133974,36.500000],[13.292893,36.707107],[13.500000,36.866026],[13.741181,36.965926],[14.000000,37.000000],[22.750000,37.000000],[22.875000,36.966507],[22.966507,36.875000],[23.000000,36.750000],[23.000000,32.750000],[23.076120,32.367316],[23.292893,32.042893],[23.617316,31.826120],[24.000000,31.750000],[46.500000,31.750000],[46.882684,31.826120],[47.207107,32.042893],[47.423880,32.367316],[47.500000,32.750000],[47.500000,36.750000],[47.533493,36.875000],[47.625000,36.966507],[47.750000,37.000000],[56.500000,37.000000],[56.758819,36.965926],[57.000000,36.866026],[57.207107,36.707107],[57.366026,36.500000],[57.465926,36.258819],[57.500000,36.000000],[57.500000,1.000000],[57.465926,0.741181],[57.366026,0.500000],[57.207107,0.292893],[57.000000,0.133974],[56.758819,0.034074],[56.500000,0.000000],[50.000000,0.000000],[49.875000,0.033493],[49.783493,0.125000],[49.750000,0.250000],[49.673880,0.632684],[49.457107,0.957107],[49.132684,1.173880],[48.750000,1.250000],[43.250000,1.250000],[42.867316,1.173880],[42.542893,0.957107],[42.326120,0.632684],[42.250000,0.250000],[42.216507,0.125000],[42.125000,0.033493],[42.000000,0.000000],[29.500000,0.000000],[29.375000,0.033493],[29.283493,0.125000],[29.250000,0.250000],[29.173880,0.632684],[28.957107,0.957107],[28.632684,1.173880],[28.250000,1.250000],[20.250000,1.250000],[19.867316,1.173880],[19.542893,0.957107],[19.326120,0.632684],[19.250000,0.250000],[19.216507,0.125000],[19.125000,0.033493],[19.000000,0.000000],[14.000000,0.000000],[13.741181,0.034074],[13.500000,0.133974],[13.292893,0.292893],[13.133974,0.500000],[13.034074,0.741181],[13.000000,1.000000],[12.951057,1.309017],[12.809017,1.587785],[12.587785,1.809017],[12.309017,1.951057],[12.000000,2.000000],[11.690983,1.951057],[11.412215,1.809017],[11.190983,1.587785],[11.048943,1.309017],[11.000000,1.000000],[10.965926,0.741181],[10.866026,0.500000],[10.707107,0.292893],[10.500000,0.133974],[10.258819,0.034074],[10.000000,0.000000],[1.000000,0.000000],[0.741181,0.034074],[0.500000,0.133974],[0.292893,0.292893],[0.133974,0.500000],[0.034074,0.741181],[0.000000,1.000000],[0.000000,10.000000],[0.034074,10.258819],[0.133974,10.500000],[0.292893,10.707107],[0.500000,10.866026],[0.741181,10.965926],[1.000000,11.000000],[10.000000,11.000000],[10.258819,10.965926],[10.500000,10.866026],[10.707107,10.707107],[10.866026,10.500000],[10.965926,10.258819],[11.000000,10.000000],[11.000000,7.000000],[11.048943,6.690983],[11.190983,6.412215],[11.412215,6.190983],[11.690983,6.048943],[12.000000,6.000000],[12.309017,6.048943],[12.587785,6.190983],[12.809017,6.412215],[12.951057,6.690983],[13.000000,7.000000],[52.100000,26.149996],[52.341699,26.091043],[52.585041,26.142797],[52.781829,26.295007],[52.893089,26.517526],[52.896784,26.766283],[52.792182,26.992009],[52.600000,27.149996],[52.249730,27.367646],[51.945385,27.645917],[51.697317,27.975346],[51.513963,28.344727],[51.401559,28.741496],[51.363929,29.152160],[51.402353,29.562751],[51.515522,29.959303],[51.699590,30.328328],[51.948294,30.657277],[52.253176,30.934960],[52.603866,31.151933],[52.988437,31.300816],[53.393808,31.376545],[53.806192,31.376545],[54.211563,31.300816],[54.596134,31.151933],[54.946824,30.934960],[55.251706,30.657277],[55.500410,30.328328],[55.684478,29.959303],[55.797647,29.562751],[55.836071,29.152160],[55.798441,28.741496],[55.686037,28.344727],[55.502683,27.975346],[55.254615,27.645917],[54.950270,27.367646],[54.600000,27.149996],[54.407815,26.992010],[54.303210,26.766283],[54.306904,26.517524],[54.418165,26.295003],[54.614956,26.142792],[54.858300,26.091040],[55.100000,26.149996],[55.565053,26.431803],[55.978457,26.785054],[56.329347,27.200464],[56.608501,27.667115],[56.808580,28.172740],[56.924328,28.704052],[56.952700,29.247084],[56.892953,29.787565],[56.746655,30.311288],[56.517652,30.804489],[56.211963,31.254204],[55.837623,31.648614],[55.404470,31.977351],[54.923890,32.231776],[54.408514,32.405201],[53.871887,32.493068],[53.328113,32.493068],[52.791486,32.405201],[52.276110,32.231776],[51.795530,31.977351],[51.362377,31.648614],[50.988037,31.254204],[50.682348,30.804489],[50.453345,30.311288],[50.307047,29.787565],[50.247300,29.247084],[50.275672,28.704052],[50.391420,28.172740],[50.591499,27.667115],[50.870653,27.200464],[51.221543,26.785054],[51.634947,26.431803]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21],[22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197],[198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273]]);}
spacing=73.500000;
pcbwidth=57.500000;
pcblength=37.000000;
// Parts to go on PCB (top)
module parts_top(part=false,hole=false,block=false){
translate([35.235000,5.790000,0.800000])translate([0.000000,-2.400000,0.000000])rotate([90.000000,-0.000000,-0.000000])m0(part,hole,block,casetop); // RevK:USB-C-Socket-H CSP-USC16-TR (back)
translate([44.450000,22.515000,0.800000])rotate([0,0,90.000000])m1(part,hole,block,casetop); // RevK:C_0603_ C_0603_1608Metric (back)
translate([5.500000,5.500000,0.800000])rotate([0,0,180.000000])m2(part,hole,block,casetop); // U2 (back)
translate([24.250000,9.150000,0.800000])rotate([0,0,90.000000])m3(part,hole,block,casetop); // J3 (back)
translate([26.050000,28.700000,0.800000])rotate([0,0,90.000000])m4(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([46.950000,19.215000,0.800000])rotate([-0.000000,-0.000000,-90.000000])m5(part,hole,block,casetop); // RevK:L_4x4_ TYA4020 (back)
translate([33.000000,20.000000,0.800000])m6(part,hole,block,casetop); // REF** (back)
translate([35.250000,24.000000,0.800000])m7(part,hole,block,casetop); // U1 (back)
translate([49.250000,35.250000,0.800000])rotate([0,0,-45.000000])m8(part,hole,block,casetop); // D4 (back)
translate([46.950000,21.815000,0.800000])m1(part,hole,block,casetop); // RevK:C_0603_ C_0603_1608Metric (back)
translate([49.450000,22.515000,0.800000])rotate([0,0,90.000000])m1(part,hole,block,casetop); // RevK:C_0603_ C_0603_1608Metric (back)
// Missing model U7.1 LX5563LL-TR
translate([20.600000,35.900000,0.800000])m9(part,hole,block,casetop); // U8 (back)
translate([37.250000,9.000000,0.800000])m10(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([53.600000,30.600000,0.800000])rotate([0,0,180.000000])m4(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([46.950000,25.715000,0.800000])m10(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([26.850000,28.700000,0.800000])rotate([0,0,90.000000])m4(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([44.450000,29.400000,0.800000])rotate([0,0,-90.000000])m4(part,hole,block,casetop); // RevK:C_0402 C_0402_1005Metric (back)
translate([43.650000,29.400000,0.800000])rotate([0,0,90.000000])m10(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([33.450000,9.000000,0.800000])rotate([0,0,180.000000])m10(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([45.150000,25.715000,0.800000])m10(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([46.000000,9.150000,0.800000])rotate([0,0,90.000000])m11(part,hole,block,casetop); // J2 (back)
translate([48.750000,25.715000,0.800000])m10(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
translate([46.950000,23.815000,0.800000])m12(part,hole,block,casetop); // RevK:SOT-23-6-MD8942 SOT-23-6 (back)
translate([38.850000,9.800000,0.800000])rotate([0,0,-90.000000])m13(part,hole,block,casetop); // D3 (back)
translate([30.650000,15.700000,0.800000])m10(part,hole,block,casetop); // RevK:R_0402 R_0402_1005Metric (back)
// Missing model D2.1 D_1206_3216Metric
}

parts_top=8;
// Parts to go on PCB (bottom)
module parts_bottom(part=false,hole=false,block=false){
}

parts_bottom=0;
module b(cx,cy,z,w,l,h){translate([cx-w/2,cy-l/2,z])cube([w,l,h]);}
module m0(part=false,hole=false,block=false,height)
{ // RevK:USB-C-Socket-H CSP-USC16-TR
// USB connector
rotate([-90,0,0])translate([-4.47,-3.84,0])
{
	if(part)
	{
		b(4.47,7,0,7,2,0.2);	// Pads
		translate([1.63,-0.2,1.63])
		rotate([-90,0,0])
		hull()
		{
			cylinder(d=3.26,h=7.55,$fn=24);
			translate([5.68,0,0])
			cylinder(d=3.26,h=7.55,$fn=24);
		}
		translate([0,6.2501,0])cube([8.94,1.1,1.6301]);
		translate([0,1.7,0])cube([8.94,1.6,1.6301]);
	}
	if(hole)
		translate([1.63,-20,1.63])
		rotate([-90,0,0])
	{
		// Plug
		hull()
		{
			cylinder(d=2.5,h=21,$fn=24);
			translate([5.68,0,0])
			cylinder(d=2.5,h=21,$fn=24);
		}
		hull()
		{
			cylinder(d=7,h=21,$fn=24);
			translate([5.68,0,0])
			cylinder(d=7,h=21,$fn=24);
		}
		translate([2.84,0,-100])
			cylinder(d=5,h=100,$fn=24);
	}
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

module m2(part=false,hole=false,block=false,height)
{ // U2
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

module m3(part=false,hole=false,block=false,height)
{ // J3
// WAGO-2060-453-998-404
N=3;
if(part)
{
	translate([-0.9,0,0])hull()
	{
		b(0,0,0,12.7,N*4-0.1,1);
		translate([0.8,0,0])b(0,0,0,11.1,N*4-0.1,4.5);
	}
	for(p=[0:N-1])translate([0,-4*(N-1)/2+p*4,2])
		rotate([90,0,-90])cylinder(d=3,h=20);
}
if(hole)
{
	for(p=[0:N-1])translate([0,-4*(N-1)/2+p*4,2])
		translate([-10,0,0])rotate([180,0,-90])cylinder(d=3,h=20);
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
{ // RevK:L_4x4_ TYA4020
// 4x4 Inductor
if(part)
{
	b(0,0,0,4,4,3);
}
}

module m6(part=false,hole=false,block=false,height)
{ // REF**
// Nothing
}

module m7(part=false,hole=false,block=false,height)
{ // U1
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

module m8(part=false,hole=false,block=false,height)
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

module m9(part=false,hole=false,block=false,height)
{ // U8
if(part)
{
	b(0,0,0,2,1.25,1);
}
}

module m10(part=false,hole=false,block=false,height)
{ // RevK:R_0402 R_0402_1005Metric
// 0402 Resistor
if(part)
{
	b(0,0,0,1.5,0.65,0.2); // Pad size
	b(0,0,0,1.0,0.5,0.5); // Chip
}
}

module m11(part=false,hole=false,block=false,height)
{ // J2
// WAGO-2060-452-998-404
N=2;
if(part)
{
	translate([-0.9,0,0])hull()
	{
		b(0,0,0,12.7,N*4-0.1,1);
		translate([0.8,0,0])b(0,0,0,11.1,N*4-0.1,4.5);
	}
	for(p=[0:N-1])translate([0,-4*(N-1)/2+p*4,2])
		rotate([90,0,-90])cylinder(d=3,h=20);
}
if(hole)
{
	for(p=[0:N-1])translate([0,-4*(N-1)/2+p*4,2])
		translate([-10,0,0])rotate([180,0,-90])cylinder(d=3,h=20);
}
}

module m12(part=false,hole=false,block=false,height)
{ // RevK:SOT-23-6-MD8942 SOT-23-6
// SOT-23-6
if(part)
{
	b(0,0,0,1.726,3.026,1.2); // Part
	b(0,0,0,3.6,2.5,0.5); // Pins
}
}

module m13(part=false,hole=false,block=false,height)
{ // D3
// SOD-123 Diode
if(part)
{
	b(0,0,0,2.85,1.8,1.35); // part
	b(0,0,0,4.2,1.2,0.7); // pads
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

// Generated case design for EnvMon/EnvMon.kicad_pcb
// By https://github.com/revk/PCBCase
// Generated 2023-10-09 10:20:34
// title:	EnvMon
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
useredge=true;

module outline(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[14.000000,0.000000],[56.500000,0.000000],[56.882683,0.076120],[57.207106,0.292893],[57.423879,0.617317],[57.500000,1.000000],[57.500000,36.000000],[57.423880,36.382683],[57.207107,36.707106],[56.882683,36.923879],[56.500000,37.000000],[14.000000,37.000000],[13.617317,36.923880],[13.292894,36.707107],[13.076121,36.382683],[13.000000,36.000000],[13.000000,1.000000],[13.076120,0.617317],[13.292893,0.292894],[13.617317,0.076121]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19]]);}

module pcb(h=pcbthickness,r=0){linear_extrude(height=h)offset(r=r)polygon(points=[[19.250000,0.250000],[19.250000,0.900000],[19.284074,1.158819],[19.383974,1.400000],[19.542893,1.607107],[19.750000,1.766026],[19.991181,1.865926],[20.250000,1.900000],[28.250000,1.900000],[28.508819,1.865926],[28.750000,1.766026],[28.957107,1.607107],[29.116026,1.400000],[29.215926,1.158819],[29.250000,0.900000],[29.250000,0.250000],[29.500000,0.000000],[42.000000,0.000000],[42.250000,0.250000],[42.250000,0.900000],[42.284074,1.158819],[42.383974,1.400000],[42.542893,1.607107],[42.750000,1.766026],[42.991181,1.865926],[43.250000,1.900000],[48.750000,1.900000],[49.008819,1.865926],[49.250000,1.766026],[49.457107,1.607107],[49.616026,1.400000],[49.715926,1.158819],[49.750000,0.900000],[49.750000,0.250000],[50.000000,0.000000],[56.500000,0.000000],[56.882684,0.076120],[57.207107,0.292893],[57.423880,0.617316],[57.500000,1.000000],[57.500000,36.000000],[57.423880,36.382684],[57.207107,36.707107],[56.882684,36.923880],[56.500000,37.000000],[47.750000,37.000000],[47.500000,36.750000],[47.500000,32.750000],[47.465926,32.491181],[47.366026,32.250000],[47.207107,32.042893],[47.000000,31.883974],[46.758819,31.784074],[46.500000,31.750000],[24.000000,31.750000],[23.741181,31.784074],[23.500000,31.883974],[23.292893,32.042893],[23.133974,32.250000],[23.034074,32.491181],[23.000000,32.750000],[23.000000,36.750000],[22.750000,37.000000],[14.000000,37.000000],[13.617316,36.923880],[13.292893,36.707107],[13.076120,36.382684],[13.000000,36.000000],[13.000000,34.000000],[12.965926,33.741181],[12.866025,33.500000],[12.707107,33.292893],[12.500000,33.133975],[12.258819,33.034074],[12.000000,33.000000],[11.741181,33.034074],[11.500000,33.133975],[11.292893,33.292893],[11.133975,33.500000],[11.034074,33.741181],[11.000000,34.000000],[11.000000,36.000000],[10.923880,36.382684],[10.707107,36.707107],[10.382684,36.923880],[10.000000,37.000000],[1.000000,37.000000],[0.617316,36.923880],[0.292893,36.707107],[0.076120,36.382684],[0.000000,36.000000],[0.000000,14.000000],[0.076120,13.617316],[0.292893,13.292893],[0.617316,13.076120],[1.000000,13.000000],[10.000000,13.000000],[10.382684,13.076120],[10.707107,13.292893],[10.923880,13.617316],[11.000000,14.000000],[11.000000,15.750000],[11.034074,16.008819],[11.133975,16.250000],[11.292893,16.457107],[11.500000,16.616025],[11.741181,16.715926],[12.000000,16.750000],[12.258819,16.715926],[12.500000,16.616025],[12.707107,16.457107],[12.866025,16.250000],[12.965926,16.008819],[13.000000,15.750000],[13.000000,7.000000],[12.965926,6.741181],[12.866025,6.500000],[12.707107,6.292893],[12.500000,6.133975],[12.258819,6.034074],[12.000000,6.000000],[11.741181,6.034074],[11.500000,6.133975],[11.292893,6.292893],[11.133975,6.500000],[11.034074,6.741181],[11.000000,7.000000],[11.000000,10.000000],[10.923880,10.382684],[10.707107,10.707107],[10.382684,10.923880],[10.000000,11.000000],[1.000000,11.000000],[0.617316,10.923880],[0.292893,10.707107],[0.076120,10.382684],[0.000000,10.000000],[0.000000,1.000000],[0.076120,0.617316],[0.292893,0.292893],[0.617316,0.076120],[1.000000,0.000000],[10.000000,0.000000],[10.382684,0.076120],[10.707107,0.292893],[10.923880,0.617316],[11.000000,1.000000],[11.034074,1.258819],[11.133975,1.500000],[11.292893,1.707107],[11.500000,1.866025],[11.741181,1.965926],[12.000000,2.000000],[12.258819,1.965926],[12.500000,1.866025],[12.707107,1.707107],[12.866025,1.500000],[12.965926,1.258819],[13.000000,1.000000],[13.076120,0.617316],[13.292893,0.292893],[13.617316,0.076120],[14.000000,0.000000],[19.000000,0.000000],[13.000000,21.750000],[13.000000,28.000000],[12.951057,28.309017],[12.809017,28.587785],[12.587785,28.809017],[12.309017,28.951057],[12.000000,29.000000],[11.690983,28.951057],[11.412215,28.809017],[11.190983,28.587785],[11.048943,28.309017],[11.000000,28.000000],[11.000000,21.750000],[11.048943,21.440983],[11.190983,21.162215],[11.412215,20.940983],[11.690983,20.798943],[12.000000,20.750000],[12.309017,20.798943],[12.587785,20.940983],[12.809017,21.162215],[12.951057,21.440983],[52.100000,26.149996],[52.341699,26.091043],[52.585041,26.142797],[52.781829,26.295007],[52.893089,26.517526],[52.896784,26.766283],[52.792182,26.992009],[52.600000,27.149996],[52.249730,27.367646],[51.945385,27.645917],[51.697317,27.975346],[51.513963,28.344727],[51.401559,28.741496],[51.363929,29.152160],[51.402353,29.562751],[51.515522,29.959303],[51.699590,30.328328],[51.948294,30.657277],[52.253176,30.934960],[52.603866,31.151933],[52.988437,31.300816],[53.393808,31.376545],[53.806192,31.376545],[54.211563,31.300816],[54.596134,31.151933],[54.946824,30.934960],[55.251706,30.657277],[55.500410,30.328328],[55.684478,29.959303],[55.797647,29.562751],[55.836071,29.152160],[55.798441,28.741496],[55.686037,28.344727],[55.502683,27.975346],[55.254615,27.645917],[54.950270,27.367646],[54.600000,27.149996],[54.407815,26.992010],[54.303210,26.766283],[54.306904,26.517524],[54.418165,26.295003],[54.614956,26.142792],[54.858300,26.091040],[55.100000,26.149996],[55.565053,26.431803],[55.978457,26.785054],[56.329347,27.200464],[56.608501,27.667115],[56.808580,28.172740],[56.924328,28.704052],[56.952700,29.247084],[56.892953,29.787565],[56.746655,30.311288],[56.517652,30.804489],[56.211963,31.254204],[55.837623,31.648614],[55.404470,31.977351],[54.923890,32.231776],[54.408514,32.405201],[53.871887,32.493068],[53.328113,32.493068],[52.791486,32.405201],[52.276110,32.231776],[51.795530,31.977351],[51.362377,31.648614],[50.988037,31.254204],[50.682348,30.804489],[50.453345,30.311288],[50.307047,29.787565],[50.247300,29.247084],[50.275672,28.704052],[50.391420,28.172740],[50.591499,27.667115],[50.870653,27.200464],[51.221543,26.785054],[51.634947,26.431803]],paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163],[164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185],[186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261]]);}
spacing=73.500000;
pcbwidth=57.500000;
pcblength=37.000000;
// Populated PCB
module board(pushed=false,hulled=false){
translate([44.750000,16.215000,0.800000])rotate([0,0,90.000000])m2(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([2.250000,34.750000,0.800000])m3(pushed,hulled); // Button_Switch_THT:SW_PUSH_6mm_H13mm SW_PUSH_6mm_H13mm (back)
translate([49.450000,33.675000,0.800000])rotate([0,0,-90.000000])m6(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([2.300000,19.750000,0.800000])m3(pushed,hulled); // Button_Switch_THT:SW_PUSH_6mm_H13mm SW_PUSH_6mm_H13mm (back)
translate([5.500000,5.500000,0.800000])rotate([0,0,180.000000])m7(pushed,hulled); // RevK:SCD41 SCD41 (back)
translate([24.250000,9.150000,0.800000])m9(pushed,hulled,3); // RevK:PTSM-HH-3-RA PTSM-HH-3-RA (back)
translate([26.050000,28.700000,0.800000])rotate([0,0,90.000000])m12(pushed,hulled); // RevK:C_0402 C_0402_1005Metric (back)
translate([47.250000,12.915000,0.800000])rotate([-0.000000,-0.000000,-90.000000])m15(pushed,hulled); // RevK:L_4x4_ TYA4020 (back)
translate([35.250000,4.400000,0.800000])translate([0.000000,-1.050000,0.000000])rotate([90.000000,-0.000000,-0.000000])m17(pushed,hulled); // RevK:USC16-TR CSP-USC16-TR (back)
translate([50.250000,33.675000,0.800000])rotate([0,0,-90.000000])m6(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([35.250000,24.000000,0.800000])m18(pushed,hulled); // RevK:ESP32-S3-MINI-1 ESP32-S3-MINI-1 (back)
translate([47.250000,15.515000,0.800000])m2(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([49.750000,16.215000,0.800000])rotate([0,0,90.000000])m2(pushed,hulled); // RevK:C_0603_ C_0603_1608Metric (back)
translate([53.600000,29.150000,0.800000])rotate([0,0,90.000000])m21(pushed,hulled); // RevK:Sensirion_DFN-4_1.5x1.5mm_P0.5mm_SHT4x LX5563LL-TR (back)
translate([20.600000,35.900000,0.800000])m23(pushed,hulled); // RevK:VEML6040 VEML3235SL (back)
translate([37.250000,9.000000,0.800000])m6(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([53.600000,30.600000,0.800000])rotate([0,0,180.000000])m12(pushed,hulled); // RevK:C_0402 C_0402_1005Metric (back)
translate([47.250000,19.415000,0.800000])m6(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([57.500000,37.000000,0.800000])rotate([0,0,180.000000])translate([44.500000,37.000000,0.000000])rotate([-0.000000,-0.000000,-180.000000])m28(pushed,hulled); // RevK:OLED1.5-RGB OLED15 (back)
translate([26.850000,28.700000,0.800000])rotate([0,0,90.000000])m12(pushed,hulled); // RevK:C_0402 C_0402_1005Metric (back)
translate([44.450000,29.400000,0.800000])rotate([0,0,-90.000000])m12(pushed,hulled); // RevK:C_0402 C_0402_1005Metric (back)
translate([43.650000,29.400000,0.800000])rotate([0,0,90.000000])m6(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([49.450000,35.525000,0.800000])rotate([0,0,180.000000])translate([0.000000,-0.400000,0.000000])m31(pushed,hulled); // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric (back)
translate([49.450000,35.525000,0.800000])rotate([0,0,180.000000])translate([0.000000,0.400000,0.000000])m31(pushed,hulled); // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric (back)
translate([33.450000,9.000000,0.800000])rotate([0,0,180.000000])m6(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([45.450000,19.415000,0.800000])m6(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([46.000000,9.150000,0.800000])m34(pushed,hulled,2); // RevK:PTSM-HH-2-RA-W PTSM-HH-2-RA-W (back)
translate([2.250000,27.250000,0.800000])m3(pushed,hulled); // Button_Switch_THT:SW_PUSH_6mm_H13mm SW_PUSH_6mm_H13mm (back)
translate([49.050000,19.415000,0.800000])m6(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([47.250000,17.515000,0.800000])m37(pushed,hulled); // RevK:SOT-23-6-MD8942 SOT-23-6 (back)
translate([25.500000,11.400000,0.800000])m6(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
translate([41.450000,9.700000,0.800000])rotate([0,0,-90.000000])m38(pushed,hulled); // RevK:D_SOD-123 D_SOD-123 (back)
translate([48.650000,33.675000,0.800000])rotate([0,0,-90.000000])m6(pushed,hulled); // RevK:R_0402 R_0402_1005Metric (back)
}

module b(cx,cy,z,w,l,h){translate([cx-w/2,cy-l/2,z])cube([w,l,h]);}
module m2(pushed=false,hulled=false)
{ // RevK:C_0603_ C_0603_1608Metric
b(0,0,0,1.6,0.95,0.2); // Pad size
b(0,0,0,1.6,0.8,1); // Chip
}

module m3(pushed=false,hulled=false)
{ // Button_Switch_THT:SW_PUSH_6mm_H13mm SW_PUSH_6mm_H13mm
translate([3.25,-2.25,0])
{
	b(0,0,0,6,6,4);
	if(!hulled&&pushed)cylinder(d=4,h=100);
	for(x=[-3.25,3.25])for(y=[-2.25,2.25])translate([x,y,-2])cylinder(d=2,h=4);
}
}

module m6(pushed=false,hulled=false)
{ // RevK:R_0402 R_0402_1005Metric
b(0,0,0,1.5,0.65,0.2); // Pad size
b(0,0,0,1.0,0.5,0.5); // Chip
}

module m7(pushed=false,hulled=false)
{ // RevK:SCD41 SCD41
b(0,0,0,10.1,10.1,0.8);
b(0,0,0.3,8.5,8.5,6);
if(!pushed&&!hulled)translate([0,0,2.5])for(a=[[0,0,0],[90,0,0],[-90,0,0],[0,90,0]])rotate(a)for(x=[-2,2])for(y=[-2,2])translate([x,y,0])cylinder(d=2,h=10,$fn=8);
}

module m9(pushed=false,hulled=false,n=0)
{ // RevK:PTSM-HH-3-RA PTSM-HH-3-RA
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

module m12(pushed=false,hulled=false)
{ // RevK:C_0402 C_0402_1005Metric
b(0,0,0,1.5,0.65,0.2); // Pad size
b(0,0,0,1.0,0.5,1); // Chip
}

module m15(pushed=false,hulled=false)
{ // RevK:L_4x4_ TYA4020
b(0,0,0,4,4,3);
}

module m17(pushed=false,hulled=false)
{ // RevK:USC16-TR CSP-USC16-TR
rotate([-90,0,0])translate([-4.47,-3.84,0])
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
	if(!hulled)
	{
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

}

module m18(pushed=false,hulled=false)
{ // RevK:ESP32-S3-MINI-1 ESP32-S3-MINI-1
translate([-15.4/2,-15.45/2,0])
{
	if(!hulled)cube([15.4,20.5,0.8]);
	translate([0.7,0.5,0])cube([14,13.55,2.4]);
}
}

module m21(pushed=false,hulled=false)
{ // RevK:Sensirion_DFN-4_1.5x1.5mm_P0.5mm_SHT4x LX5563LL-TR
b(0,0,0,1.5,1.5,0.5);
if(!pushed&&!hulled)cylinder(d=1.5,h=20);
}

module m23(pushed=false,hulled=false)
{ // RevK:VEML6040 VEML3235SL
b(0,0,0,2.95,1.5,1.5);
hull(){b(0,0,1.49,2.95,1.5,1);b(0,0,10,29.5,15,1);}

}

module m28(pushed=false,hulled=false)
{ // RevK:OLED1.5-RGB OLED15
translate([44.5,37,0])rotate([0,0,180])
{
	translate([0,0,6])hull()for(x=[1.5,44.5-1.5])for(y=[1.5,37-1.5])translate([x,y,0])cylinder(r=1.4995,h=1.599,$fn=24);
	for(x=[2.5,44.5-2.5])for(y=[2.5,37-2.5])translate([x,y,0])cylinder(d=4.98,h=6,$fn=6);
	for(x=[2.5,44.5-2.5])for(y=[2.5,37-2.5])translate([x,y,7])cylinder(d=3.99,h=1.5,$fn=24);
	for(x=[2.5,44.5-2.5])for(y=[2.5,37-2.5])translate([x,y,-3])cylinder(d=3.99,h=2,$fn=24);
	translate([5.25,0,6])cube([34,37,3.2]);
	translate([40.73,9.61,7.5])cube([2.54,7*2.54,1.5]); // pins
	if(!hulled)hull()
	{
		translate([8.25,2,9.199]) cube([28,28,20]);
		if(!pushed) translate([4.25,-2,13.5]) cube([36,36,20]);
	}
}
}

module m31(pushed=false,hulled=false)
{ // RevK:LED-RGB-1.6x1.6 LED_0603_1608Metric
b(0,0,0,1.6,0.8,0.25);
b(0,0,0,1.2,0.8,0.55);
b(0,0,0,0.8,0.8,0.95);
if(!hulled&&pushed)b(0,0,0,1,1,20);
}

module m34(pushed=false,hulled=false,n=0)
{ // RevK:PTSM-HH-2-RA-W PTSM-HH-2-RA-W
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

module m37(pushed=false,hulled=false)
{ // RevK:SOT-23-6-MD8942 SOT-23-6
b(0,0,0,3.05,3.05,0.5);
b(0,0,0,1.45,3.05,1.1);
}

module m38(pushed=false,hulled=false)
{ // RevK:D_SOD-123 D_SOD-123
b(0,0,0,3.85,0.65,0.7);
b(0,0,0,2.85,1.8,1.35);
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

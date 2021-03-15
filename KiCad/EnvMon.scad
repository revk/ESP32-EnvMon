// Generated case design for ../ESP32-EnvMon/KiCad/EnvMon.kicad_pcb
// By https://github.com/revk/PCBCase
// title:	EnvMon
// date:	2021-03-14
// rev:	2.000000
// comment:	www.me.uk
// comment:	@TheRealRevK
// comment:	Adrian Kennard Andrews & Arnold Ltd
//

// Globals
debug=true;
margin=0.800000;
casebase=5.000000;
casetop=5.000000;
casewall=3.000000;
fit=0.100000;
edge=2.000000;
pcbthickness=1.600000;
pcbwidth=44.500000;
pcblength=37.000000;

// PCB
module pcb(){linear_extrude(height=1.600000)polygon([[39.250000,31.250000],[39.250000,37.000000],[44.500000,37.000000],[44.500000,0.000000],[0.000000,0.000000],[0.000000,37.000000],[11.250000,37.000000],[11.250000,31.250000]]);}

// Populated PCB
module board(){
	pcb();
translate([37.100000,30.150000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m0(); // RevK:SCD30-4pin
translate([37.100000,30.150000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m1(); // RevK:SCD30-4pin
translate([44.500000,37.000000,1.600000])rotate([0,0,180.000000])m2(); // RevK:OLED1.5-RGB
translate([44.500000,37.000000,1.600000])rotate([0,0,180.000000])m3(); // RevK:OLED1.5-RGB
translate([36.550000,3.050000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m4(); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder
translate([34.050000,3.050000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m4(); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder
translate([31.550000,3.050000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m4(); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder
translate([29.050000,3.050000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m4(); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder
translate([26.550000,3.050000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m4(); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder
translate([24.050000,3.050000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m4(); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder
translate([21.550000,3.050000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m4(); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder
translate([8.490000,3.050000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m4(); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder
translate([11.030000,3.050000,0.000000])rotate([0,0,270.000000])rotate([180,0,0])m4(); // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder
translate([25.250000,21.000000,1.600000])m5(); // RF_Module:ESP32-WROOM-32
translate([9.725000,11.525000,1.600000])m6(); // Package_DFN_QFN:QFN-16-1EP_4x4mm_P0.65mm_EP2.1x2.1mm
translate([8.750000,14.500000,1.600000])m7(); // RevK:D24V5F3-NoSHDN
translate([19.050000,6.850000,1.600000])m8(); // Connector_Molex:Molex_SPOX_5268-08A_1x08_P2.50mm_Horizontal
translate([9.750000,0.005000,1.600000])m9(); // RevK:USC16-TR-Round
}

module m0()
{ // RevK:SCD30-4pin
translate([0,0,4])cube([23,35,1.6]);
cube([2.54,17.78,5]);
translate([6,0,0])cube([17,35,7]);

}

module m1()
{ // RevK:SCD30-4pin
translate([-1.27,-1.27,-3])cube([2.54,10.16,100]);

}

module m2()
{ // RevK:OLED1.5-RGB
translate([-1.27,-1.27,-3])cube([2.54,17.78,100]);

}

module m3()
{ // RevK:OLED1.5-RGB
{
	translate([0,0,6])cube([44.4999,37,1.5999]);
	translate([5,0,6])cube([33.5,37,3.2]);
	translate([2.5,2.5,0])cylinder(d=5,h=6,$fn=7);
	translate([2.5,37-2.5,0])cylinder(d=5,h=6,$fn=7);
	translate([44.5-2.5,2.5,0])cylinder(d=5,h=6,$fn=7);
	translate([44.5-2.5,37-2.5,0])cylinder(d=5,h=6,$fn=7);
	translate([2.5,2.5,7])cylinder(d=4.99,h=1.6,$fn=24); // screws
	translate([2.5,37-2.5,7])cylinder(d=4.99,h=1.6,$fn=24);
	translate([44.5-2.5,2.5,7])cylinder(d=4.99,h=1.6,$fn=24);
	translate([44.5-2.5,37-2.5,7])cylinder(d=4.99,h=1.6,$fn=24); 
	translate([2.5,2.5,-2.6])cylinder(d=4.99,h=1.6,$fn=24); // screws
	translate([2.5,37-2.5,-2.6])cylinder(d=4.99,h=1.6,$fn=24);
	translate([44.5-2.5,2.5,-2.6])cylinder(d=4.99,h=1.6,$fn=24);
	translate([44.5-2.5,37-2.5,-2.6])cylinder(d=4.99,h=1.6,$fn=24); 
	translate([40.73,9.61,7.5])cube([2.54,7*2.54,1.5]); // pins
	translate([8.75,1.75,7.5])cube([27,27,20]); // Display view
}
}

module m4()
{ // Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder
cube([4.9,2.23,1.5],center=true);
}

module m5()
{ // RF_Module:ESP32-WROOM-32
translate([-9,-9.75,0])
{
	cube([18,25.50,0.80]);
	translate([1.1,1.05,0])cube([15.80,17.60,3.10]);
	translate([-0.5,0.865,0])cube([19,17.4,0.8]);
	translate([2.65,-0.5,0])cube([12.7,2,0.8]);

}
}

module m6()
{ // Package_DFN_QFN:QFN-16-1EP_4x4mm_P0.65mm_EP2.1x2.1mm
cube([4,4,1],center=true);
}

module m7()
{ // RevK:D24V5F3-NoSHDN
translate([-5.1,0,0])cube([10.2,12.7,2.82]);

}

module m8()
{ // Connector_Molex:Molex_SPOX_5268-08A_1x08_P2.50mm_Horizontal
translate([-2.5,-6.6,0])
{
	cube([22.4,4.9,4.9]);
	cube([22.4,5.9,3.9]);
	hull()
	{
		cube([22.4,7.4,1]);
		cube([22.4,7.9,0.5]);
	}
	translate([1,6,-3.5])cube([20.4,1.2,6]);
	// Plug
	translate([0.5,-20,0.7])cube([21.4,21,3.9]);
	translate([0,-23,0])cube([22.4,20,4.9]);
}

}

module m9()
{ // RevK:USC16-TR-Round
translate([-4.47,-0.49,0])
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

height=casebase+pcbthickness+casetop;

module boardf()
{ // This is the board, but stretched up to make a push out in from the front
	render()
	{
		intersection()
		{
			translate([-casewall-1,-casewall-1,-casebase-1]) cube([pcbwidth+casewall*2+2,pcblength+casewall*2+2,height+2]);
			minkowski()
			{
				board();
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
				board();
				translate([0,0,-height-100])
				cylinder(h=height+100,d=margin,$fn=8);
			}
		}
	}
}

module pyramid()
{ // A pyramid
 polyhedron(points=[[0,0,0],[-height,-height,height],[-height,height,height],[height,height,height],[height,-height,height]],faces=[[0,1,2],[0,2,3],[0,3,4],[0,4,1],[4,3,2,1]]);
}

module cutf()
{ // This cut up from base in the wall
	intersection()
	{
		boardf();
		difference()
		{
			translate([-casewall+0.01,-casewall+0.01,-casebase+0.01])cube([pcbwidth+casewall*2-0.02,pcblength+casewall*2-0.02,casebase+pcbthickness]);
			translate([-0.01-margin/2,-0.01-margin/2,-casebase-1])cube([pcbwidth+margin+0.02,pcblength+margin+0.02,height+2]);
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
			translate([-0.01-margin/2,-0.01-margin/2,-casebase-1])cube([pcbwidth+margin+0.02,pcblength+margin+0.02,height+2]);
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
			translate([-casewall-0.01,-casewall-0.01,-casebase-0.01])cube([pcbwidth+casewall*2+0.02,pcblength+casewall*2+0.02,casebase+pcbthickness+0.02]);
			translate([0.01-margin/2,0.01-margin/2,-casebase-1])cube([pcbwidth+margin-0.02,pcblength+margin+0.02,height+2]);
			board();
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
			translate([0.01-margin/2,0.01-margin/2,-casebase-1])cube([pcbwidth+margin-0.02,pcblength+margin+0.02,height+2]);
			board();
		}
		translate([-casewall,-casewall,-casebase])case();
	}
}


module case()
{ // The basic case
	hull()
	{
		translate([edge,0,edge])
		cube([pcbwidth+casewall*2-edge*2,pcblength+casewall*2,height-edge*2]);
		translate([0,edge,edge])
		cube([pcbwidth+casewall*2,pcblength+casewall*2-edge*2,height-edge*2]);
		translate([edge,edge,0])
		cube([pcbwidth+casewall*2-edge*2,pcblength+casewall*2-edge*2,height]);
	}
}

module base()
{ // The base
	difference()
	{
		case();
		translate([-1,-1,casebase+pcbthickness])cube([pcbwidth+casewall*2+2,pcblength+casewall*2+2,casetop+1]);
		translate([casewall/2-fit/2-margin/4,casewall/2-fit/2-margin/4,casebase])cube([pcbwidth+casewall+fit+margin/2,pcblength+casewall+fit+margin/2,casetop+pcbthickness+1]);
		translate([casewall,casewall,casebase-fit])boardf();
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
				translate([-1,-1,-1])cube([pcbwidth+casewall*2+2,pcblength+casewall*2+2,casebase+pcbthickness+1]);
				translate([casewall/2-margin/4+fit/2,casewall/2-margin/4+fit/2,casebase])cube([pcbwidth+casewall+margin/2-fit,pcblength+casewall+margin/2-fit,casetop+pcbthickness+1]);
			}
			translate([casewall,casewall,casebase+fit])boardb();
			translate([casewall,casewall,casebase])cutpb();
		}
		translate([casewall,casewall,casebase])cutpf();
	}
}

module test()
{
	board();
	translate([1*(pcbwidth+casewall+10),0,0])boardf();
	translate([2*(pcbwidth+casewall+10),0,0])boardb();
	translate([3*(pcbwidth+casewall+10),0,0])cutpf();
	translate([4*(pcbwidth+casewall+10),0,0])cutpb();
	translate([5*(pcbwidth+casewall+10),0,0])cutf();
	translate([6*(pcbwidth+casewall+10),0,0])cutb();
	translate([7*(pcbwidth+casewall+10),0,0])case();
	translate([8*(pcbwidth+casewall+10),0,0])base();
	translate([9*(pcbwidth+casewall+10),0,0])top();
}

module parts()
{
	base();
	translate([pcbwidth+casewall+10,0,0])top();
}

if(debug)test();
else parts();

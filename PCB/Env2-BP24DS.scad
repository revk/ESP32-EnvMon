use <blankplate.scad>
use <LCD24.scad>
use <Env2/EnvMon.scad>

difference()
{
    union()
    {
        blankplate();
        translate([-24.75,-38.25,0])cube([49.5,76.5,8]);
    }
    translate([-21.75-0.2,-35.25-0.2,4])cube([43.5+0.4,70.5+0.4,10]);   
    translate([0,0,11])rotate([180,0,90])lcd24(0.4);
    translate([-8.89-24.1,55.65-32.85,5.2])rotate([0,0,-90])minkowski()
    {
       cube([0.1,0.1,10]);
       pcb();
    }
    translate([-30,-39,5])cube([10,5,10]);
    translate([-36,-30,5])rotate([90,0,-90])hull()
    {
        cylinder(d=6,h=100,$fn=24);
        translate([0,10,0])cylinder(d=6,h=100,$fn=24);
    }
    for(y=[0:5:20])translate([-30,y+10,-1])cylinder(d=3,h=10,$fn=24);
}

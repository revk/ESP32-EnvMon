use <blankplate.scad>
use <LCD24.scad>
use <Env2/EnvMon.scad>

difference()
{
    union()
    {
        blankplate();
        translate([-24.75,-38.25,0])cube([49.5,76.5,11.5]);
    }
    translate([-21.75-0.2,-35.25-0.2,4])cube([43.5+0.4,70.5+0.4,10]);   
    translate([0,0,11])rotate([180,0,90])lcd24(0.4);
    translate([-8.89-24.1,55.65-32.85,5.2])rotate([0,0,-90])minkowski()
    {
       pcb();
       cube([0.1,0.1,10]);
    }
}

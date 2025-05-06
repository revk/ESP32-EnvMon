use <blankplate.scad>
use <Env2/EnvMonBlind.scad>

difference()
{
    union()
    {
        blankplate();
        translate([-25,-25,0])cube([45,65,5]);
    }
    translate([-8.89-24.1,55.65-32.85+15,3])rotate([0,0,-90])minkowski()
    {
       pcb();
       cube([0.3,0.3,10]);
    }
    translate([-36,-30,5])rotate([90,0,-90])hull()
    {
        cylinder(d=6,h=100,$fn=24);
        translate([0,10,0])cylinder(d=6,h=100,$fn=24);
    }
    for(y=[0:5:20])translate([-30,y+10,-1])cylinder(d=3,h=10,$fn=24);    
}

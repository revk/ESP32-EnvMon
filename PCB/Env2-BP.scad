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
}

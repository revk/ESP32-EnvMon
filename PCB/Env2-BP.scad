use <blankplate.scad>
use <Env2/EnvMonBlind.scad>

difference()
{
    union()
    {
        blankplate();
        translate([-24.75,-38.25,0])cube([49.5,76.5,8]);
    }
    #translate([-8.89-24.1,55.65-32.85+15,5.2])rotate([0,0,-90])minkowski()
    {
       pcb();
       cube([0.1,0.1,10]);
    }
}

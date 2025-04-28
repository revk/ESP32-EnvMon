use <lcd2.scad>
use <Env2/EnvMonCO2.scad>

module case(r=2)
{
translate([-29,-17.5,r])
minkowski()
{
    sphere(r=r,$fn=24);
    cube([58,35,10]);
}
}

difference()
{
    case();
    translate([0,0,5.5])case(2.001);
    translate([0,0,12])rotate([180,0,0])lcd2(0.2);
}

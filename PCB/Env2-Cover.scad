use <lcd2.scad>

module case(r=3)
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
    translate([0,0,4.5])case(3.001);
    translate([0,0,11])rotate([180,0,0])lcd2(0.4);
}
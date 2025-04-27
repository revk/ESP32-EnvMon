// 2" LCD
module lcd24(m)
{
    translate([-35.25,-21.65,0]) // Centre
    {
        minkowski()
        {
            if(m>0)sphere(m);
            union()
            {
                // Main PCB
                hull()for(x=[2.5,68])for(y=[2.5,40.8])translate([x,y,6])cylinder(r=2.5,h=1.5,$fn=24);
                // Display
                translate([5.1,0.3,6])cube([60.3,42.7,4]);
                // Ribbon
                translate([3.5,11.65,6])cube([2,20,3.5]);
                translate([3.5,4,6])cube([2,2,3.5]);
                // Screws / pillars
                for(x=[2.5,68])for(y=[2.5,40.85])translate([x,y,0])
                {
                    translate([0,0,7.5])cylinder(d=4,h=1.5,$fn=24);
                    translate([0,0,-2.4])cylinder(d=4,h=1.5,$fn=24);
                    translate([0,0,-2])cylinder(d=2,h=3,$fn=24);
                    cylinder(d=2.5,h=6,$fn=24);
                }
                // Pins
                for(x=[0:1:7])translate([2.4,12.76+x*2.54,0])
                {
                    translate([0,0,-2])cube([.254,.254,11]);
                    translate([0,0,7.5])cylinder(d1=1.5,d2=0.254,h=1.5,$fn=24);
                }
                // Cable block
                translate([59,11.65,0])cube([6.5,20,6]);
                translate([56.5,11.65,3])cube([2.5,1,3]);
                translate([56.5,30.65,3])cube([2.5,1,3]);
                translate([53,13.15,5])cube([3.5,17,1]);
                // Components
                translate([35,18.15,5])cube([14,7,1]);
            }
        }
        // Viewing
        translate([35.25+2.5,21.65,10])hull(){cube([50,38,1],center=true);translate([0,0,10])cube([60,48,1],center=true);}
    }
}
lcd24(0);

// 2" LCD
module lcd2(m)
{
    translate([-29,-17.5,0]) // Centre
    {
        minkowski()
        {
            if(m>0)sphere(m);
            union()
            {
                // Main PCB
                hull()for(x=[2.5,55.5])for(y=[2.5,32.5])translate([x,y,6])cylinder(r=2.5,h=1.5,$fn=24);
                // Display
                translate([5.085,0.2,6])cube([47.83,34.6,4]);
                // Ribbon
                translate([3.5,7.5,6])cube([2,20,3.5]);
                translate([3.5,4,6])cube([2,2,3.5]);
                // Screws / Pillars
                for(x=[2.5,55.5])for(y=[2.5,32.5])translate([x,y,0])
                {
                    translate([0,0,7.5])cylinder(d=4,h=1.5,$fn=24);
                    translate([0,0,-2.4])cylinder(d=4,h=1.5,$fn=24);
                    translate([0,0,-2])cylinder(d=2,h=3,$fn=24);
	    	    cylinder(d=2.5,h=6,$fn=24);
                }
                // Pins
                for(x=[0:1:7])translate([2.4,8.61+x*2.54,0])
                {
                    translate([0,0,-2])cube([.254,.254,11]);
                    translate([0,0,7.5])cylinder(d1=1.5,d2=0.254,h=1.5,$fn=24);
                }
                // Cable block
                translate([47.5,7.5,0])cube([6.5,20,6]);
                translate([45,7.5,3])cube([2.5,1,3]);
                translate([45,26.5,3])cube([2.5,1,3]);
                translate([44,9,5])cube([3.5,17,1]);
                // Components
                translate([32,12,5])cube([4,11,1]);
            }
        }
        // Viewing
        translate([29+1.5,17.5,10])hull(){cube([42,32,1],center=true);translate([0,0,10])cube([52,42,1],center=true);}
    }
}
lcd2(0);

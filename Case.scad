// 3D case for encironmental monitor
// Copyright (c) 2019 Adrian Kennard, Andrews & Arnold Limited, see LICENSE file (GPL)

use <PCBCase/case.scad>

display=true; // has display

width=45;
height=37;

// Box thickness reference to component cube
base=(display?13:8)+0.4;
top=5;

$fn=48;

module pcb(s=0)
{
    translate([-1,-1,0])
    { // 1mm ref edge of PCB vs SVG design
        esp32(s,26.170,10.448,-90);        
        if(display)
        {
            oled(s,1,1,d=6.8,h=9,nopads=true);
            pads(5.443,9.89,ny=4);
        }
        d24v5f3(s,30,21,180);
        spox(s,1,19.715,-90,4,hidden=true);
        spox(s,30,1,0,3);
        usbc(s,13.065,0.510,0);
        co2(s,7,2);
        smd1206(s,13.735,9.010);
        smd1206(s,18.135,9.010);
        pads(34.05,35.287,nx=2);
        switch66(s,22.940,2.270,90);
        // DC power cable.
        translate([32.598,36.560-4,-2-1.6])
        cube([5.192,1.44+4,2]);
        translate([32.598,36.560,-2-1.6])
        cube([5.192,1.44,22+1.6]);
    }
}

case(width,height,base,top,cutoffset=-5){pcb(0);pcb(-1);pcb(1);};


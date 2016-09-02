General board for MIB projects. MIDI in (via optocoupler)
and out (via optional buffer), GVS pin groups for easy connection
of potentiometers, switches and LEDs to A0-A3 and digital pins
10, 11 and 12. Optional battery power supply part with soldered
in charge/step-up board.

Component list:

1x Arduino Pro Mini
1x 6N137, 6N138 or 6N139 optocoupler, DIP (OK2)
1x 74LS14 buffer/inverter, DIP, optional (IC1)
1x 1N4148 diode, mind the orientation (D1)
2x 2K2 resistor (if 6N137, just one) (R3, R5*)
3x 220 ohm resistor (R1, R2, R4)
2x PCB mount DIN-5 MIDI PCB connector (X1, X2)
1x Slide switch DPDT (2.54 mm pin separation) (S1)
Breakable pin headers, black, red, white for GVS

Optional power circuit:

Battery holder 16340
Li-Ion battery 3.7V 16340
All-in-one charge and step-up board (r.ebay.com-uWUNOm)

VOUT +/- connects to PWR +/- of main circuit via a switch.
VIN +/- connects to panel mount power jack for 5V input.

Notes for assembling:

If the 74LS14 DIP is installed, the jumper pads marked <<>> are
left unconnected. If DIP is not installed, bridge the pads with
a jumper to connect the TX line to the output.

If the optocoupler selected for the build is a 6N137, the 2K2
resistor where PCB is marked >137 (R5) is omitted.

The slide switch on the board is for disconnecting the TX and RX
lines when uploading new programs to the Arduino. If you can't find
a DPDT switch that fits, you could go with single slide switches or
probably DIP switches too. Or jumpers.


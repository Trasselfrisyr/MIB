General board for MIB projects. MIDI in (via optocoupler)
and out (via buffer), GVS pin groups for easy connection
of potentiometers, switches and LEDs to A0-A3 and digital
pins 10 through 13 on Pro Mini. Pins 10, 15, 14 and 15 on
Pro Micro. LED connectors with resistors on pins 5 and 6.
Board power is supplied via the USB port on the Pro Micro
and by supplying regulated 5V DC power to VCC/GND if using
Pro Mini. A micro USB board connected to the dedicated pins
on the GVS strip closest to the reset button, and a USB power
adapter is a good way to do this.


Component list:

1x Arduino Pro Mini or Pro Micro (socket recommended)
1x 6N138 or 6N139 optocoupler, DIP (socket recommended)
1x 74HC125 buffer, negative enable, DIP (socket recommended)
1x 1N4148 diode, mind the orientation
2x 2K2 resistor
5x 220 ohm resistor
1x 10K resistor
2x 100nF ceramic capacitor
2x PCB mount DIN-5 MIDI PCB connector
1x Tact Switch 6x6 mm
Breakable pin headers, black, red, white/yellow for GVS
Pin headers in black and green or blue for LED connectors,
and black headers for the Arduino, to plug it into the socket
(for a snug fit agains the socket, mount the header upside
down and cut the excess off the top after soldering.)


Notes:
The board uses the 74HC125 to disconnect the MIDI circuits for
problem free reprogramming of the Pro Mini. To enable the MIDI
connection, you must set pin 2 low in the Arduino sketch.

pinMode(2,OUTPUT);
digitalWrite(2,LOW); // set pin 2 low to enable DIN MIDI

This is if you write your own code. In the MIB code projects
available on github, this is already done.
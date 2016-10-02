# MIB
MIDI Injector Box, merge controller data to MIDI

Makes use of the the FortySevenEffects Arduino MIDI Library, so make sure you've got that one.

For hardware, you can build the board from plans in the MIB-board folders to use together with a Pro Mini or use any MIDI shield together with an Arduino Uno.

Breath-Injector.ino - adds breath controller data to DIN-5 MIDI from a keyboard, great for creating a MIDI melodica. For use with breath controller synth patches responding to CC#2 (breath). Can easily be changed to send other CC data like volume or mod wheel by changing the CC_Number definition in the code.

Breath-Injector-ProMicroArcore.ino - version of breath injector above, using a Pro Micro and the Arcore arduino core for USB MIDI support. This version also transmits the breath data to USB. Made for the MIB mkII board.

Volume-Injector.ino - adds volume cc data. Build your own MIDI volume pedal perhaps?


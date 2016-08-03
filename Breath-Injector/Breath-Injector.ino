#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

/*
NAME:                 MIB Breath Injector
WRITTEN BY:           JOHAN BERGLUND
DATE:                 2016-07-13
FILE SAVED AS:        Breath-Injector.ino
FOR:                  Arduino Pro Mini, ATmega328
CLOCK:                16.00 MHz CRYSTAL                                        
PROGRAMME FUNCTION:   Input MIDI, add breath controller data, output merged data to MIDI
                      Freescale MPXV5004GP breath sensor connected for breath control
                      Optional dip switch for disconnecting RX line from MIDI 

HARDWARE NOTES:
* For the MIDI connection, attach a MIDI out Female 180 Degree 5-Pin DIN socket to Arduino.
* Socket is seen from solder tags at rear.
* 
* DIN-5 pinout for MIDI output is:                         _______ 
*    pin 2 - GND                                          /       \
*    pin 4 - 220 ohm resistor to +5V                     | 1     3 |  MIDI jack
*    pin 5 - Arduino Pin 1 (TX) via a 220 ohm resistor   |  4   5  |
*    all other pins - unconnected                         \___2___/
*    
* DIN-5 pinout for MIDI input is:                          _______ 
*    pin 2 - GND                                          /       \
*    pin 4 - 220 ohm resistor to pin 2 of 6N139          | 1     3 |  MIDI jack
*    pin 5 - to pin 3 of 6N139                           |  4   5  |
*    all other pins - unconnected                         \___2___/
*
* 6N139 optocoupler connections:
*    pin 8 - to +5V
*    pin 7 - via 1k8 ohm resistor to GND
*    pin 6 - to Arduino pin 0 (RX) via optional disconnection switch
*    pin 5 - GND
*    pin 4 - n/c
*    pin 3 - MIDI jack pin 5
*    pin 2 - MIDI jack pin 4 via 220 ohm resistor
*    pin 1 - n/c
*    also connected, a 1N4148 diode from pin 3 to pin 2, 
*    a 1k8 ohm resistor between pins 6 and 8,
*    and a 100 nF capacitor between pins 8 and 5
*      
* The Freescale MPXV5004GP pressure sensor output (V OUT) is connected to Arduino pin A3.
* 
* Sensor pinout
* 1: n/c
* 2: V OUT 
* 3: GND
* 4: VCC (to 5V)    
* 5: n/c
* 6: n/c
* 7: n/c
* 8: n/c
*     
* For a proper schematic of all this, see github repository at https://github.com/Trasselfrisyr/MIB/Breath-Injector/
*/



//_______________________________________________________________________________________________ DECLARATIONS

#define ON_Thr 300       // Set threshold level before switching ON
#define breath_max 1023  // Upper limit for pressure
#define CC_INTERVAL 10  // Interval for sending CC data
#define CC_Number 2     // Controller number (2=Breath, 1=Mod Wheel, 7=Volume, 11=Expression, 74=Cutoff)

unsigned long ccSendTime = 0L;     // The last time we sent CC values

int pressureSensor;  // pressure data from breath sensor, for midi breath cc and breath threshold checks
byte itsOn=0;        // keep track and make sure we send CC with 0 value when off threshold

byte x;
byte LedPin=13;    // select the pin for the LED
byte channel=1;    // MIDI channel

//_______________________________________________________________________________________________ SETUP

void setup() {
  pinMode(LedPin,OUTPUT);

  MIDI.begin(channel);
  
  for (x=1; x<=4; x++){  // Do the flashy-flashy to say we are up and running
    digitalWrite( LedPin, HIGH );
    delay(300);
    digitalWrite( LedPin, LOW );
    delay(300);
  }
}


//_______________________________________________________________________________________________ MAIN LOOP

void loop() {
  MIDI.read();
  if (millis() - ccSendTime > CC_INTERVAL) {
   pressureSensor=analogRead(A3);
   if (pressureSensor >= ON_Thr) {
     breath();
     ccSendTime = millis();
     itsOn=1;
    } else if (itsOn) {
     breath();
     itsOn=0;
    }
  }  
}

//_______________________________________________________________________________________________ FUNCTIONS

void breath(){
  int breathCC;
  breathCC = map(constrain(pressureSensor,ON_Thr,breath_max),ON_Thr,breath_max,0,127);
  digitalWrite(LedPin, HIGH);
  MIDI.sendControlChange(CC_Number, breathCC, channel);
  digitalWrite(LedPin, LOW);
}
//***********************************************************

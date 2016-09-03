#include <MIDI.h>
// this is the FortySevenEffects Arduino MIDI Library, has to be downloaded and included

MIDI_CREATE_DEFAULT_INSTANCE();

/*
NAME:                 MIB Volume Injector
WRITTEN BY:           JOHAN BERGLUND
DATE:                 2016-08-29
FILE SAVED AS:        Volume-Injector.ino
FOR:                  Arduino Pro Mini, ATmega328
CLOCK:                16.00 MHz CRYSTAL                                        
PROGRAMME FUNCTION:   Input MIDI, add volume data, output merged data to MIDI
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
* Potentiometer for midi midi volume is connected to
* analog input A0.
* 
* Instead of hardware listed above, a regular Arduino Uno with the SparkFun MIDI shield can be used.
* 
*/



//_______________________________________________________________________________________________ DECLARATIONS


#define CC_Interval 10  // Interval for sending CC data
#define CC_Number 7     // Controller number (7=Volume, 11=Expression)

unsigned long ccSendTime = 0L;     // The last time we sent CC values

int volumeCC;
int oldVol=999;

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
  MIDI.read(); // read MIDI and forward messages 
  if (millis() - ccSendTime > CC_Interval) {  // is it time for sending CC data?
   volumeCC = map(analogRead(A0), 0, 1023, 0, 127); // set volume CC value with potentiometer on A1
   if (volumeCC != oldVol){ // only send CC if value has changed
    digitalWrite(LedPin, HIGH);
    MIDI.sendControlChange(CC_Number, volumeCC, channel); // send MIDI CC
    digitalWrite(LedPin, LOW);
    oldVol = volumeCC;
   }
   ccSendTime = millis();                   // reset cc timer
  }
}


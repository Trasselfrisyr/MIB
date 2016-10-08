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
* Other 5V sensors can be used if thr/max values are altered or potentiometers installed.
* Examples of other sensors suitable for use: Freescale MPX5010GP, MPXV4006GP
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
* Optional potentiometers for adjusting values for threshold and max breath are connected to
* analog inputs A0 and A1.
*     
* For a proper schematic of this, see github repository at https://github.com/Trasselfrisyr/MIB/Breath-Injector/
*/



//_______________________________________________________________________________________________ DECLARATIONS


#define CC_Interval 10  // Interval for sending CC data
#define CC_Number 2     // Controller number (2=Breath, 1=Mod Wheel, 7=Volume, 11=Expression, 74=Cutoff)
#define AT_Enable 1     // Send breath as channel aftertouch (0=Off, 1=On, 2=Exclusively)
#define POT_Avail 1     // Potentiometers installed at A0 and A1?

unsigned long ccSendTime = 0L;     // The last time we sent CC values

int pressureSensor=0;  // pressure data from breath sensor, for midi breath cc and breath threshold checks
byte itsOn=0;        // keep track and make sure we send CC with 0 value when off threshold
int ON_Thr=350;      // Set threshold level before switching ON (value to use if no pots installed, suits MPXV5004GP)
int breath_max=1023; // Upper limit for pressure (value to use if no pots installed, suits MPXV5004GP)

byte x;
byte LedPin=18;    // select the pin for the LED
byte channel=1;    // MIDI channel
float smooth=0.03; // smoothing value

//_______________________________________________________________________________________________ SETUP

void setup() {
  pinMode(LedPin,OUTPUT);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW); // set pin 2 low to enable DIN MIDI on MIB mkII board

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
  if (POT_Avail) {
    ON_Thr=map(analogRead(A0),0,1023,0,850);  // set threshold value with potentiometer on A0
    breath_max=map(analogRead(A1),0,1023,150,1023); // set max value with potentiometer on A1
    if ((ON_Thr+127) > breath_max){            // make sure max value always has a minimum offset from thr
      breath_max=ON_Thr+127;
    }
  }
  pressureSensor=pressureSensor*(1-smooth)+analogRead(A3)*smooth;   // get pressure sensor data, do some smoothing
  MIDI.read();
  if (millis() - ccSendTime > CC_Interval) {  // is it time for sending breath data?
   if (pressureSensor >= ON_Thr) {            // if we are over the threshold, send breath
     breath();    
     itsOn=1;
    } else if (itsOn) {                       // we have just gone below threshold, so send zero value
     breathOff();
     itsOn=0;
    }
    ccSendTime = millis();                   // reset cc timer
  }  
}

//_______________________________________________________________________________________________ FUNCTIONS

void breath(){
  int breathCC;
  breathCC = map(constrain(pressureSensor,ON_Thr,breath_max),ON_Thr,breath_max,1,127);
  digitalWrite(LedPin, HIGH);
  if (AT_Enable != 2) {
    MIDI.sendControlChange(CC_Number, breathCC, channel);
  }
  if (AT_Enable) {
    MIDI.sendAfterTouch(breathCC, channel);
  }
  digitalWrite(LedPin, LOW);
}

//***********************************************************

void breathOff(){
  digitalWrite(LedPin, HIGH);
  if (AT_Enable != 2) {
    MIDI.sendControlChange(CC_Number, 0, channel);
  }
  if (AT_Enable) {
    MIDI.sendAfterTouch(0, channel);
  }
  digitalWrite(LedPin, LOW);
}

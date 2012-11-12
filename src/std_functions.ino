#include <avr/pgmspace.h>

//-------------------
//   Global Vars 
//-------------------

// IO pin constants
const int firstLED = 2;
const int lastLED = 15;
const int onOffSwitch = 18;


// values returned by GetUserMode()()
const byte USER_MODE_OFF = 0;
const byte USER_MODE_SIMPLE = 1;
const byte USER_MODE_MELLOW = 2;


// global storage for PWM values
int ledPwm[lastLED+1];
byte ledIndex[lastLED+1];


//throb table - sinusoidal table normalize for eye linearity with a log function
const int throbTable[] PROGMEM = {
  0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 
  4, 4, 5, 6, 7, 8, 10, 12, 14, 17, 21, 26, 31, 38, 47, 57, 
  70, 85, 104, 126, 153, 185, 222, 266, 316, 375, 441, 516, 599, 691, 791, 899, 
  1012, 1130, 1250, 1370, 1487, 1599, 1702, 1793, 1870, 1931, 1972, 1993, 1993, 1972, 1931, 1870, 
  1793, 1702, 1599, 1487, 1370, 1250, 1130, 1012, 899, 791, 691, 599, 516, 441, 375, 316, 
  266, 222, 185, 153, 126, 104, 85, 70, 57, 47, 38, 31, 26, 21, 17, 14, 
  12, 10, 8, 7, 6, 5, 4, 4, 3, 3, 2, 2, 2, 2, 2, 1, 
  1, 1, 1, 1, 0, 0, 0, 0};

const int throbTableLength = sizeof(throbTable) / sizeof(int);







//------------------------------------------------------------------
//                     General purpose functions (written by Stan)
//------------------------------------------------------------------

//
// turn off all LEDs
//
void turnOffAllLEDs()
{ 
  int pinNum;

  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
  { 
    digitalWrite(pinNum, LOW);  
    ledPwm[pinNum] = 0;
    ledIndex[pinNum] = 0;
  }
}


//
// fade all LED down until they are all off ledIndex table
//    Enter:  Current LED index values must be set in the 
//            LEDOnTime = aproximate number of milliseconds LED is on for 
//              each PWM cycle, minimum value is 10 (or 10ms)
//            tableStep = 1 to fade up using every value, 2 to fade up faster using every other value
//
void fadeLEDsToBlack(int LEDOnTime, byte tableStep)
{ 
  bool stillFadingFlag;
  byte pinNum;
  int i;
  byte halfTableLength;

  //
  // run through the list of indexes, make sure they are all on the rising side of the sine wave
  //
  halfTableLength = throbTableLength / 2;

  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
  { 
    i = ledIndex[pinNum];
    if (i > halfTableLength)
      ledIndex[pinNum] = halfTableLength - (i - halfTableLength);
  }

  //
  // starting with the LED's current throb table value, march backwards in the table 
  // dimming the LEDs until they are all off
  //
  do 
  { 
    stillFadingFlag = false;

    for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    { 
      i = ledIndex[pinNum];
      if (i > 0)
      { 
        i -= tableStep;
        if (i < 0)
          i = 0;
        ledIndex[pinNum] = i;
        stillFadingFlag = true;
      }

      ledPwm[pinNum] = pgm_read_word(&throbTable[i]);
    }

    pwmAllPins(LEDOnTime);

  } 
  while(stillFadingFlag);
}



//
// fade all LED up to their values in the ledIndex table
//    Enter:  LEDOnTime = aproximate number of milliseconds LED is on for 
//              each PWM cycle, minimum value is 10 (or 10ms)
//
void fadeUpLEDs(int LEDOnTime)
{ 
  bool stillFadingFlag;
  byte pinNum;
  byte fadeUpIndex;
  byte halfTableLength;
  byte i;

  //
  // run through the list of indexes, make sure they are all on the rising side of the sine wave
  //
  halfTableLength = throbTableLength / 2;

  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
  { 
    i = ledIndex[pinNum];
    if (i > halfTableLength)
      ledIndex[pinNum] = halfTableLength - (i - halfTableLength);
  }

  //
  // start with all the LEDs off
  //
  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    ledPwm[pinNum] = 0;

  //
  // starting with the LED's off, march forward in the throb table, brightening them, until
  // they have reached their final values as set in the ledIndex array
  //
  fadeUpIndex = 1;

  while(true)
  { 
    stillFadingFlag = false;

    for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    { 
      if (ledIndex[pinNum] >= fadeUpIndex)
      { 
        ledPwm[pinNum] = pgm_read_word(&throbTable[fadeUpIndex]);
        stillFadingFlag = true;
      }
    }

    if (!stillFadingFlag)
      break;

    pwmAllPins(LEDOnTime);
    fadeUpIndex++;  
  } 
}



//
// fade up one LED, other LED values are not changed in the "ledPwm" table
//    Enter:  pinNum = LED to fade up
//            tableStep = 1 to fade up using every value, 2 to fade up faster using every other value
//            LEDOnTime = aproximate number of milliseconds LED is on for each PWM cycle, minimum value is 10 (or 10ms)
//
void fadeUpOneLED(byte pinNum, byte tableStep, int LEDOnTime)
{ 
  byte halfTableLength;
  byte tableIndex;
  int pwm;

  halfTableLength = throbTableLength / 2;

  //
  // fade up one LED, leave other values unchanged
  //
  for(tableIndex = 0; tableIndex < halfTableLength; tableIndex += tableStep)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[pinNum] = pwm;
    ledIndex[pinNum] = tableIndex;

    //
    // now turn on the LEDs for a short period with their PWM value
    //
    pwmAllPins(LEDOnTime);

    //
    // quit if the user mode has changed
    //
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }
}



//
// fade up and down one LED, other LED values are not changed in the "ledPwm" table
//    Enter:  pinNum = LED to fade up
//            tableStep = 1 to fade up using every value, 2 to fade up faster using every other value
//            LEDOnTime = aproximate number of milliseconds LED is on for each PWM cycle, minimum value is 10 (or 10ms)
//
void FadeUpAndDownOneLED(byte pinNum, byte tableStep, int LEDOnTime)
{ 
  byte halfTableLength;
  byte tableIndex;
  int pwm;

  //
  // fade up one LED, leave other values unchanged
  //
  for(tableIndex = 0; tableIndex < throbTableLength; tableIndex += tableStep)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[pinNum] = pwm;
    ledIndex[pinNum] = tableIndex;

    //
    // now turn on the LEDs for a short period with their PWM value
    //
    pwmAllPins(LEDOnTime);

    //
    // quit if the user mode has changed
    //
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }
}



//
// PWM all pins with a value between 0 and 2000, for aprox n milliseconds
//    Enter:  ledPwm[] contains the array of PWM values, corresponding to each LED
//            LEDOnTime = aproximate number of milliseconds LED is on for 
//              each PWM cycle, minimum value is 10 (or 10ms)
//
void pwmAllPins(int LEDOnTime)
{ 
  int i;
  int j;
  int onTime;

  //
  // repeat PWM cycle for the desired number of milliseconds
  //
  onTime = LEDOnTime / 10;
  if (onTime < 1) 
    onTime = 1;

  for (j = 0; j < onTime; j++)
  { //
    // turn on all LEDS that have a PWM value greater than 0
    //
    if (ledPwm[2]) bitSet(PORTD, 2); 
    else bitClear(PORTD, 2);
    if (ledPwm[3]) bitSet(PORTD, 3); 
    else bitClear(PORTD, 3);
    if (ledPwm[4]) bitSet(PORTD, 4); 
    else bitClear(PORTD, 4); 
    if (ledPwm[5]) bitSet(PORTD, 5); 
    else bitClear(PORTD, 5); 
    if (ledPwm[6]) bitSet(PORTD, 6); 
    else bitClear(PORTD, 6); 
    if (ledPwm[7]) bitSet(PORTD, 7); 
    else bitClear(PORTD, 7); 
    if (ledPwm[8]) bitSet(PORTB, 0); 
    else bitClear(PORTB, 0); 
    if (ledPwm[9]) bitSet(PORTB, 1); 
    else bitClear(PORTB, 1); 
    if (ledPwm[10]) bitSet(PORTB, 2); 
    else bitClear(PORTB, 2); 
    if (ledPwm[11]) bitSet(PORTB, 3); 
    else bitClear(PORTB, 3); 
    if (ledPwm[12]) bitSet(PORTB, 4); 
    else bitClear(PORTB, 4); 
    if (ledPwm[13]) bitSet(PORTB, 5); 
    else bitClear(PORTB, 5); 
    if (ledPwm[14]) bitSet(PORTC, 0); 
    else bitClear(PORTC, 0);
    if (ledPwm[15]) bitSet(PORTC, 1); 
    else bitClear(PORTC, 1);

    //
    // loop for one PWM cycle, turning off each LED at the right PWM value
    //
    for (i = 1; i <= 2000; i++)
    { 
      if (i == ledPwm[2]) bitClear(PORTD, 2);
      if (i == ledPwm[3]) bitClear(PORTD, 3);
      if (i == ledPwm[4]) bitClear(PORTD, 4);
      if (i == ledPwm[5]) bitClear(PORTD, 5);
      if (i == ledPwm[6]) bitClear(PORTD, 6);
      if (i == ledPwm[7]) bitClear(PORTD, 7);
      if (i == ledPwm[8]) bitClear(PORTB, 0);
      if (i == ledPwm[9]) bitClear(PORTB, 1);
      if (i == ledPwm[10]) bitClear(PORTB, 2);
      if (i == ledPwm[11]) bitClear(PORTB, 3);
      if (i == ledPwm[12]) bitClear(PORTB, 4);
      if (i == ledPwm[13]) bitClear(PORTB, 5);
      if (i == ledPwm[14]) bitClear(PORTC, 0);
      if (i == ledPwm[15]) bitClear(PORTC, 1);
    }
  }
}



//
// return the state of the user mode
//    Exit:  USER_MODE_OFF returned if on/off switch in the off position
//           USER_MODE_SIMPLE returned if in the "simple display" mode
//           USER_MODE_MELLOW returned if in the "mellow display" mode
//
byte GetUserMode()
{ 
  int analogValue;

  //
  // check if the on/off switch is off
  // 
  if (digitalRead(onOffSwitch) == HIGH)
    return(USER_MODE_OFF);

  //
  // switch is on, read the pot position
  //
  analogValue = analogRead(5);

  // NOTE: THIS REALLY SHOULD HAVE HYSTERESIS! 

  if (analogValue < 512)
    return(USER_MODE_SIMPLE);

  return(USER_MODE_MELLOW);
}




//-------------------------------------
//      Custom Library Functions
//-------------------------------------

void exitIfNotMode(byte userMode) {
  if (GetUserMode() != userMode) return;
}

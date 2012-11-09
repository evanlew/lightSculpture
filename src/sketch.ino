
//      ******************************************************************
//      *                                                                *fadeUpOneLED
//      *               DPEA LED Machine - Diana's footbal               *
//      *                                                                *
//      *             Stan Reifel                    9/18/12             *
//      *                                                                *
//      ******************************************************************

#include <avr/pgmspace.h>

//
// IO pin constants
//
const int firstLED = 2;
const int lastLED = 15;
const int onOffSwitch = 18;


//
// values returned by GetUserMode()()
//
const byte USER_MODE_OFF = 0;
const byte USER_MODE_SIMPLE = 1;
const byte USER_MODE_MELLOW = 2;


//
// global storage for PWM values
//
int ledPwm[lastLED+1];
byte ledIndex[lastLED+1];


//
//throb table - sinusoidal table normalize for eye linearity with a log function
//
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


//
// top level setup function
//
void setup()
{  
  int pinNum;



  //
  // configure the LED pins as outputs
  //
  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    pinMode(pinNum, OUTPUT);  


  //
  // configure the on/off switch as an input
  //
  pinMode(onOffSwitch, INPUT_PULLUP);


  //
  // setup the serial port for print statement debugging
  //
  Serial.begin(9600);
  Serial.println("HERE");
}



//
// main loop, select the display show based on the User Mode Setting
//
void loop()
{ 
  byte userMode; 
//    digitalWrite(2, HIGH);
//    delay(10);
//    digitalWrite(2, LOW);
//    delay(10);
 
 



  //
  // check if the user has selected the Simple mode
  //
//    if (GetUserMode() == USER_MODE_SIMPLE)
//    {
//      simpleLEDDisplays();
//      Serial.println("simple");
//    }


  //
  // check if the user has selected the Mellow mode
  //
  //  if (GetUserMode() == USER_MODE_MELLOW)
  //  {
      mellowLEDDisplays();
      Serial.println("mellow");
  //  }

}

//------------------------------------------------------------------
//                        Simple LED Displays
//------------------------------------------------------------------


//
// loop through the mellow LED displays
//
void simpleLEDDisplays()
{ 
  const int delayBetweenShows = 800;

  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;

  showColors();
  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;

  rotateColors(800, 2);
  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows + 400))
    return;

  sequenceLEDsClockwise(150, 3);
  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;

  sequenceLEDsCounterClockwise(90, 3);
  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;

  sequenceLEDsAllOnThenAllOff(90, 2);
  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;

  rotateLEDsSpeedingUp(160, 30);
  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;

  rotateBackAndForthOnSides(80, 4);
  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;

  randomBursts(25, 100, 90);
  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;

  sequenceTwoLEDsClockwise(100, 3);
  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;

  flashAndHold();
  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;

  if (delayBetweenDisplaysAndCheckIfUserModeNotSimple(delayBetweenShows))
    return;
}



//
// display each of the colors
//
void showColors(void)
{ 
  int pinNum;

  //
  // dipslay the yellows
  //
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);
  delay(1000);
  if(GetUserMode() != USER_MODE_SIMPLE) 
    return;

  //
  // now the oranges
  //
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  delay(1000);
  if(GetUserMode() != USER_MODE_SIMPLE) 
    return;

  //
  // now the reds
  //
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  delay(2000);
  if(GetUserMode() != USER_MODE_SIMPLE) 
    return;

  //
  // turn them off, two at a time starting from the top
  //
  for(pinNum = 8; pinNum >= firstLED; pinNum--)
  { 
    digitalWrite(pinNum, LOW);
    digitalWrite(9 + (8 - pinNum), LOW);
    delay(250);

    if(GetUserMode() != USER_MODE_SIMPLE) 
      return;
  }
}



//
// rotate each of the colors
//
void rotateColors(int LEDOnTime, int repeat)
{  
  int repeatCount;

  //
  // repeat the sequence n times
  //
  for (repeatCount = 0; repeatCount < repeat; repeatCount++)
  { //
    // dipslay the yellows
    //
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(15, HIGH);
    delay(LEDOnTime);

    if(GetUserMode() != USER_MODE_SIMPLE)     // quit if the user mode has changed
      return;

    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);

    //
    // now the oranges
    //
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    delay(LEDOnTime);

    if(GetUserMode() != USER_MODE_SIMPLE)      // quit if the user mode has changed
      return;

    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);

    //
    // now the reds
    //
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    delay(LEDOnTime);

    if(GetUserMode() != USER_MODE_SIMPLE)      // quit if the user mode has changed
      return;

    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);


    //
    // now the oranges
    //
    digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    delay(LEDOnTime);

    if(GetUserMode() != USER_MODE_SIMPLE)      // quit if the user mode has changed
      return;

    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
}



//
// sequence the LEDs one at a time in a counter clockwise direction
//    Enter:  LEDOnTime = number of milliseconds LED is on
//            repeat = number of times the sequence repeats
//
void sequenceLEDsCounterClockwise(int LEDOnTime, int repeat)
{ 
  int repeatCount;
  int pinNum;

  //
  // repeat the sequence n times
  //
  for (repeatCount = 0; repeatCount < repeat; repeatCount++)
  { //
    // sequence the LEDs from one end to the other once
    //
    for(pinNum = lastLED; pinNum >= firstLED; pinNum--)
    { 
      digitalWrite(pinNum, HIGH);  
      delay(LEDOnTime);
      digitalWrite(pinNum, LOW);  

      //
      // quit if the user mode has changed
      //
      if(GetUserMode() != USER_MODE_SIMPLE) 
        return;
    }
  }
}



//
// sequence the LEDs one at a time in a clockwise direction
//    Enter:  LEDOnTime = number of milliseconds LED is on
//            repeat = number of times the sequence repeats
//
void sequenceLEDsClockwise(int LEDOnTime, int repeat)
{ 
  int repeatCount;
  int pinNum;

  //
  // repeat the sequence n times
  //
  for (repeatCount = 0; repeatCount < repeat; repeatCount++)
  { //
    // sequence the LEDs from one end to the other once
    //
    for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    { 
      digitalWrite(pinNum, HIGH);  
      delay(LEDOnTime);
      digitalWrite(pinNum, LOW);  

      //
      // quit if the user mode has changed
      //
      if(GetUserMode() != USER_MODE_SIMPLE) 
        return;
    }
  }
}



//
// sequence the LEDs one at a time in a clockwise direction
//    Enter:  LEDOnTime = number of milliseconds LED is on
//            repeat = number of times the sequence repeats
//
void sequenceTwoLEDsClockwise(int LEDOnTime, int repeat)
{ 
  int repeatCount;
  byte pinNum;
  byte firstPin;
  byte secondPin;

  //
  // repeat the sequence n times
  //
  for (repeatCount = 0; repeatCount < repeat; repeatCount++)
  { //
    // sequence the LEDs from one end to the other once
    //
    for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    { 
      firstPin = pinNum + 3;
      if(firstPin > lastLED)
        firstPin -= 14;

      secondPin = firstPin + 7;
      if(secondPin > lastLED)
        secondPin -= 14;

      digitalWrite(firstPin, HIGH);  
      digitalWrite(secondPin, HIGH);  
      delay(LEDOnTime);
      digitalWrite(firstPin, LOW);  
      digitalWrite(secondPin, LOW);  

      //
      // quit if the user mode has changed
      //
      if(GetUserMode() != USER_MODE_SIMPLE) 
        return;
    }
  }
}


//
// sequence the LEDs all on, then all off
//    Enter:  LEDOnTime = number of milliseconds LED is on
//            repeat = number of times the sequence repeats
//
void sequenceLEDsAllOnThenAllOff(int LEDOnTime, int repeat)
{ 
  int repeatCount;
  int pinNum;

  //
  // repeat the sequence n times
  //
  for (repeatCount = 0; repeatCount < repeat; repeatCount++)
  { //
    // turn on the row of LEDs
    //
    for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    { 
      digitalWrite(pinNum, HIGH);  
      delay(LEDOnTime);

      if(GetUserMode() != USER_MODE_SIMPLE) 
        return;
    }

    //
    // turn off the row of LEDs
    //
    for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    { 
      digitalWrite(pinNum, LOW);    
      delay(LEDOnTime);

      if(GetUserMode() != USER_MODE_SIMPLE) 
        return;
    }
  }
}



//
// rotate the LEDs CCW, increasing the speed as it goes
//        Enter:  slowOnTime = milliseconds between advancing, in the beginning
//                fastOnTime = milliseconds between advancing, at the end
//
void rotateLEDsSpeedingUp(int slowOnTime, int fastOnTime)
{ 
  int pinNum;
  int stepOnTime;
  int currentOnTime;
  int repeatCount;

  currentOnTime = slowOnTime;

  //
  // repeat until going really fast
  //
  while(currentOnTime >= fastOnTime+1)
  { //
    // sequence the LEDs clockwise once
    //
    for(pinNum = lastLED; pinNum >= firstLED; pinNum--)
    { 
      digitalWrite(pinNum, HIGH);  
      delay(currentOnTime);
      digitalWrite(pinNum, LOW);  

      if(GetUserMode() != USER_MODE_SIMPLE)      // quit if the user mode has changed
        return;
    }

    //
    // compute a faster LED on time
    //
    stepOnTime = (currentOnTime * 15) / 100;
    if (stepOnTime < 1) 
      stepOnTime = 1;

    currentOnTime -= stepOnTime;
  }

  //
  // continue going at top speed
  //
  for (repeatCount = 0; repeatCount < 10; repeatCount++)
  { 
    for(pinNum = lastLED; pinNum >= firstLED; pinNum--)
    { 
      digitalWrite(pinNum, HIGH);  
      delay(fastOnTime-1);
      digitalWrite(pinNum, LOW);  

      if(GetUserMode() != USER_MODE_SIMPLE)      // quit if the user mode has changed
        return;
    }
  }
}



//
// sequence back and forth on the sides
//    Enter:  LEDOnTime = number of milliseconds LED is on
//            repeat = number of times the sequence repeats
//
void rotateBackAndForthOnSides(int LEDOnTime, int repeat)
{ 
  int repeatCount;
  int pinNum;
  int otherPinNum;

  //
  // repeat the sequence n times
  //
  for (repeatCount = 0; repeatCount < repeat; repeatCount++)
  { 
    pinNum = firstLED;
    while(true)
    { 
      otherPinNum = 9 + (8 - pinNum);

      digitalWrite(pinNum, HIGH);  
      digitalWrite(otherPinNum, HIGH);  

      delay(LEDOnTime);

      digitalWrite(pinNum, LOW);  
      digitalWrite(otherPinNum, LOW);  

      pinNum++;
      if (pinNum == 9)
        break;

      if(GetUserMode() != USER_MODE_SIMPLE)      // quit if the user mode has changed
        return;
    }

    pinNum = 7;
    while(true)
    { 
      otherPinNum = 9 + (8 - pinNum);

      digitalWrite(pinNum, HIGH);  
      digitalWrite(otherPinNum, HIGH);  

      delay(LEDOnTime);

      digitalWrite(pinNum, LOW);  
      digitalWrite(otherPinNum, LOW);  

      pinNum--;
      if (pinNum == firstLED)
        break;

      if(GetUserMode() != USER_MODE_SIMPLE)      // quit if the user mode has changed
        return;    
    }
  }
}



//
// random bursts
//    Enter:  LEDOnTime = number of milliseconds LED is on
//            repeat = number of times the sequence repeats
//
void randomBursts(int LEDOnTime, int LEDOffTime, int repeat)
{ 
  int repeatCount;
  int pinNum;

  //
  // repeat the sequence n times
  //
  for (repeatCount = 0; repeatCount < repeat; repeatCount++)
  { //
    // choose a pin number at random
    //
    pinNum = (int) random(firstLED, lastLED);

    //
    // blink that pin
    //
    digitalWrite(pinNum, HIGH);  
    delay(LEDOnTime);
    digitalWrite(pinNum, LOW);  
    delay(LEDOffTime);

    if(GetUserMode() != USER_MODE_SIMPLE)      // quit if the user mode has changed
      return;
  }
}



//
// flash an LED, then go to the next one, building up as you go
//
void flashAndHold()
{ 
  int pinNum;
  int holdPinNum;
  byte flashCount;
  const int LEDOnTime = 30;

  holdPinNum = lastLED;
  for(holdPinNum = lastLED; holdPinNum >= firstLED; holdPinNum--)
  { 
    for(pinNum = firstLED; pinNum <= holdPinNum-1; pinNum++)
    { //
      // flash the LED once
      //
      digitalWrite(pinNum, HIGH);  
      delay(LEDOnTime);
      digitalWrite(pinNum, LOW);  
      delay(LEDOnTime);

      delay(80);

      if(GetUserMode() != USER_MODE_SIMPLE)      // quit if the user mode has changed
        return;
    }

    //
    // hold the last LED on
    //
    digitalWrite(pinNum, HIGH);
  }

  delay(1200);

  //
  // sequence CCW turning the LEDs off
  //
  for(pinNum = lastLED; pinNum >= firstLED; pinNum--)
  { 
    digitalWrite(pinNum, LOW);  
    delay(55);
  }
}



//
// delay for the given period, also check if the User Mode has changed
//    Enter:   delayBetweenShows = amount to delay in ms (with a resolution of 10ms)
//    Exit:    true returned if User Mode has changed
//
bool delayBetweenDisplaysAndCheckIfUserModeNotSimple(int delayBetweenShows)
{ 
  int delay10ms;

  //
  // if User Mode has changed, turn off the LEDs and return
  //
  if(GetUserMode() != USER_MODE_SIMPLE)
  { 
    turnOffAllLEDs();
    return(true);
  }


  //
  // delay for the desired time while checking if the User Mode has changed
  //
  delay10ms = delayBetweenShows / 10;

  while(delay10ms > 0)
  {  
    delay(10);

    if(GetUserMode() != USER_MODE_SIMPLE)
    { 
      turnOffAllLEDs();
      return(true);
    }

    delay10ms--;
  }

  return(false);
}

//------------------------------------------------------------------
//                        Mellow LED Displays
//------------------------------------------------------------------

//
// loop through the mellow LED displays
//
void mellowLEDDisplays()
{ 
  const int delayBetweenShows = 800;

  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
    return;
  showColorsMellowOne(10);
//  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
//    return;
//
//  showColorsMellowTwo(10);
//  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
//    return;
//
//  fadeUpSlowAndDownFast(60);
//  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
//    return;
//
//  alternateThrobEveryOther(25, 3);
//  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
//    return;
//
//  climbUp(24);
//  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
//    return;
//
//  sequenceClockwiseWithFading(firstLED, lastLED, 2, 10, 1);
//  if(GetUserMode() != USER_MODE_MELLOW) return;
//  sequenceClockwiseWithFading(firstLED, lastLED, 4, 10, 1);
//  if(GetUserMode() != USER_MODE_MELLOW) return;
//  sequenceClockwiseWithFading(firstLED, lastLED, 6, 10, 1);
//  if(GetUserMode() != USER_MODE_MELLOW) return;
//  sequenceClockwiseWithFading(firstLED, lastLED, 8, 10, 1);
//  if(GetUserMode() != USER_MODE_MELLOW) return;
//  sequenceClockwiseWithFading(firstLED, lastLED, 10, 10, 1);
//  if(GetUserMode() != USER_MODE_MELLOW) return;
//  sequenceClockwiseWithFading(firstLED, lastLED, 12, 10, 1);
//  if(GetUserMode() != USER_MODE_MELLOW) return;
//  sequenceClockwiseWithFading(firstLED, lastLED, 13, 10, 5);
//  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
//    return;    
//
//  theWave(firstLED, lastLED, 16, 10, 6);
//  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
//    return;    
//
//  theWave(firstLED, lastLED, 35, 10, 6);
//  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
//    return;    
//
//  sequenceClockwiseWithFading(firstLED, lastLED, 8, 40, 2);
//  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
//    return;    
//
//  if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
//    return;
}



//
// display each of the colors
//
void showColorsMellowOne(int LEDOnTime)
{ 
  int pinNum;
  int count;
  byte halfTableLength;
  byte tableIndex;
  int pwm;

  //
  // start with all the LEDs off
  //
  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    ledPwm[pinNum] = 0;

  halfTableLength = throbTableLength / 2;

  //
  // dipslay the yellows, ramping them up in brightness
  //
  for(tableIndex = 0; tableIndex < halfTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[14] = pwm;
    ledIndex[14] = tableIndex;

    ledPwm[15] = pwm;
    ledIndex[15] = tableIndex;

    ledPwm[2] = pwm;
    ledIndex[2] = tableIndex;

    ledPwm[3] = pwm;
    ledIndex[3] = tableIndex;

    pwmAllPins(LEDOnTime);
  } 

  //
  // delay while maintaining the PWM and checking if the User Mode has changed
  //
  for(count = 0; count < 150; count++)
  { 
    pwmAllPins(LEDOnTime);
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }


  //
  // dipslay the oranges, ramping them up in brightness
  //
  for(tableIndex = 0; tableIndex < halfTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[4] = pwm;
    ledIndex[4] = tableIndex;

    ledPwm[5] = pwm;
    ledIndex[5] = tableIndex;

    ledPwm[6] = pwm;
    ledIndex[6] = tableIndex;

    ledPwm[11] = pwm;
    ledIndex[11] = tableIndex;

    ledPwm[12] = pwm;
    ledIndex[12] = tableIndex;

    ledPwm[13] = pwm;
    ledIndex[13] = tableIndex;

    pwmAllPins(LEDOnTime);
  } 

  //
  // delay while maintaining the PWM and checking if the User Mode has changed
  //
  for(count = 0; count < 150; count++)
  { 
    pwmAllPins(LEDOnTime);
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }


  //
  // dipslay the reds, ramping them up in brightness
  //
  for(tableIndex = 0; tableIndex < halfTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[7] = pwm;
    ledIndex[7] = tableIndex;

    ledPwm[8] = pwm;
    ledIndex[8] = tableIndex;

    ledPwm[9] = pwm;
    ledIndex[9] = tableIndex;

    ledPwm[10] = pwm;
    ledIndex[10] = tableIndex;

    pwmAllPins(LEDOnTime);
  } 

  //
  // delay while maintaining the PWM and checking if the User Mode has changed
  //
  for(count = 0; count < 250; count++)
  { 
    pwmAllPins(LEDOnTime);
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }


  //
  // turn them all off
  //
  fadeLEDsToBlack(10, 1);
}



//
// display each of the colors
//
void showColorsMellowTwo(int LEDOnTime)
{ 
  int pinNum;
  int count;
  const byte fadeUpSpeed = 15;

  //
  // start with all the LEDs off
  //
  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    ledPwm[pinNum] = 0;

  //
  // dipslay the yellows, fading them up one at a time
  //
  fadeUpOneLED(14, fadeUpSpeed, LEDOnTime);
  fadeUpOneLED(15, fadeUpSpeed, LEDOnTime);
  fadeUpOneLED(2, fadeUpSpeed, LEDOnTime);
  fadeUpOneLED(3, fadeUpSpeed, LEDOnTime);

  //
  // delay while maintaining the PWM and checking if the User Mode has changed
  //
  for(count = 0; count < 150; count++)
  { 
    pwmAllPins(LEDOnTime);
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }

  //
  // now the oranges, fading them up one at a time
  //
  fadeUpOneLED(4, fadeUpSpeed, LEDOnTime);
  fadeUpOneLED(5, fadeUpSpeed, LEDOnTime);
  fadeUpOneLED(6, fadeUpSpeed, LEDOnTime);

  //
  // delay while maintaining the PWM and checking if the User Mode has changed
  //
  for(count = 0; count < 150; count++)
  { 
    pwmAllPins(LEDOnTime);
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }

  //
  // now the reds
  //
  fadeUpOneLED(7, fadeUpSpeed, LEDOnTime);
  fadeUpOneLED(8, fadeUpSpeed, LEDOnTime);
  fadeUpOneLED(9, fadeUpSpeed, LEDOnTime);
  fadeUpOneLED(10, fadeUpSpeed, LEDOnTime);

  //
  // delay while maintaining the PWM and checking if the User Mode has changed
  //
  for(count = 0; count < 150; count++)
  { 
    pwmAllPins(LEDOnTime);
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }

  //
  // now the oranges
  //
  fadeUpOneLED(11, fadeUpSpeed, LEDOnTime);
  fadeUpOneLED(12, fadeUpSpeed, LEDOnTime);
  fadeUpOneLED(13, fadeUpSpeed, LEDOnTime);

  //
  // delay while maintaining the PWM and checking if the User Mode has changed
  //
  for(count = 0; count < 30; count++)
  { 
    pwmAllPins(100);
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }

  //
  // turn them all off
  //
  fadeLEDsToBlack(10, 1);
}



//
// fade all the LEDs up slowly, then down fast
//    Enter:  LEDOnTime = aproximate number of milliseconds LED is on for 
//              each PWM cycle, minimum value is 10 (or 10ms)
//            repeat = number of times the sequence repeats
//
void fadeUpSlowAndDownFast(int LEDOnTime)
{ 
  byte pinNum;
  int pwm;
  byte halfTableLength;
  byte tableIndex;
  int count;

  halfTableLength = throbTableLength / 2;

  //
  // get the PWM values from the throb table, increasing the brightness
  //
  for(tableIndex = 0; tableIndex < halfTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);

    //
    // set the PWM value for every LED to be the same
    //
    for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    { 
      ledPwm[pinNum] = pwm; 
      ledIndex[pinNum] = tableIndex; 
    }

    //
    // now turn on the LEDs for a short period with their PWM value
    //
    pwmAllPins(LEDOnTime);

    //
    // quit if the User Mode has changed
    //
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }

  //
  // delay while maintaining the PWM and checking if the User Mode has changed
  //
  for(count = 0; count < 30; count++)
  { 
    pwmAllPins(100);
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }

  //
  // turn them all off
  //
  fadeLEDsToBlack(10, 1);
}



//
// climb from the bottom LED on up
//    Enter:  LEDOnTime = aproximate number of milliseconds LED is on for 
//
void climbUp(int LEDOnTime)
{ 
  int pinNum;
  int count;
  int tableIndex;
  int pwm;
  byte halfTableLength;
  const int fadeOutStep = 4;


  halfTableLength = (throbTableLength / 2);

  //
  // start with all the LEDs off
  //
  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    ledPwm[pinNum] = 0;


  //
  // start at the bottom, work the way up
  //
  for(tableIndex = 0; tableIndex < throbTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[2] = pwm;
    ledPwm[15] = pwm;
    pwmAllPins(LEDOnTime);
  } 
  if(GetUserMode() != USER_MODE_MELLOW) 
    return;


  for(tableIndex = 0; tableIndex < throbTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[2] = pwm;
    ledPwm[15] = pwm;
    ledPwm[3] = pwm;
    ledPwm[14] = pwm;
    pwmAllPins(LEDOnTime);
  } 
  if(GetUserMode() != USER_MODE_MELLOW) 
    return;


  for(tableIndex = 0; tableIndex < throbTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[2] = pwm;
    ledPwm[15] = pwm;
    ledPwm[3] = pwm;
    ledPwm[14] = pwm;
    ledPwm[4] = pwm;
    ledPwm[13] = pwm;
    pwmAllPins(LEDOnTime);
  } 
  if(GetUserMode() != USER_MODE_MELLOW) 
    return;


  for(tableIndex = 0; tableIndex < throbTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[2] = pwm;
    ledPwm[15] = pwm;
    ledPwm[3] = pwm;
    ledPwm[14] = pwm;
    ledPwm[4] = pwm;
    ledPwm[13] = pwm;
    ledPwm[5] = pwm;
    ledPwm[12] = pwm;
    pwmAllPins(LEDOnTime);
  } 
  if(GetUserMode() != USER_MODE_MELLOW) 
    return;


  for(tableIndex = 0; tableIndex < throbTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[2] = pwm;
    ledPwm[15] = pwm;
    ledPwm[3] = pwm;
    ledPwm[14] = pwm;
    ledPwm[4] = pwm;
    ledPwm[13] = pwm;
    ledPwm[5] = pwm;
    ledPwm[12] = pwm;
    ledPwm[6] = pwm;
    ledPwm[11] = pwm;
    pwmAllPins(LEDOnTime);
  } 
  if(GetUserMode() != USER_MODE_MELLOW) 
    return;


  for(tableIndex = 0; tableIndex < throbTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[2] = pwm;
    ledPwm[15] = pwm;
    ledPwm[3] = pwm;
    ledPwm[14] = pwm;
    ledPwm[4] = pwm;
    ledPwm[13] = pwm;
    ledPwm[5] = pwm;
    ledPwm[12] = pwm;
    ledPwm[6] = pwm;
    ledPwm[11] = pwm;
    ledPwm[7] = pwm;
    ledPwm[10] = pwm;
    pwmAllPins(LEDOnTime);
  } 
  if(GetUserMode() != USER_MODE_MELLOW) 
    return;


  for(tableIndex = 0; tableIndex < halfTableLength; tableIndex++)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);

    for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    { 
      ledPwm[pinNum] = pwm;
      ledIndex[pinNum] = tableIndex;
    }

    pwmAllPins(LEDOnTime);
  } 
  if(GetUserMode() != USER_MODE_MELLOW) 
    return;


  //
  // delay at full brightness, while maintaining the PWM and checking if the User Mode has changed
  //
  for(count = 0; count < 10; count++)
  { 
    pwmAllPins(100);
    if(GetUserMode() != USER_MODE_MELLOW) 
      return;
  }


  //
  // turn them off, from the top down
  //
  for(tableIndex = halfTableLength; tableIndex >= 0; tableIndex-=fadeOutStep)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[8] = pwm;
    ledPwm[9] = pwm;
    pwmAllPins(10);
  } 

  for(tableIndex = halfTableLength; tableIndex >= 0; tableIndex-=fadeOutStep)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[7] = pwm;
    ledPwm[10] = pwm;
    pwmAllPins(10);
  } 

  for(tableIndex = halfTableLength; tableIndex >= 0; tableIndex-=fadeOutStep)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[6] = pwm;
    ledPwm[11] = pwm;
    pwmAllPins(10);
  } 

  for(tableIndex = halfTableLength; tableIndex >= 0; tableIndex-=fadeOutStep)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[5] = pwm;
    ledPwm[12] = pwm;
    pwmAllPins(10);
  } 

  for(tableIndex = halfTableLength; tableIndex >= 0; tableIndex-=fadeOutStep)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[4] = pwm;
    ledPwm[13] = pwm;
    pwmAllPins(10);
  } 

  for(tableIndex = halfTableLength; tableIndex >= 0; tableIndex-=fadeOutStep)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[3] = pwm;
    ledPwm[14] = pwm;
    pwmAllPins(10);
  } 

  for(tableIndex = halfTableLength; tableIndex >= 0; tableIndex-=fadeOutStep)
  { 
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[2] = pwm;
    ledPwm[15] = pwm;
    pwmAllPins(10);
  } 
}



//
// alternate throbbing every other LED
//    Enter:  LEDOnTime = aproximate number of milliseconds LED is on for 
//              each PWM cycle, minimum value is 10 (or 10ms)
//            repeat = number of times the sequence repeats
//
void alternateThrobEveryOther(int LEDOnTime, int repeat)
{ 
  int repeatCount;
  byte halfTableLength;
  byte throbTableIndex;
  int evenPWM;
  byte evenIndex;
  int oddPWM;
  byte oddIndex;
  byte pinNum;
  bool firstTimeFlag;

  halfTableLength = throbTableLength / 2;

  firstTimeFlag = true;

  //
  // loop once for each time the sequence is repeated
  //
  for (repeatCount = 0; repeatCount < repeat; repeatCount++)
  { 
    for(throbTableIndex=0; throbTableIndex<=throbTableLength; throbTableIndex++)
    { //
      // determine the PWM value for the even LEDs
      //
      evenIndex = throbTableIndex;
      evenPWM = pgm_read_word(&throbTable[evenIndex]);

      //
      // determine the PWM value for the odd LEDs, they are 90 degrees out of phase with the evens
      //
      oddIndex = evenIndex + halfTableLength;
      if (oddIndex >= throbTableLength)
        oddIndex -= throbTableLength;
      oddPWM = pgm_read_word(&throbTable[oddIndex]);

      //
      // set the PWM value for each LED
      //
      pinNum = firstLED;
      while(pinNum <= lastLED)
      { 
        ledPwm[pinNum] = evenPWM;
        ledIndex[pinNum] = evenIndex;
        pinNum++;

        ledPwm[pinNum] = oddPWM;
        ledIndex[pinNum] = oddIndex;
        pinNum++;       
      }

      //
      // now turn on the LEDs, if this is the first cycle, begin with a fade up
      //
      if (firstTimeFlag)
      { 
        fadeUpLEDs(LEDOnTime);
        firstTimeFlag = false;
      }
      else
        pwmAllPins(LEDOnTime);

      //
      // quit if the user mode has changed
      //
      if(GetUserMode() != USER_MODE_MELLOW) 
        return;
    }
  }

  //
  // all done, fade to black
  //
  fadeLEDsToBlack(LEDOnTime, 1);
}



//
// sequence the LEDs clockwise, fading them up and down as the go
//    Enter:  firstLEDNumber = first LED number in the sequence
//            lastLEDNumber = last LED number in the sequence
//            numberOfLEDsIlluminatedAtOnce = number of LEDs that are on at once (2, 3, 4, 5, 6, 8, 10, 12)
//            LEDOnTime = aproximate number of milliseconds LED is on for each PWM cycle, minimum value is 10 (or 10ms)
//            repeat = number of times the sequence repeats
//
void sequenceClockwiseWithFading(byte firstLEDNumber, byte lastLEDNumber, byte numberOfLEDsIlluminatedAtOnce, int LEDOnTime, byte repeat)
{ 
  byte repeatCount;
  byte lastRepeatCount;
  byte numberOfLEDsInSequence;
  int stepWidth;
  int lengthOfTable;
  int indexToStartOfThrobTable;
  int indexToEndOfThrobTable;
  int initialTableIndexAnchor;
  int finalTableIndexAnchor;
  int tableIndexAnchor;
  int tableIndex;
  byte pinNum;

  //
  // start with all the LEDs off, especially those outside firstLEDNumber through lastLEDNumber
  //
  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    ledPwm[pinNum] = 0;

  //
  // precompute some values to speed up the loop
  //
  numberOfLEDsInSequence = lastLEDNumber - firstLEDNumber + 1;
  stepWidth = throbTableLength / numberOfLEDsIlluminatedAtOnce ;
  lengthOfTable = numberOfLEDsInSequence * stepWidth;
  indexToStartOfThrobTable = lengthOfTable - throbTableLength;
  indexToEndOfThrobTable = indexToStartOfThrobTable + throbTableLength;
  lastRepeatCount = repeat - 1;

  //
  // begin before the table so LEDs fade in to start
  //
  initialTableIndexAnchor = -numberOfLEDsIlluminatedAtOnce * stepWidth;


  //
  // loop once for each time the sequence is repeated
  //
  for (repeatCount = 0; repeatCount <= lastRepeatCount; repeatCount++)
  { //
    // if this is the last repeat cycle, extend the length of the table so the LEDs will fade out at the end
    //
    if (repeatCount == lastRepeatCount)
      finalTableIndexAnchor = 2 *lengthOfTable;
    else
      finalTableIndexAnchor = lengthOfTable; 

    //
    // march from one end of the table to the other causing the LED to sequence from firstLEDNumber to lastLEDNumber
    //
    for(tableIndexAnchor = initialTableIndexAnchor; tableIndexAnchor < lengthOfTable; tableIndexAnchor++)
    { 
      tableIndex = tableIndexAnchor;

      //
      // set a PWM value for each LED, selecting the PWM value for so they are equally spaced across the table
      //
      for(pinNum = lastLEDNumber; pinNum >= firstLEDNumber; pinNum--)
      { //
        // our table includes a blanking region before and after the actual throb table
        //
        if ((tableIndex < indexToStartOfThrobTable) || (tableIndex >= indexToEndOfThrobTable))
          ledPwm[pinNum] = 0;
        else
          ledPwm[pinNum] = pgm_read_word(&throbTable[tableIndex - indexToStartOfThrobTable]);

        //
        // advance across the table by the step width, wrapping back after getting to the table's end
        //
        tableIndex += stepWidth;
        if (tableIndex >= finalTableIndexAnchor)
          tableIndex -= lengthOfTable;
      }

      //
      // now that all the PWM values have been set, display them by turning on the LEDs for a short period
      //
      pwmAllPins(LEDOnTime);

      //
      // quit if the user mode has changed
      //
      if(GetUserMode() != USER_MODE_MELLOW) 
        return;
    }

    //
    // since one cycle has been completed, reset the table starting point so next cycle will not fade in
    //
    initialTableIndexAnchor = 0;
  }
}



//
// sequence the LEDs counter clockwise, fading them up and down as the go
//    Enter:  firstLEDNumber = first LED number in the sequence
//            lastLEDNumber = last LED number in the sequence
//            numberOfLEDsIlluminatedAtOnce = number of LEDs that are on at once (2, 3, 4, 5, 6, 8, 10, 12)
//            LEDOnTime = aproximate number of milliseconds LED is on for each PWM cycle, minimum value is 10 (or 10ms)
//            repeat = number of times the sequence repeats
//
void SequenceCounterClockwiseWithFading(byte firstLEDNumber, byte lastLEDNumber, byte numberOfLEDsIlluminatedAtOnce, int LEDOnTime, byte repeat)
{ 
  byte repeatCount;
  byte lastRepeatCount;
  byte numberOfLEDsInSequence;
  int stepWidth;
  int lengthOfTable;
  int indexToStartOfThrobTable;
  int indexToEndOfThrobTable;
  int initialTableIndexAnchor;
  int finalTableIndexAnchor;
  int tableIndexAnchor;
  int tableIndex;
  byte pinNum;

  //
  // start with all the LEDs off, especially those outside firstLEDNumber through lastLEDNumber
  //
  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    ledPwm[pinNum] = 0;

  //
  // precompute some values to speed up the loop
  //
  numberOfLEDsInSequence = lastLEDNumber - firstLEDNumber + 1;
  stepWidth = throbTableLength / numberOfLEDsIlluminatedAtOnce ;
  lengthOfTable = numberOfLEDsInSequence * stepWidth;
  indexToStartOfThrobTable = lengthOfTable - throbTableLength;
  indexToEndOfThrobTable = indexToStartOfThrobTable + throbTableLength;
  lastRepeatCount = repeat - 1;

  //
  // begin before the table so LEDs fade in to start
  //
  initialTableIndexAnchor = -numberOfLEDsIlluminatedAtOnce * stepWidth;

  //
  // loop once for each time the sequence is repeated
  //
  for (repeatCount = 0; repeatCount <= lastRepeatCount; repeatCount++)
  { //
    // if this is the last repeat cycle, extend the length of the table so the LEDs will fade out at the end
    //
    if (repeatCount == lastRepeatCount)
      finalTableIndexAnchor = 2 *lengthOfTable;
    else
      finalTableIndexAnchor = lengthOfTable; 

    //
    // march from one end of the table to the other causing the LED to sequence from firstLEDNumber to lastLEDNumber
    //
    for(tableIndexAnchor = initialTableIndexAnchor; tableIndexAnchor < lengthOfTable; tableIndexAnchor++)
    { 
      tableIndex = tableIndexAnchor;

      //
      // set a PWM value for each LED, selecting the PWM value for so they are equally spaced across the table
      //
      for(pinNum = firstLEDNumber; pinNum <= lastLEDNumber; pinNum++)
      { //
        // our table includes a blanking region before and after the actual throb table
        //
        if ((tableIndex < indexToStartOfThrobTable) || (tableIndex >= indexToEndOfThrobTable))
          ledPwm[pinNum] = 0;
        else
          ledPwm[pinNum] = pgm_read_word(&throbTable[tableIndex - indexToStartOfThrobTable]);

        //
        // advance across the table by the step width, wrapping back after getting to the table's end
        //
        tableIndex += stepWidth;
        if (tableIndex >= finalTableIndexAnchor)
          tableIndex -= lengthOfTable;
      }

      //
      // now that all the PWM values have been set, display them by turning on the LEDs for a short period
      //
      pwmAllPins(LEDOnTime);

      //
      // quit if the user mode has changed
      //
      if(GetUserMode() != USER_MODE_MELLOW) 
        return;
    }

    //
    // since one cycle has been completed, reset the table starting point so next cycle will not fade in
    //
    initialTableIndexAnchor = 0;
  }
}



//
// wave the LEDs
//    Enter:  firstLEDNumber = first LED number in the sequence
//            lastLEDNumber = last LED number in the sequence
//            tableStep = sets the phasing between the LEDs in the sin table
//            LEDOnTime = aproximate number of milliseconds LED is on for 
//              each PWM cycle, minimum value is 10 (or 10ms)
//            repeat = number of times the sequence repeats
//
void theWave(byte firstLEDNumber, byte lastLEDNumber, int tableStep, int LEDOnTime, int repeat)
{ 
  int repeatCount;
  byte pinNum;
  byte tableIndexAnchor;
  byte tableIndex;
  bool firstTimeFlag;
  byte i;

  //
  // start with all the LEDs off, especially those outside firstLEDNumber through lastLEDNumber
  //
  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
  {  
    ledPwm[pinNum] = 0;
    ledIndex[pinNum] = 0;
  }

  firstTimeFlag = true;
  tableIndexAnchor = 0;

  for (repeatCount = 0; repeatCount < repeat; repeatCount++)
  {        
    for(i = 0; i < throbTableLength; i++)
    { 
      tableIndexAnchor++;
      if (tableIndexAnchor >= throbTableLength)
        tableIndexAnchor = 0;

      tableIndex = tableIndexAnchor;

      for(pinNum = firstLEDNumber; pinNum <= lastLEDNumber; pinNum++)
      { 
        tableIndex += tableStep; 
        if (tableIndex >= throbTableLength) 
          tableIndex -= throbTableLength;

        ledPwm[pinNum] = pgm_read_word(&throbTable[tableIndex]);
        ledIndex[pinNum] = tableIndex;
      }

      //
      // now turn on the LEDs, if this is the first cycle, begin with a fade up
      //
      if (firstTimeFlag)
      { 
        fadeUpLEDs(LEDOnTime);
        firstTimeFlag = false;
      }
      else
        pwmAllPins(LEDOnTime);

      //
      // quit if the user mode has changed
      //
      if(GetUserMode() != USER_MODE_MELLOW) 
        return;
    }
  }

  //
  // all done, fade to black
  //
  fadeLEDsToBlack(LEDOnTime, 1);
}



//
// delay for the given period, also check if the User Mode has changed
//    Enter:   delayBetweenShows = amount to delay in ms (with a resolution of 10ms)
//    Exit:    true returned if User Mode has changed
//
bool delayBetweenDisplaysAndCheckIfUserModeNotMellow(int delayBetweenShows)
{ 
  int delay10ms;

  if(GetUserMode() != USER_MODE_MELLOW)
    return(true);

  delay10ms = delayBetweenShows / 10;

  while(delay10ms > 0)
  {  
    delay(10);

    if(GetUserMode() != USER_MODE_MELLOW)
      return(true);

    delay10ms--;
  }

  return(false);
}


//------------------------------------------------------------------
//                     General purpose functions
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



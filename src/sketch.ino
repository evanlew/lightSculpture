
//      ******************************************************************
//      *                                                                *fadeUpOneLED
//      *               DPEA LED Machine - Diana's footbal               *
//      *                                                                *
//      *             Stan Reifel                    9/18/12             *
//      *                                                                *
//      ******************************************************************

#include "std_functions.ino"


//
// top level setup function
//
void setup()
{  
  int pinNum;
  // configure the LED pins as outputs
  for(pinNum = firstLED; pinNum <= lastLED; pinNum++)
    pinMode(pinNum, OUTPUT);  

  // configure the on/off switch as an input
  pinMode(onOffSwitch, INPUT_PULLUP);

  // setup the serial port for print statement debugging
  Serial.begin(9600);
  Serial.println("HERE");
}

// main loop, select the display show based on the User Mode Setting
void loop()
{ 
  byte userMode; 
  // check if the user has selected the Simple mode
   if (GetUserMode() == USER_MODE_SIMPLE)
   {
     simpleLEDDisplays();
     Serial.println("simple");
   }

  // check if the user has selected the Mellow mode
   if (GetUserMode() == USER_MODE_MELLOW)
   {
      mellowLEDDisplays();
      Serial.println("mellow");
  }

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
  evanSimpleMellow();
  // const int delayBetweenShows = 800;

  // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
  //   return;
  // showColorsMellowOne(100);
 // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
 //   return;

 // showColorsMellowTwo(10);
 // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
 //   return;

 // fadeUpSlowAndDownFast(60);
 // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
 //   return;

 // alternateThrobEveryOther(25, 3);
 // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
 //   return;

 // climbUp(24);
 // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
 //   return;

 // sequenceClockwiseWithFading(firstLED, lastLED, 2, 10, 1);
 // if(GetUserMode() != USER_MODE_MELLOW) return;
 // sequenceClockwiseWithFading(firstLED, lastLED, 4, 10, 1);
 // if(GetUserMode() != USER_MODE_MELLOW) return;
 // sequenceClockwiseWithFading(firstLED, lastLED, 6, 10, 1);
 // if(GetUserMode() != USER_MODE_MELLOW) return;
 // sequenceClockwiseWithFading(firstLED, lastLED, 8, 10, 1);
 // if(GetUserMode() != USER_MODE_MELLOW) return;
 // sequenceClockwiseWithFading(firstLED, lastLED, 10, 10, 1);
 // if(GetUserMode() != USER_MODE_MELLOW) return;
 // sequenceClockwiseWithFading(firstLED, lastLED, 12, 10, 1);
 // if(GetUserMode() != USER_MODE_MELLOW) return;
 // sequenceClockwiseWithFading(firstLED, lastLED, 13, 10, 5);
 // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
 //   return;    

 // theWave(firstLED, lastLED, 16, 10, 6);
 // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
 //   return;    

 // theWave(firstLED, lastLED, 35, 10, 6);
 // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
 //   return;    

 // sequenceClockwiseWithFading(firstLED, lastLED, 8, 40, 2);
 // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
 //   return;    

 // if (delayBetweenDisplaysAndCheckIfUserModeNotMellow(delayBetweenShows))
 //   return;
}


void evanSimpleMellow()
{
  int pwm = 0;
  for(int tableIndex = 0; tableIndex < throbTableLength; tableIndex++)
  {
    pwm = pgm_read_word(&throbTable[tableIndex]);
    ledPwm[2] = pwm;
    ledIndex[2] = tableIndex;

    pwmAllPins(50);
  }
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



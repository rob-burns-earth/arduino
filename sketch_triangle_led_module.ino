/* sketch for triangle led module
 *  
 *  pinout:
 *  8 - shift - data in
 *  12 - shift - latch
 *  13 - shift - clock
 *  
 *  LEDs
 * 
 *  1   3   8   6   5
 *  9   4   10  7
 *  14  12  11  
 *  15  13          2
 *  16
 *
 */

/*
 * http://www.binaryhexconverter.com/binary-to-decimal-converter
 *
 * top to bottom:
 * 1110 1101 0000 0000  60672
 * 0101 0010 1100 0000  21184
 * 0100 0000 0011 0100  16436
 * 0100 0000 0000 1010  16394
 * 0100 0000 0000 0001  16385
 *
 * left to right:
 * 1100 0000 1000 0111  49287
 * 0111 0000 0001 1000  28696  
 * 0100 0001 0110 0000  16736  
 * 0100 0110 0000 0000  17920  
 * 0100 1000 0000 0000  18432
 * 
 * diagonal:
 * 0100 1010 0010 1001  18985
 * 0100 0100 0101 0010  17490  
 * 0101 0001 0000 0100  20740  
 * 0110 0000 1000 0000  24704
 * 1100 0000 0000 0000  49152     
 *
 * corner out:
 * 1100 0000 0000 0000  49152    
 * 0111 0000 1000 0000  28800    
 * 0100 0001 0111 0100  16756
 * 0100 0110 0000 1010  17930  
 * 0100 1000 0000 0001  18433  
 *
 */

long patternTopDown[9] = {60672, 21184, 16436, 16394, 16385, 16394, 16436, 21184, 60672}; 
long patternLeftRight[9] = {49287, 28696, 16736, 17920, 18432, 17920, 16736, 28696, 49287}; 
long patternDiagonal[9] = {18985, 17490, 20740, 24704, 49152, 24704, 20740, 17490, 18985}; 
long patternCornerOut[9] = {49152, 28800, 16756, 17930, 18433, 17930, 16756, 28800, 49152}; 
 
const int LED_BACKLIGHT = 2;

const int PIN_SHIFT_DATA = 8;
const int PIN_SHIFT_LATCH = 12;
const int PIN_SHIFT_CLOCK = 13;

long randomNumber = 0.0;  // variable to hold the random numbers generated throughout the program (to avoid creating/deleting)

int delayDiagnosticAmount = 100;
int delayAmount = 500;            // milliseconds to wait between steps
int delayPatternStepAmount = 100; // milliseconds to wait between steps
int delayRGB = 200;               // milliseconds to wait between RGB steps
int stepVal = 10;                 // how much to increment (0-255) for the RGB
int stepsBeforeLedChange = 10;    // used to control how many steps before randomizing the 24 leds
int lastStep = 10;                // used to keep track of how many steps we taken

int probabilityOfTurningOnLed = 87;   // % chance each led has of being turned on when we randomize 

void setup() {
  pinMode(PIN_SHIFT_DATA, OUTPUT);
  pinMode(PIN_SHIFT_LATCH, OUTPUT);
  pinMode(PIN_SHIFT_CLOCK, OUTPUT);

  turnAllOff();
  delay(1000);

  turnAllOn();
  delay(1000);

  turnAllOff();
  delay(1000);
}

void loop() {
  
  showRandomPattern(4);
  
  for(int i=0; i<4; i++) {
    showPattern(patternTopDown, 9);
  }

  for(int i=0; i<4; i++) {
    showPattern(patternLeftRight, 9);
  }

  for(int i=0; i<4; i++) {
    showPattern(patternDiagonal, 9);
  }

  for(int i=0; i<4; i++) {
    showPattern(patternCornerOut, 9);
  }

}

void showRandomPattern(int steps) {
  for(int i=0; i<steps; i++) {
    randomize16Leds();
    delay(delayAmount);
  }
}

void turnAllOff() {
  digitalWrite(PIN_SHIFT_LATCH, LOW);
  for(int i=0;i<16;i++) {
    writeData(LOW);
  }
  digitalWrite(PIN_SHIFT_LATCH, HIGH);  
}

void turnAllOn() {
  digitalWrite(PIN_SHIFT_LATCH, LOW);
  for(int i=0;i<16;i++) {
    writeData(HIGH);
  }
  digitalWrite(PIN_SHIFT_LATCH, HIGH);  
}

void writeData(int data) {
  digitalWrite(PIN_SHIFT_CLOCK, LOW);
  digitalWrite(PIN_SHIFT_DATA, data);
  digitalWrite(PIN_SHIFT_CLOCK, HIGH);  
}

void randomize16Leds() {
  digitalWrite(PIN_SHIFT_LATCH, LOW);
  for(int i=0; i<16; i++) {
    randomNumber = random(0, 100);
    // turn on each led only probabilityOfTurningOnLed % of the time
    if(randomNumber <= probabilityOfTurningOnLed) {
      writeData(HIGH);
    }
    else {
      writeData(LOW);
    }
  }
  digitalWrite(PIN_SHIFT_LATCH, HIGH);  
}

void showPattern(long patternSteps[], int steps) {
  // interate through each step of the of pattern
  for(int i=0; i<steps; i++) {

    // start the shift register
    digitalWrite(PIN_SHIFT_LATCH, LOW);
    
    // for each led read the corresponding bit from the pattern to determine if the led is on or off
    for(int pos=0; pos<16; pos++) {
      // read the bit for the current step of the pattern and write it to the shift register
      writeData(bitRead(patternSteps[i], pos));
    }

    // save the shift register
    digitalWrite(PIN_SHIFT_LATCH, HIGH);  
    
    // add a delay
    delay(delayPatternStepAmount);
  } 
  
}  


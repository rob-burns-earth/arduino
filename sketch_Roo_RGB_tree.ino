/* sketch for Roo's RGB tree
 *  
 *  pinout:
 *  8 - shift - data in
 *  9 - RGB - 
 *  10 - RGB - 
 *  11 - RGB - 
 *  12 - shift - latch
 *  13 - shift - clock
 *  
 */

const int PIN_RGB_R = 9;
const int PIN_RGB_G = 10;
const int PIN_RGB_B = 11;
const int PIN_SHIFT_DATA = 8;
const int PIN_SHIFT_LATCH = 12;
const int PIN_SHIFT_CLOCK = 13;

int newR;   // hold the Red value for the RGB color we're transitioning to
int newG;   // hold the Green value for the RGB color we're transitioning to
int newB;   // hold the Blue value for the RGB color we're transitioning to

int lastR;  // hold the Red value for the RGB color currently being shown
int lastG;  // hold the Green value for the RGB color currently being shown
int lastB;  // hold the Blue value for the RGB color currently being shown

long randomNumber = 0.0;  // variable to hold the random numbers generated throughout the program (to avoid creating/deleting)

int delayDiagnosticAmount = 100;
int delayAmount = 10;          // milliseconds to wait between steps
int delayRGB = 200;             // milliseconds to wait between RGB steps
int stepVal = 10;               // how much to increment (0-255) for the RGB
int stepsBeforeLedChange = 10;  // used to control how many steps before randomizing the 24 leds
int lastStep = 10;              // used to keep track of how many steps we taken

int probabilityOfTurningOnLed = 87;   // % chance each led has of being turned on when we randomize 

void setup() {
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);
  pinMode(PIN_SHIFT_DATA, OUTPUT);
  pinMode(PIN_SHIFT_LATCH, OUTPUT);
  pinMode(PIN_SHIFT_CLOCK, OUTPUT);

  turnAllOff();
}

void loop() {
  if (lastStep == stepsBeforeLedChange) { 
    // turn on random leds
    randomize24Leds();
    lastStep = 1;
  }

  //pick the next RGB color to transition to
  newR = random(0, 255);
  newG = random(0, 255);
  newB = random(0, 255);

  transitionRGB();
}

void turnAllOff() {
  digitalWrite(PIN_SHIFT_LATCH, LOW);
  for(int i=0;i<24;i++) {
    writeData(LOW);
  }
  digitalWrite(PIN_SHIFT_LATCH, HIGH);  
}

void showColor(int r, int g, int b) {
  analogWrite(PIN_RGB_R,r);
  analogWrite(PIN_RGB_G,g);
  analogWrite(PIN_RGB_B,b);
}

void writeData(int data) {
  digitalWrite(PIN_SHIFT_CLOCK, LOW);
  digitalWrite(PIN_SHIFT_DATA, data);
  digitalWrite(PIN_SHIFT_CLOCK, HIGH);  
}

void randomize24Leds() {
  digitalWrite(PIN_SHIFT_LATCH, LOW);
  for(int i=0;i<24;i++) {
    randomNumber = random(0, 100);
    // turn on each led only probabilityOfTurningOnLed % of the time
    if(randomNumber <= probabilityOfTurningOnLed) {
      writeData(HIGH);
    }
    else {
      writeData(LOW);
    }
    delay(delayAmount);
  }
  digitalWrite(PIN_SHIFT_LATCH, HIGH);  
}

int sign(int val) {
  if(val < 0) {
    return -1;
  }
  else if(val == 0) {
    return 0;
  }
  else {
    return 1;
  }
}

void transitionRGB() {
  // recursive function to transition from last RGB value to new RGB value   

  // determine the new value after advancing one step - note that we're passing by value
  lastR = calcValueAfterStep(lastR, newR);
  lastG = calcValueAfterStep(lastG, newG);
  lastB = calcValueAfterStep(lastB, newB);

  // show the new color on the RGB led
  showColor(lastR, lastG, lastB);
  delay(delayRGB);

  // every stepsBeforeLedChange * delayAmount milliseconds randomize the 24 leds and reset the counter
  if (lastStep == stepsBeforeLedChange) { 
    // turn on random leds
    randomize24Leds();
    lastStep = 0;
  }

  // increment the step counter
  lastStep = lastStep + 1;

  if (!((lastR == newR) && (lastG == newG) && (lastB == newB))) {
    // recurse until we reach the new R, G and B values
    transitionRGB();
  }
}

int calcValueAfterStep(int fromVal, int toVal) {
  // calculate if the value needs to increment, decrement or already equal to target value
  int signOfStep = sign(toVal - fromVal);
  
  int tempVal = fromVal + (signOfStep * stepVal);

  // ensure we don't overshoot the target value (eg from fromVal=10 to toVal=5 by stepVal=10, newVal=5 (not fromVal-stepVal=0)
  if ((signOfStep > 0) && (tempVal > toVal)) tempVal = toVal;
  if ((signOfStep < 0) && (tempVal < toVal)) tempVal = toVal;

  return tempVal;
}



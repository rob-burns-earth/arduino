/* sketch for Roo's RGB tree
 *
 * This arduino sketch is for a twisted wire tree - with a 3mm led at the end of each branch, and an RGB led in the heart 
 * of the tree.  There are 24 3mm leds (branches) of different colours (r, b, g, y).  The branch leds are driven by 3 shift 
 * registers (3 shift registers * 8 output pins = 24 leds driven by 3 arduino output pins)
 *
 * This sketch does two interesting things:
 *  1 - transitions the RGB led smoothly from one random colour to another using a simple linear algorithm.  It uses a random number between 0 - 6
 *      to determine what the new colour to transition to is (0 = red, 1 = green, ... 5 = purple, 6 = random)
 *
 *  2 - randomly turns the 24 branch leds on and off - while the RGB is transitioning (which is why I'm keeping track of the 
 *      steps of the RGB transition - eg stepVal, lastStep, stepsBeforeLedChange variables).  
 *      I wanted the branch leds on more than they are off but wanted some randomness so probabilityOfTurningOnLed variable
 *      controls the chance that any given led will be turned on each cycle.  Just reduce this value if you them off more of 
 *      the time
 * 
 * TODO: add randomSeed(seed) to make a bit more random (though not likely necessary given the multiple layers of randomness)
 * 
 *  pinout:
 *  8 - shift - data in
 *  9 - RGB - blue
 *  10 - RGB - red
 *  11 - RGB - green
 *  12 - shift - latch
 *  13 - shift - clock
 *  
 */

const int PIN_RGB_R = 10;
const int PIN_RGB_G = 11;
const int PIN_RGB_B = 9;
const int PIN_SHIFT_DATA = 8;
const int PIN_SHIFT_LATCH = 12;
const int PIN_SHIFT_CLOCK = 13;

const int RGB_red[3] = {255, 0, 0};
const int RGB_green[3] = {0, 255, 0};
const int RGB_blue[3] = {0, 0, 255};
const int RGB_yellow[3] = {255, 255, 0};
const int RGB_cyan[3] = {0, 255, 255};
const int RGB_purple[3] = {255, 0, 255};

int colour_choice = 0;

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
int stepVal = 7;               // how much to increment (0-255) for the RGB
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
  colour_choice = random(0,7);
  choose_color(colour_choice);

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
    randomNumber = random(0, 101);
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

void choose_color(int colour) {
    switch (colour) {
    case 0:
    {
      newR = RGB_red[0];
      newG = RGB_red[1];
      newB = RGB_red[2];
      break;
      }
    case 1:
    {
      newR = RGB_green[0];
      newG = RGB_green[1];
      newB = RGB_green[2];
      break;
      }
    case 2:
    {
      newR = RGB_blue[0];
      newG = RGB_blue[1];
      newB = RGB_blue[2];
      break;
      }
    case 3:
    {
      newR = RGB_yellow[0];
      newG = RGB_yellow[1];
      newB = RGB_yellow[2];
      break;
      }
    case 4:
    {
      newR = RGB_cyan[0];
      newG = RGB_cyan[1];
      newB = RGB_cyan[2];
      break;
      }
    case 5:
    {
      newR = RGB_purple[0];
      newG = RGB_purple[1];
      newB = RGB_purple[2];
      break;
      }
    default:
    {
      newR = random(0, 256);
      newG = random(0, 256);
      newB = random(0, 256);
      break;
      }
    }
}

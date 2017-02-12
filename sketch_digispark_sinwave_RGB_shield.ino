/*
 * digispark sin wave RGB
 *  custom board with RGB led and 3 static leds (plus 1 capacitor)
 * 
 * pinout:
 * 0 = RGB - blue
 * 1 = RGB - green
 * 2 = static blue led
 * 3 = 
 * 4 = RGB - red
 * 5 = 
 */

// pin variables (switch pins to match UNO PWM pins)
const int PIN_RGB_B = 0;
const int PIN_RGB_G = 1;
const int PIN_RGB_R = 4;
const int PIN_BLUE = 2;

//this a variable that will receive the angle value from variable i. This value is converted to radians in the sine function and will be used to generate the PWM values 
float x;
// these variables will receive the PWM values calculated by the three sine functions
float r; 
float g;
float b;

void setup() {
  pinMode(PIN_RGB_B, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_R, OUTPUT);
  
  pinMode(PIN_BLUE, OUTPUT);
//  digitalWrite(PIN_BLUE, HIGH);

//  runDiagnostics();
}

void loop() {
 /*
  * The for loop generates a value for a variable i which corresponds to 0 to 360 degrees. 
  * i is increased by 1 with each iteration.  
  * It is later converted to radians within the loop. 
  * Once i reaches 360 it resets back to 0. 
  * This establishes the periodic behaviour of the sine functions
  */

  for (int i=0; i<360; i++)
  {
    x=float(i);
    calcMethodGreenPurple(x);
  }

  for (int i=0; i<360; i++)
  {
    x=float(i);
    calcMethodBlueGreen(x);
  }

  calcMethod
}

void runDiagnostics() {
  // diagnostic test to varify led
  digitalWrite(PIN_RGB_B, HIGH);
  delay(1000);
  digitalWrite(PIN_RGB_G, HIGH);
  delay(1000);
  digitalWrite(PIN_RGB_R, HIGH);
  delay(1000);
  digitalWrite(PIN_RGB_B, LOW);
  delay(1000);
  digitalWrite(PIN_RGB_G, LOW);
  delay(1000);
  digitalWrite(PIN_RGB_R, LOW);
  delay(1000);
}

void showColor(int r, int g, int b) {
  analogWrite (PIN_RGB_B,b);
  analogWrite (PIN_RGB_G,g);
  analogWrite (PIN_RGB_R,r);
  
  //wait for 1/100 of a second
  delay(10);
}

void calcMethodBlueGreen(float x) {
  /* to calculate r,g,b the sine function is modified to increase amplitute (127*) 
  * to create a phase shift (x+1/2*PI) and (x+3/2*PI) 
  * finally the sine wave is raised to illiminate negative values below zero by adding 1
  */
  
  r=int(127*(sin(x/180*PI)+1));
  g=int(127*(sin(x/180*PI+2/3*PI)+1));
  b=int(127*(sin(x/180*PI+4/3*PI)+1));
  
  showColor(r, g, b);
}

void calcMethodGreenPurple(float x) {
  r=int(127*(sin(x/180*PI)+1));
  g=int(127*(sin(x/180*PI+3/2*PI)+1));     
  b=int(127*(sin(x/180*PI+1/2*PI)+1));
  
  showColor(r, g, b);
}

void calcMethod(float x) {
  r=int(127*(sin(x/180*PI)));
  g=int(127*(sin(x/180*PI+3/2*PI)));     
  b=int(127*(sin(x/180*PI+1/2*PI)));
  
  showColor(r, g, b);
}



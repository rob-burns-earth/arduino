// revamped truck sensor board

// pinout - "eyes" = top center, female pin headers = botton left, pin 1 = bottom left
// power bar:
//    1  = power into board
//    2  = ground
//    3  = infra-red analog in  - can be removed if needed (use digital instead)
//    4  =   
//
// digital bar:
//    1  = ultra-sonic trigger
//    2  = ultra-sonic echo
//    3  = infra-red digital in  
//    4  = blue led
//    5  = green led
//    6  = 
//    7  = 
//    8  = 
//    9  = 
//    10 = 

const int pin_ir_detector_analog = A0;

const int pin_obsticle_detector_trigger = 8;
const int pin_obsticle_detector_echo = 9;
const int pin_ir_detector_digital = 10;
const int pin_blue_led = 11;
const int pin_green_led = 12;
const int pin_onboard_led = 13;

// set some thresholds for the ultrasonic obsticle detector
const long threshold_nearing_object = 1000;
const long threshold_collision_imminent = 500;

long duration = 0;            // duration in microseconds
float forwardDistance = 0.0;  // distance to object in front of robot in cm
bool onGround = false;
bool blueLedOn = false;

void setup(){
  Serial.begin(9600);

  // input pins
  pinMode(pin_ir_detector_analog, INPUT);
  pinMode(pin_ir_detector_digital, INPUT);
  pinMode(pin_obsticle_detector_echo, INPUT);

  // output pins
  pinMode(pin_obsticle_detector_trigger, OUTPUT);
  pinMode(pin_blue_led, OUTPUT);
  pinMode(pin_green_led, OUTPUT);

  pinMode(pin_onboard_led, OUTPUT);
  digitalWrite(pin_onboard_led, LOW);
 
  // turn on green light indicating powered
  digitalWrite(pin_green_led, HIGH);
}

void loop(){
    // give priority to determining if the ground is still there (ie not at a stairs/drop-off or picked up)
    // force to digital value - since range is only 4 cms or so, and we just want to know if the obsticle (ground) is there
    onGround = !digitalRead(pin_ir_detector_digital);
  
    Serial.println("onGround=" + String(onGround, DEC));
    
    //debugging 
    onGround = 1;
    
    if (!onGround) {
      // stop the motors
      
      // turn off blue led - indicating not on the ground (stairs, picked up, flying, etc)
      digitalWrite(pin_blue_led, LOW);
      blueLedOn = false;
    }
    else {
      // turn on blue led - indicating on the ground
      if (blueLedOn == false) {
        // use variable to avoid keep setting value if already set
        digitalWrite(pin_blue_led, HIGH);
        blueLedOn = true;
      }
      
      // determine distance to object infront of car
      determineForwardDistance();
      
      // within range of an object - either slow down or turn
      if (forwardDistance <= threshold_nearing_object) {
        // about to hit something
        // slow down and turn
        if (forwardDistance <= threshold_collision_imminent) {
          Serial.println("collision imminent");

        }
        else {
          Serial.println("distance within threshold");
          
        }
      }
      else {
        // clear road ahead, let 'er rip
        Serial.println("all clear ahead");
      
      }
    }
    
    //debugging delay
    delay(2000);
 }

void determineForwardDistance() {
  Serial.println("determineForwardDistance");
  
  // send out a ulta-sonic pulse and listen for echo
  // turn off the trig for 2 ps then turn on for 10 ps then turn off and read echo duration
  digitalWrite(pin_obsticle_detector_trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pin_obsticle_detector_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_obsticle_detector_trigger, LOW);
  duration = pulseIn(pin_obsticle_detector_echo, HIGH);

  // distance (cm) based on speed of sound
  // distance = duration/58.2;
  forwardDistance = duration * 0.034/2;

  Serial.print("forwardDistance=");
  Serial.println(forwardDistance);
}


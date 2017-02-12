#include <NewPing.h>

// pinout
int pin_leftSpeed = 6;
int pin_rightSpeed = 5;
int pin_left = 8;
int pin_right = 7;

int pin_trigger = 12;
int pin_echo = 11;
#define MAX_DISTANCE 200

// create a local instance of the sonar object
NewPing sonar1(pin_trigger, pin_echo, MAX_DISTANCE);

// speed settings
int fastSpeed = 255;
int slowSpeed = 175;

// set some thresholds for the ultrasonic obsticle detector
const long threshold_nearing_object = 20;
const long threshold_collision_imminent = 10;

float forwardDistance = 0.0;  // distance to object in front of robot in cm

void setup() {
  
  pinMode(pin_leftSpeed, OUTPUT);
  pinMode(pin_rightSpeed, OUTPUT);
  pinMode(pin_left, OUTPUT);
  pinMode(pin_right, OUTPUT);

  pinMode(pin_trigger, OUTPUT);
  pinMode(pin_echo, INPUT);
  
//  Serial.begin(9600);
}

void loop() {
  // determine distance to object infront of car
  determineForwardDistance();

  // within range of an object - either slow down or turn
  if (forwardDistance <= threshold_nearing_object) {
    // about to hit something
    // slow down and turn
    if (forwardDistance <= threshold_collision_imminent) {
//      Serial.println("collision imminent");
      stop();
    }
    else {
//      Serial.println("distance within threshold");
      forward(slowSpeed);
    }
  }
  else {
    // clear road ahead, let 'er rip
//    Serial.println("all clear ahead");
    forward(fastSpeed);
  }
}

void determineForwardDistance() {
//  Serial.println("determineForwardDistance");
  
  forwardDistance = sonar1.ping() / US_ROUNDTRIP_CM;

//  Serial.print("forwardDistance=");
//  Serial.println(forwardDistance);
}

void forward(int speedVal) {
  analogWrite(pin_leftSpeed, speedVal);
  digitalWrite(pin_left, HIGH);
  analogWrite(pin_rightSpeed, speedVal);
  digitalWrite(pin_right, HIGH);
}

void reverse(int speedVal) {
  analogWrite(pin_leftSpeed, speedVal);
  digitalWrite(pin_left, LOW);
  analogWrite(pin_rightSpeed, speedVal);
  digitalWrite(pin_right, LOW);
}

void stop() {
  analogWrite(pin_leftSpeed, 0);
  analogWrite(pin_rightSpeed, 0);
}


void forwardRight() {
  
}





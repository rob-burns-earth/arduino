// 16 led controlled by 2 shift registers - tree


// pinout
const int pin_data = 8;
const int pin_latch = 9;
const int pin_clock = 10;

long randomNumber = 0.0;
long delayAmount = 0;

void setup() {
  pinMode(pin_latch, OUTPUT);  
  pinMode(pin_data, OUTPUT);  
  pinMode(pin_clock, OUTPUT);  

//  Serial.begin(9600);

}

void loop() {

  randomize();

/*
  randomize();
  turnAllOn();
  turnAllOff();
  turnOn1AtaTime();
*/

}

void writeData(int data) {
  digitalWrite(pin_clock, LOW);
  digitalWrite(pin_data, data);
  digitalWrite(pin_clock, HIGH);  
}

void turnAllOff() {
  digitalWrite(pin_latch, LOW);
  for(int i=0;i<16;i++) {
    writeData(LOW);
    delay(500);
  }
  digitalWrite(pin_latch, HIGH);  
}

void turnAllOn() {
  digitalWrite(pin_latch, LOW);  
  for(int i=0;i<8;i++) {
    writeData(HIGH);
  }
  digitalWrite(pin_latch, HIGH);
//  delay(500);
}

void turnOn1AtaTime() {
  for(int j=0;j<16;j++) {
    digitalWrite(pin_latch, LOW);
    for(int i=0;i<8;i++) {
      if(j==i) {
        writeData(HIGH);
      }
      else {
        writeData(LOW);
      }
    }
    digitalWrite(pin_latch, HIGH);  
    delay(500);
  }
} 


void randomize() {
  digitalWrite(pin_latch, LOW);
  for(int i=0;i<16;i++) {
    randomNumber = random(0, 100);
    // turn off each led only 13% of the time
    if(randomNumber >= 13) {
      writeData(HIGH);
    }
    else {
      writeData(LOW);
    }  
    randomNumber = random(50, 200);
//    Serial.println(randomNumber);
    delayAmount = (long) randomNumber;
    delay(delayAmount);
  }
  digitalWrite(pin_latch, HIGH);  
}


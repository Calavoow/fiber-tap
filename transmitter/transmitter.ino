#include <Event.h>
#include <Timer.h>

// LED pin
int led = 2;
short ledCounter = 0;
boolean ledOn = false;

// Timers
Timer ledTimer;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  ledTimer.every(20, updateLed);
}

void updateLed() {
  ledCounter++;
  
  if(ledOn){
    digitalWrite(led, LOW);
    ledOn = false;
    ledCounter = 0;
  } else if(ledCounter == 50) {
    digitalWrite(led,HIGH);
    ledOn = true;
  }
}

void loop() {
  ledTimer.update();
}

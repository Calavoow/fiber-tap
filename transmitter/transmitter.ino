#include <Event.h>
#include <Timer.h>

// LED pin
int led = 2;
short ledCounter = 0;
boolean ledOn = false;

// Timers
Timer ledTimer;

// Photoresistor pin
int photo = 4;
int photoSensor = A0;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(photo, INPUT_PULLUP);
  
  ledTimer.every(20, updateLed);
}

/**
* Turns led on or off depending on the ledCounter.
* Is called every 20ms.
*/
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
  
  int photoVal = digitalRead(photo);
  int sensorVal = analogRead(photoSensor);
  if(sensorVal < 200) {
    Serial.println(millis());
    Serial.println("On!");
  }
}

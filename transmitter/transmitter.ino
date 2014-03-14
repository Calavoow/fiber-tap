#include <Manchester.h>
#include <Event.h>
#include <Timer.h>

// LED pin
int led = 4;
short ledCounter = 0;
boolean ledOn = false;

// Timers
Timer ledTimer;
Timer TXTimer;

// Photoresistor pin
int photoSensor = A0;

#define BUFFER_SIZE 10
uint8_t buffer[BUFFER_SIZE];
void setup() {
  Serial.begin(9600);
  
  //Use a pullup as input pin for the photoresistor.
  man.setupTransmit(led, MAN_19200);
  man.setupReceiveAnalog(photoSensor, MAN_19200, 200);
  man.beginReceive();
  
  TXTimer.every(500, transmitDataFunc);
  
  // Wait for Serial to connect to so output.
  while(!Serial) ;
  Serial.println("begin");
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

int minimum = 30000;
void readSensor() {
  int sensorVal = analogRead(photoSensor);
  Serial.print("Sensor Value: ");
  Serial.println(sensorVal);
  minimum = min(minimum, sensorVal);
  Serial.print("Minimum Value: ");
  Serial.println(minimum);
  
  if(sensorVal < 200) {
    Serial.println(millis());
    Serial.println("On!");
  }
}

uint8_t txData = 0;
int iteration = 0;
void transmitDataFunc() {
  if(iteration < (16 + 8*sizeof(txData))) {
    man.transmitArrayNonBlocking(sizeof(txData), &txData, iteration);
  } else {
    Serial.println("Done");
    txData++;
    iteration=0;
  }
  Serial.println("Continuing");
}

void loop() {
  Serial.println("Loop");
  TXTimer.update();
  if(man.receiveComplete()) {
    uint16_t m = man.getMessage();
    man.beginReceive();
    Serial.println(m);
  }
}

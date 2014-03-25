#include <Manchester.h>
#include <Event.h>
// #include <Timer.h>

// LED pin
int led = 4;
short ledCounter = 0;
boolean ledOn = false;

// Timers
// Timer ledTimer;
// Timer TXTimer;

// Photoresistor pin
int photoSensor = A0;

#define BUFFER_SIZE 10
uint8_t buffer[BUFFER_SIZE];
void setup() {
  Serial.begin(300);
  
  //Use a pullup as input pin for the photoresistor.
  man.setupTransmit(led, MAN_300);
  //man.setupReceiveAnalog(photoSensor, MAN_19200, 200);
  // man.beginReceive();
  
  //TXTimer.every(500, transmitDataFunc);
  
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
int totalIterations = 2*(16+8*sizeof(txData));
int iteration = 0;
/**
 * Try to transmit another data fragment in a nonblocking manner.
 *
 * IMPORTANT NOTICE: If this function is not called often enough, then data transmission will fluctuate.
 **/
void transmitDataNonBlocking() {
  //Serial.println(iteration);
  if(iteration < 2*(16 + 8*sizeof(txData))) {
    // If succesfully executed
    if(man.transmitArrayNonBlocking(sizeof(txData), &txData, iteration)) {
      // Increase iteration
      iteration++;
    } else {
      Serial.println("Skipped");
    }
  } else {
    Serial.println("Done");
    txData++;
    iteration=0;
  }
}

void loop() {
  //transmitDataNonBlocking();
//  man.transmit(15);
  Serial.println("Hi");
  man.transmit(15);
  Serial.println("HI2");
  //TXTimer.update();
  /*if(man.receiveComplete()) {
    uint16_t m = man.getMessage();
    man.beginReceive();
    Serial.println(m);
  }*/
}



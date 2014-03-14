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

unsigned char data = 0xA1;
char transmitCounter = 0;
unsigned char transmitData[16];

void transmit(){
  if(transmitCounter >= 16) {
    transmitCounter = 0;
    return;
  }
  
  Serial.println("Transmitting!");
  Serial.println(millis());
  unsigned char data = transmitData[transmitCounter];
  Serial.println(data);
  if(data){
    Serial.println("High");
    digitalWrite(led, HIGH);
  } else {
    Serial.println("Low");
    digitalWrite(led,LOW);
  }
  
  transmitCounter++;
}

void manchesterData(byte data) {;
  int i = 0;
    for(; i < 8; i+=2){
      unsigned char bit = data & (1 << i);
      if( bit ){
        //high-to-low
	transmitData[i] = 1;
	transmitData[i+1] = 0;
      } else {
	//low-to-high
        transmitData[i] = 0;
        transmitData[i+1] = 1;
      }
    }
}

#define BUFFER_SIZE 10
uint8_t buffer[BUFFER_SIZE];
void setup() {
  Serial.begin(9600);
  
  //pinMode(led, OUTPUT);
  
  //Use a pullup as input pin for the photoresistor.
  man.setupTransmit(led, MAN_19200);
  pinMode(photoSensor, INPUT_PULLUP);
  man.setupReceiveAnalog(photoSensor, MAN_19200, 200);
  man.beginReceive();
  
  
  TXTimer.every(500, transmitDataFunc);
  // manchesterData(data);
  
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

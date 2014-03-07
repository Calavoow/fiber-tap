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

void setup() {
  Serial.begin(9600);
  
  pinMode(led, OUTPUT);
  
  //Use a pullup as input pin for the photoresistor.
  pinMode(photoSensor, INPUT_PULLUP);
  
  //ledTimer.every(20, updateLed);
  TXTimer.every(10, transmit);
  TXTimer.after(1000, ledOff);
  manchesterData(data);
  
  // Wait for Serial to connect to so output.
  while(!Serial) ;
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

void ledOff() {
  digitalWrite(led, LOW);
}

int minimum = 1000;
void loop() {
  //ledTimer.update();
  TXTimer.update();
  
  int sensorVal = analogRead(photoSensor);
  Serial.print("Sensor Value: ");
  Serial.println(sensorVal);
  minimum = min(minimum, sensorVal);
  Serial.print("Minimum Value: ");
  Serial.println(minimum);
  
  if(sensorVal < 30) {
    Serial.println(millis());
    Serial.println("On!");
  }
}

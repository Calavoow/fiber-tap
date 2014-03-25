#include <Manchester.h>

// Photoresistor pin
int photoSensor = A0;

void setup() {
	Serial.begin(9600);
	
	man.setupReceiveAnalog(photoSensor, MAN_9600, 200);
	man.beginReceive();
	
	// Wait for Serial to connect to so output.
	while(!Serial) ;
	Serial.println("begin");
}

/**
 * Read a sensor value. Used for debugging the threshold.
 **/
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

/**
 * The main receiver program loop.
 **/
void loop() {
	// Receive data packets.
	if(man.receiveComplete()) {
		uint16_t m = man.getMessage();
		// Get ready to receive next packet
		man.beginReceive();
		Serial.println(m);
	}
}



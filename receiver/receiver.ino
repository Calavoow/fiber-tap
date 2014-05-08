#include <Manchester.h>

// Photoresistor pin

int photoSensor = A0;

void setup() {
	Serial.begin(9600);

	// Use analog pin as digital
	man.setupReceive(photoSensor, MAN_300);
//		  pinMode(photoSensor, INPUT);
		
	// Wait for Serial to connect to so output.
	while(!Serial) ;
	Serial.println("begin");

		man.beginReceive();
}

/**
 * Read a sensor value. Used for debugging the threshold.
 **/
 /*
int minimum = 30000;
int maximum = 0;
int iteration = 0;
void readSensor() {
	int sensorVal = analogRead(photoSensor);
	minimum = min(minimum, sensorVal);
	maximum = max(maximum, sensorVal);
	if(iteration > 1000){
		Serial.print("Maximum:");
		Serial.print(maximum);
		Serial.print("\tMinimum: ");
		Serial.print(minimum);
		Serial.println("");
		iteration = 0;
	}
	iteration++;
}
*/

// Buffer the received messages because printing corrupts the next message.
uint16_t msgs[128];
int msg_counter = 0;
/**
 * The main receiver program loop.
 **/
void loop() {
	/**
	 * Debug transfer
	int meas[128];
	int meas2[128];
	for(int i=0; i<128; i++){
		meas[i] = digitalRead(photoSensor);
		meas2[i] = analogRead(photoSensor);
	}
 
	for(int i=0; i<128; i++){
	   Serial.print(meas[i]);
	   Serial.print("\t");
	   Serial.println(meas2[i]);
	}
	Serial.println("-----------------");
	**/

	// Receive data packets.
	if(man.receiveComplete()) {
		uint16_t m = man.getMessage();
		// Get ready to receive next packet
		man.beginReceive();
		msgs[msg_counter] = m;
		msg_counter++;
			
		// Print received messages
		if(msg_counter > 127){
			for(int i=0; i<128; i++){
				Serial.println(msgs[i]);
			}
			msg_counter = 0;
		}
	}
}

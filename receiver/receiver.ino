#include <Manchester.h>
// Photo sensor pin
int photoSensor = A0;

void setup() {
	Serial.begin(9600);

	// Use analog pin as digital
	man.setupReceiveAnalog(photoSensor, MAN_300, 50);
	// Wait for Serial to connect to so output.
	while(!Serial);
	Serial.println("begin");

	man.beginReceive();
}

// Buffer the received messages
// because printing each char takes too long
// and corrupts the next receiver byte.
char password[25];
int msg_counter = 0;
/**
 * The main receiver program loop.
 **/
void loop() {
	// Receive data packets.
	if(man.receiveComplete()) {
		char m = man.getMessage();

		password[msg_counter] = m;
		msg_counter++;
		// Print received messages
		if(msg_counter > 24){
		    Serial.println(password);
		    msg_counter = 0;
		}
		// Get ready to receive next packet
		man.beginReceive();
	}
}

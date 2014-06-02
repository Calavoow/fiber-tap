#include <Manchester.h>
// LED pin
int led = 4;

void setup() {
	Serial.begin(9600);
  
	man.setupTransmit(led, MAN_300);
	// Wait for Serial to connect to output.
	while(!Serial) ;
	Serial.println("begin");
}

char password[26] = "correctHorseBatteryStaple";
int counter = 0;
void loop() {
	// Transmit some data
	Serial.println(password);
  
	man.transmit(password[counter]);

	// Set next character
	counter++;
	if(counter >= strlen(password)) counter = 0;
}

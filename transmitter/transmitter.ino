#include <Manchester.h>

// LED pin
int led = 4;

void setup() {
  Serial.begin(9600);
  
  man.setupTransmit(led, MAN_9600);
  
  // Wait for Serial to connect to so output.
  while(!Serial) ;
  Serial.println("begin");
}

void loop() {
  // Transmit some data
  man.transmit(15);
}

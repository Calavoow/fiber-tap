#include <Manchester.h>

// LED pin
int led = 4;

void setup() {
  Serial.begin(9600);
  
  man.setupTransmit(led, MAN_300);
  
  // Wait for Serial to connect to so output.
  while(!Serial) ;
  Serial.println("begin");
}

int16_t counter = 1;
void loop() {
  // Transmit some data
    Serial.println(counter);
  
    man.transmit(counter);

    counter++;
}

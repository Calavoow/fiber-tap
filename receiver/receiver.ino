const unsigned int rate = 50; // bit/s
const unsigned int halfBit = 500/rate; // delay in microseconds per half-bit, i.e. per transition.
// The wait time in microseconds until to the middle of the next flank.
// We wait until we are in the middle of a transmission flank.
const unsigned int threeQuartBit = 750/rate;

// Photo sensor pin
const int photoSensor = A0;
// Threshold between a transition.
const int threshold = 150;

void setup() {
	Serial.begin(9600);
	// Wait for Serial to connect to so output.
	while(!Serial);
	Serial.println("begin");

}

// Read the current sensor value
// Adjust so that a '1' on the sensor also means a '1' on the wire.
// Inversion can be caused by pull-up or pull-down circuit.
bool readSensor() {
	//Serial.println(analogRead(photoSensor));
	return !(analogRead(photoSensor) >= threshold);
}

// low-high is 1, high-low is 0.
bool readTransition() {
	// Read the current value
	bool curVal = readSensor();

	// And wait until it changes.
	bool nextVal = curVal;
	while(nextVal == curVal) {
		nextVal = readSensor();
	}
	return nextVal; //if true, it was low-high -> 1
}

// Read a byte from the sensor.
byte readByte() {
	byte readByte = 0x00;
	for(int i=0; i < 8; i++){
		bool val = readTransition();
                Serial.print(val);
		// Writes lowest-significance first.
		bitWrite(readByte, i, val);
		// Wait for 3/4 of the next bit transition,
		// so that we end up in the middle of the next flank
		delay(threeQuartBit);
	}
	Serial.println("");
	return readByte;
}

// Make sure that first bit sent is low -> high, for synchronisation
const byte sync[] = {0xCB, 0xFE, 0xBA, 0xBE};
// receive bytes number of bytes.
void receive(int bytes, byte* values) {
	// Sync must succeed first, check if 'sync' has been received.
	int syncCounter = 0;
	while(syncCounter < sizeof(sync)) {
		byte val = readByte();
		Serial.println(val); // Print the tapped bits
		if(val == sync[syncCounter]) {
    		Serial.println("success");
			syncCounter++;
		} else {
    		Serial.println("reset");
			// Reset counter, wrong byte received.
			syncCounter = 0;
		}
	}

	// When synced assume that everything is OK, and start receiving bytes.
	for(int i=0; i < bytes; i++) {
		values[i] = readByte();
	}
}

/**
 * The main receiver program loop.
 **/
void loop() {
	// Start receiving
    Serial.println("Receive");

	// Buffer the received bytes.
	byte val[25];
	receive(25, val);

	// Once received completely, print data as a String.
	Serial.println((char*) val);
}

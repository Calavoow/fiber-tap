const unsigned int rate = 50; // bit/s
const unsigned int halfBit = 500/rate; // delay in microseconds per half-bit, i.e. per transition.
// The wait time in microseconds until to the middle of the next flank.
// We wait until we are in the middle of a transmission flank.
const unsigned int threeQuartBit = 750/rate;

// Photo sensor pin
const int photoSensor = A0;
const int threshold = 75;

void setup() {
	Serial.begin(9600);
	// Wait for Serial to connect to so output.
	while(!Serial);
	Serial.println("begin");

}

// Read the current sensor value
bool readSensor() {
	return !(analogRead(photoSensor) >= threshold);
}

// low-high is 1, high-low is 0.
bool readTransition() {
	bool curVal = readSensor();
	bool nextVal = curVal;
	while(nextVal == curVal) {
		nextVal = readSensor();
	}
	return nextVal; //if true, it was low-high -> 1
}

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
	// Sync must succeed first.
	int syncCounter = 0;
	while(syncCounter < sizeof(sync)) {
		byte val = readByte();
                Serial.println(val);
		if(val == sync[syncCounter]) {
    Serial.println("success");
			syncCounter++;
		} else {
    Serial.println("reset");
			// Reset counter, wrong byte received.
			syncCounter = 0;
		}
	}

	// When synced receive bytes.
	for(int i=0; i < bytes; i++) {
		values[i] = readByte();
	}
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
    Serial.println("Receive");
	byte val[25];
	receive(25, val);
	Serial.println((char*) val);
}

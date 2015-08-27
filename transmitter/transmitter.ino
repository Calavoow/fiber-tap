const unsigned int rate = 50; // bit/s
const unsigned int halfBit = 500/rate; // delay in millisecond per half-bit, i.e. per transition.
// output pin
const int led = 4;

void setup() {
	pinMode(led, OUTPUT);

	Serial.begin(9600);
	// Wait for Serial to connect to output.
	while(!Serial) ;
	Serial.println("begin");
}

// Send a bit by writing to the given led.
void sendBit(bool bit) {
	if(bit) { // 1 => low -> high
		digitalWrite(led,  LOW);
		delay(halfBit);
		digitalWrite(led, HIGH);
		delay(halfBit);
	} else { // 0 => high -> low
		digitalWrite(led,  HIGH);
		delay(halfBit);
		digitalWrite(led, LOW);
		delay(halfBit);
	}
}

// Send the given byte, bit by bit.
void sendByte(byte value) {
	for(int i=0; i < 8; i++){
		sendBit(bitRead(value, i));
	}
}

/**
* Make sure that first bit sent is low -> high, for synchronisation,
* because we assume that the idle signal is 'low'.
* I.e. 0xCB = 0b11001011, so the least significant bit is '1',
* which is a low->high transition.
**/
const byte sync[] = {0xCB, 0xFE, 0xBA, 0xBE};

// Send the array of data in 'data', which is 'bytes' long.
void send(int bytes, byte* data) {
	// First send the sync bits
	for(int i = 0; i < sizeof(sync); i++) {
		sendByte(sync[i]);
	}

	// Then send the data
	for(int i = 0; i<bytes; i++){
		sendByte(data[i]);
	}
}

// An example string to send (must fit in Arduino memory)
const char password[26] = "correctHorseBatteryStaple";
void loop() {
	Serial.println("Send");
	// Keep sending the password.
	send(sizeof(password), (byte*) password);
}

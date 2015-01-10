const unsigned int rate = 100; // bit/s
const unsigned int halfBit = 500/rate; // delay in millisecond per half-bit, i.e. per transition.
// LED pin
const int led = 4;

void setup() {
	pinMode(led, OUTPUT);

	Serial.begin(9600);
	// Wait for Serial to connect to output.
	while(!Serial) ;
	Serial.println("begin");
}

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

void sendByte(byte value) {
	for(int i=0; i < 8; i++){
		sendBit(bitRead(value, i));
	}
}

const byte sync[] = {0xCA, 0xFE, 0xBA, 0xBE};
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

const char password[26] = "correctHorseBatteryStaple";
void loop() {
	Serial.println(halfBit);
	send(sizeof(password), (byte*) password);
}

// LED pin
int led = 2;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);
  delay(20);
  digitalWrite(led, LOW);
  delay(1000);
}

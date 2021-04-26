const int potPin = 34; // Potentiometer is connected to GPIO 34 (Analog ADC1_CH6)
float sums = 0;
int repeats = 64;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Start timer
  unsigned long time = millis();
  // Loop 64 times
  for (int x = 0; x<repeats; x++) {
    // Reading potentiometer value
    sums += analogRead(potPin);
    }
  Serial.print("Time (ms): ");
  Serial.print(millis()-time);
  Serial.print(", Average: ");
  Serial.print(sums/repeats, 0);
  Serial.println();

  sums = 0;
  delay(1000);
}

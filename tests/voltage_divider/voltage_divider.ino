void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Read the sensor value at A0
  int sensorValue = analogRead(A0);
  // Print on new lines sensor value
  Serial.println(sensorValue);
  delay(2000);
}

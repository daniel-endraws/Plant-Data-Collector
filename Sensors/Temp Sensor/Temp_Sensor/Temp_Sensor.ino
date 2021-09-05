int sensorPin = 0; // analog pin that the TMP36 is connected to
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(sensorPin);

  // convert reading to a voltage
  float voltage = reading * 3.3;
  voltage /= 1024.0;

  // print out voltage
  Serial.print(voltage);
  Serial.println(" V");

  // convert to temperature (10mV per degree with 500 mV offset)
  float tempC = (voltage - 0.5) * 100;

  Serial.print(tempC);
  Serial.println(" degrees C");

  // convert to Fahrenheit and print
  float temperatureF = (tempC * 9.0 / 5.0) + 32.0;
  Serial.print(temperatureF); Serial.println(" degrees F");

  delay(1000); // wait a second


}

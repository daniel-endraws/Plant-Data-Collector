// Uses a voltage divider to measure the resistance between probes
int moistPin = 0; // Analog pin the node is connected to
int moistVal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  moistVal = analogRead(moistPin);
  Serial.println(moistVal);
  int percent = 2.718282 * 2.718282 * (.008985 * moistVal + 0.207762); //calculate percent for probes about 1 - 1.5 inches apart with a 10k Ohm resistor
  Serial.print(percent);
  Serial.println("% Moisture ");

  delay(1000);

}

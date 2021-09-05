#include <DigitalIO.h>
#include "RF24.h"
#include <printf.h>

/* Analog input pin that connects to temperature sensor */
#define TEMP_PIN 0
/* Analog input pin connecting to photoresistor */
#define LIGHT_PIN 1
/* Analog input pin connecting to moisture sensor */
#define MOIST_PIN 2
/* Max number of bytes carried by an instruction */
#define INST_LENGTH 10

#define PIN_CE  2
#define PIN_CSN 3

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 2 & 3 */
RF24 radio(PIN_CE, PIN_CSN);
uint8_t addresses[][6] = {"0Node", "1Node", "2Node", "3Node"}; 
// Radio pipe addresses for the 2 nodes to communicate.

uint8_t instructions[][INST_LENGTH] = {"Get_Temp", "Moisture", "Light"};


void setup() {
  Serial.begin(115200);
  printf_begin();

  radio.begin();

  radio.setChannel(0x5F);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.enableDynamicPayloads();

  // Transmitter
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  Serial.println(radio.txDelay);
  radio.printPrettyDetails();
}

/* Gets the temp in celcius from sensor data */
float getTemp() {
  analogRead(TEMP_PIN); // Throw away first reading
  int reading = analogRead(TEMP_PIN);
  // convert reading to a voltage
  float voltage = reading * 3.3 / 1024.0;
  // convert to temperature (10mV per degree with 500 mV offset)
  return (voltage - 0.5) * 100;
}

/* Gets the moisture level from sensor data */
int getMoisture() {
  analogRead(MOIST_PIN); // Throw away first reading
  int reading = analogRead(MOIST_PIN);
  // Calculate percent for probes about 1 - 1.5 inches apart with a 10k Ohm resistor
  return 2.718282 * 2.718282 * (.008985 * reading + 0.207762);
}

/* Gets light intensity in lux from photoresistor */
float getLightIntensity() {
  analogRead(LIGHT_PIN); // Throw away first reading
  int reading = analogRead(LIGHT_PIN);
  // convert reading to a voltage
  float V_photo = reading * 3.3 / 1024.0;
  // voltage divider w/ 10k Ohm to get the resistance of the photoresistor
  float R_photo = (3.3 / V_photo - 1) * 10000;
  // Convert to lux with formula L = B*R_photo ^ m, with B and m discovered
  float lux = 4497354.981 * pow(R_photo, -1.0626);
  return lux;
}

/* prints the bytes in a readable format */
void print_bytes(uint8_t inst[]) {
  for (int i = 0; i < INST_LENGTH; i++) {
    Serial.print(inst[i]);
    Serial.print(" ");
  }
  Serial.println();
}

/* Validates the recieved instructions using memcmp */
boolean validate_instruction(byte recieved[], byte compare[]) {
  return memcmp(recieved, compare, INST_LENGTH) == 0;
}

void loop() {
  uint8_t instruction[INST_LENGTH] = {};

  radio.startListening(); // Start listening for instructions

  if (radio.available()) {
    radio.read(&instruction, INST_LENGTH);
    
    print_bytes(instruction);
    /*
    print_bytes(instructions[0]);
    print_bytes(instructions[1]);
    print_bytes(instructions[2]);
    */
    Serial.println("----------------------");
    radio.stopListening();
  }

  
  if (validate_instruction(instruction, instructions[0])) {
    // Get temp and send it back
    float temp = getTemp();
    
    Serial.println(temp);
    
    radio.openWritingPipe(addresses[1]);
    bool report = radio.write(&temp, sizeof(temp));

    if (!report) {
      Serial.println("Temp failed to send");
    }
  } else if (validate_instruction(instruction, instructions[1])) {
    // Get moisture level and send it back
    int moisture = getMoisture();
    
    Serial.println(moisture);
    
    radio.openWritingPipe(addresses[2]);
    bool report = radio.write(&moisture, sizeof(moisture));

    if (!report) {
      Serial.println("Moisture failed to send");
    }
  } else if (validate_instruction(instruction, instructions[2])) {
    // Get light intensity and send it back
    float lightIntensity = getLightIntensity();
    
    Serial.println(lightIntensity);
    
    radio.openWritingPipe(addresses[3]);
    bool report = radio.write(&lightIntensity, sizeof(lightIntensity));

    if (!report) {
      Serial.println("Light failed to send");
    }
  }

}

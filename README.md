# Overview
A [WebApp](TBD) running on a Raspberry Pi calls a [C++ Script](TBD) to collect data from the sensors near the plant. This script, quiered with what data to collect, communicates via radio to an Arduino Pro Mini. The Arduino then collects data, like temperature, light intensity and soil moisture levels, and sends it back to the Raspberry Pi.
The C++ script then updates an SQLite3 database with the timestamped data.

# Arduino
## SoftSPI
In order to use software defined SPI pins, since one of my Arduino Pro Mini's SPI pins were blown, follow the directions [here](https://nrf24.github.io/RF24/md_docs_arduino.html). 

Download the DIgitalIO library and uncomment the `#define SOFTSPI` line in **`RF24_config.h`**. I defined the pins as such:
|||
|--|--|
|`SOFT_SPI_MISO_PIN` | 9
|`SOFT_SPI_MOSI_PIN` |8
|`SOFT_SPI_SCK_PIN`  | 7

## Connections
I connected the Arduino Pro Mini to the nRF24L01+ like so:

| RF Module | Pin Name | Arduino Pin |
|--|--|--|
|1|GND|GND|
|2|VCC (+3.3V)|ACC|
|3|CE|2|
|4|CSN|3|
|5|SCK|7|
|6|MOSI|8|
|7|MISO|9|
|8|IRQ| |

## Sensors
View sensor documentation [here](Sensors/README.md).


# Raspberry Pi
## Set up
For various set up instructions on Linux with C++, check the [RF24 Documentation](https://nrf24.github.io/RF24/md_docs_linux_install.html) and [SQLite3](https://www.sqlite.org/cintro.html).

## Connections
I connected the raspberry pi to the nRF unit like so (pinout available [here](https://pinout.xyz/) or [here](https://learn.sparkfun.com/tutorials/raspberry-gpio/gpio-pinout)):

| RF Module | Pin Name | RPI Pin | RPI Pin Name |
|--|--|--|--|
|1|GND|25|rpi-gnd|
|2|VCC (+3.3V)|17|rpi-3v3|
|3|CE|15|rpi-gpio22|
|4|CSN|24|rpi-gpio8|
|5|SCK|23|rpi-sckl|
|6|MOSI|19|rpi-mosi|
|7|MISO|21|rpi-miso|
|8|IRQ| ||


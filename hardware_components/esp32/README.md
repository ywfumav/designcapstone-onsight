# ESP32
The ESP32 is compatible with Arduino Integrated Development Environment (IDE) and was developed there for our Onsight project.

## Installation
Download the Arduino Software IDE from the official [Arduino](https://www.arduino.cc/en/software) weblink.

The version used for Onsight is Arduino 1.8.8.

To use the ESP32 supported libraries in Arduino IDE, several packages have to be downloaded. This can be done in Arduino IDE under Tools > Manage Libraries.

 1. ESP32Servo by Kevin Harrington, John K. Bennett v0.11.0
 2. [ESP32 BLE](https://www.hackster.io/abdularbi17/how-to-install-esp32-board-in-arduino-ide-1cd571)
 3. Under Tools, the Board selected should read "ESP32 Dev Module".

With these installations, you should be able to upload code onto the ESP32.

## Implementation
The code provided in this implementation utilises BLE to communicate. For debugging purposes, you may find it useful to utilise a Bluetooth Connecting App on your phone to send/receive messages to the ESP32. 

We used BLE Scanner on Android which allows us to send Hexadecimal messages for compatibility with our code.

The code briefly does the following:

 1. Initialise the BLE on ESP32 for availability to connect from phone.
 2. Once connected, the ESP32 remains paired with the phone.
 3. The current code implementation supports one-way communication from the phone to ESP32. Based on hexadecimal values, the corresponding servo motors will rotate to raise and lower the pin.

 
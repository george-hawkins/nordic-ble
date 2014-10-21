#include <SPI.h>
#include <Streaming.h>
#include "BleStream.h"
#include "ble_echo.h"

const int8_t REQN_PIN = 10;
const int8_t RDYN_PIN = 2;
const int8_t RESET_PIN = 9;

BleUart ble_uart;
BleStream ble_stream(&ble_uart);

void setup() {
    Serial.begin(9600);
    while (!Serial); // Wait for serial to become available on Leonardo and similar boards.
    Serial << F("nRF8001 echo demo 0.3...") << endl;

    ble_uart.begin(REQN_PIN, RDYN_PIN, RESET_PIN); // Start BLE advertising.
}

void loop() {
    ble_uart.pollACI(); // Poll continuously for new events.

    int size = ble_stream.available();

    if (size > 0) {
        uint8_t buffer[size];

        ble_stream.readBytes(buffer, size);
        ble_stream.write(buffer, size);
    }
}

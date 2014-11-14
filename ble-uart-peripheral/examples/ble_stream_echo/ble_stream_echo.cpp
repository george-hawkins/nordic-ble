/**
 * Copyright 2014 George C. Hawkins
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <Streaming.h>

#include "BleStream.h"
#include "ble_stream_echo.h"

const int8_t REQN_PIN = 10;
const int8_t RDYN_PIN = 2; // Must be an interrupt pin.
const int8_t RESET_PIN = 9;

static BleCore ble_core;
static BleUart ble_uart(ble_core);
static BleStream ble_stream(ble_uart);

void setup() {
    Serial.begin(9600);
    while (!Serial); // Wait for serial to become available on Leonardo and similar boards.
    Serial << F("nRF8001 echo stream demo...") << endl;

    ble_uart.begin(REQN_PIN, RDYN_PIN, RESET_PIN); // Start BLE advertising.
}

void loop() {
    ble_core.handleAciEvent(); // Process the next incoming event.

    int size = ble_stream.available();

    if (size > 0) {
        uint8_t buffer[size];

        ble_stream.readBytes(buffer, size);
        ble_stream.write(buffer, size);
    }
}

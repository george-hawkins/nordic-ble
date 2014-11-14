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
#include <BleUart.h>

#include "ble_echo.h"

const int8_t REQN_PIN = 10;
const int8_t RDYN_PIN = 2; // Must be an interrupt pin.
const int8_t RESET_PIN = 9;

static BleCore ble_core;
static BleUart ble_uart(ble_core);

static class : public ReceivedObserver {
public:
    void received(uint8_t pipe, const uint8_t* buffer, size_t len) {
        ble_uart.write(buffer, len);
    }
} observer;

void setup() {
    Serial.begin(9600);
    while (!Serial); // Wait for serial to become available on Leonardo and similar boards.
    Serial << F("nRF8001 echo demo...") << endl;

    ble_core.setReceivedObserver(&observer);

    ble_uart.begin(REQN_PIN, RDYN_PIN, RESET_PIN); // Start BLE advertising.
}

void loop() {
    ble_core.handleAciEvent(); // Process the next incoming event.
}

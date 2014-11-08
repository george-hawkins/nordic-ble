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

BleStream::BleStream(BleUart& uart) : uart(uart), rx_buffer(rx_intern_buffer, RX_BUFFER_SIZE) {
    uart.getCore().setReceivedObserver(this);
}

void BleStream::received(uint8_t pipe, const uint8_t* buffer, size_t len) {
    for (int i = 0; i < len; i++) {
        if (!rx_buffer.add(buffer[i])) {
            // If the buffer is full just discard the data.
            Serial << F("Discarding 0x") << _HEX(buffer[i]);
        }
    }
}

size_t BleStream::write(uint8_t buffer) {
    return uart.write(&buffer, 1) ? 1 : 0;
}

size_t BleStream::write(const uint8_t* buffer, size_t len) {
    const size_t max_len = uart.getMaxWriteLen();
    uint8_t sent = 0;

    // Note: this logic will loop forever if the data can't be completely sent.
    while (len) { // Send in chunks
        size_t chunk_len = len < max_len ? len : max_len;

        if (uart.write((buffer + sent), chunk_len)) {
            len -= chunk_len;
            sent += chunk_len;
        }
    }

    return sent;
}

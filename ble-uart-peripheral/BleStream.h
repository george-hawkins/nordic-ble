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
#ifndef BLESTREAM_H_
#define BLESTREAM_H_

#include "RingBuffer.h"
#include "BleUart.h"

class BleStream: public Stream, private ReceivedObserver {
private:
    BleUart& uart;
    virtual void received(uint8_t pipe, const uint8_t* buffer, size_t len);

    const static size_t RX_BUFFER_SIZE = 64;
    uint8_t rx_intern_buffer[RX_BUFFER_SIZE];
    RingBuffer rx_buffer;

public:
    BleStream(BleUart& uart);
    virtual ~BleStream() { }

    // Print methods.
    virtual size_t write(const uint8_t* buffer, size_t len);
    virtual size_t write(uint8_t buffer);
    // Overriding Print:print(const __FlashStringHelper*) would also make sense as the default
    // implementation doesn't use the chunking write method - however it is not a virtual method.

    // Stream methods.
    virtual int available() { return rx_buffer.available(); }
    virtual int read() { return rx_buffer.remove(); }
    virtual int peek() { return rx_buffer.peek(); }
    virtual void flush() { /* NOP */ }
};

#endif /* BLESTREAM_H_ */

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
#include "RingBuffer.h"

int16_t RingBuffer::remove() {
    const int c = peek();

    if (c != -1) {
        tail = (tail + 1) % size;
    }

    return c;
}

bool RingBuffer::add(uint8_t c) {
    const uint16_t new_head = (head + 1) % size;

    if (new_head == tail) {
        return false;
    }

    buffer[head] = c;
    head = new_head;

    return true;
}

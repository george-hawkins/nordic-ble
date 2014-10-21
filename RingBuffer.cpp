#include "RingBuffer.h"

int16_t RingBuffer::remove() {
    int c = peek();

    if (c != -1) {
        tail = (tail + 1) % size;
    }

    return c;
}

bool RingBuffer::add(uint8_t c) {
    uint16_t new_head = (head + 1) % size;

    if (new_head == tail) {
        return false;
    }

    buffer[head] = c;
    head = new_head;

    return true;
}

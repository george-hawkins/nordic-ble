#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <stddef.h>
#include <stdint.h>

class RingBuffer {
private:
    uint8_t* const buffer;
    const size_t size;
    uint16_t head = 0;
    uint16_t tail = 0;

public:
    RingBuffer(uint8_t* buffer, size_t size) : buffer(buffer), size(size) { }

    size_t available() { return (size + head - tail) % size; }

    int16_t peek() { return (head == tail) ? -1 : buffer[tail]; }

    int16_t remove();

    bool add(uint8_t c);
};

#endif /* RINGBUFFER_H_ */

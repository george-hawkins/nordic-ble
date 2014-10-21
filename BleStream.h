#ifndef BLESTREAM_H_
#define BLESTREAM_H_

#include "BleUart.h"

class BleStream: public Stream, private ReceivedObserver {
private:
    BleUart* const uart;
    virtual void received(const uint8_t* buffer, size_t len);

    const static size_t RX_BUFFER_SIZE = 64;
    uint8_t rx_intern_buffer[RX_BUFFER_SIZE];
    RingBuffer rx_buffer;

public:
    BleStream(BleUart* uart);
    virtual ~BleStream() { }

    // Print methods.
    virtual size_t write(const uint8_t* buffer, size_t size);
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

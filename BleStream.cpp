#include "BleStream.h"

BleStream::BleStream(BleUart* uart) : uart(uart), rx_buffer(rx_intern_buffer, RX_BUFFER_SIZE) {
	uart->setObserver(this);
}

void BleStream::received(const uint8_t* buffer, size_t len) {
    for (int i = 0; i < len; i++) {
    	if (!rx_buffer.add(buffer[i])) {
            // If the buffer is full just discard the data.
			Serial.print("Discarding 0x");
			Serial.println(buffer[i], HEX);
		}
    }
}

size_t BleStream::write(uint8_t buffer) {
	return uart->write(&buffer, 1) ? 1 : 0;
}

size_t BleStream::write(const uint8_t* buffer, size_t len) {
    const size_t max_len = uart->getMaxWriteLen();
    uint8_t sent = 0;

    // Note: this logic will loop forever if the data can't be completely sent.
    while (len) { // Send in chunks
        size_t chunk_len = len < max_len ? len : max_len;

        if (uart->write((buffer + sent), chunk_len)) {
            len -= chunk_len;
            sent += chunk_len;
        }
    }

    return sent;
}

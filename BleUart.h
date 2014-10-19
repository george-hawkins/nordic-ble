#ifndef BLE_UART_H_
#define BLE_UART_H_

#include <Arduino.h>
#include <lib_aci.h>

#include "uart/services.h"
#include "RingBuffer.h"

struct ReceivedObserver {
	virtual ~ReceivedObserver() { }

	// Note: len will never be greater than ACI_PIPE_TX_DATA_MAX_LEN.
	virtual void received(const uint8_t* buffer, size_t len) = 0;
};

class BleUart {
public:
	BleUart(const char* deviceName = "", uint16_t adv_timeout = 0, uint16_t adv_interval = 80);
	virtual ~BleUart() { }

	void begin(int8_t reqn_pin, int8_t rdyn_pin, int8_t reset_pin);
	void pollACI();

	bool write(const uint8_t* buffer, size_t len);
	size_t getMaxWriteLen() { return ACI_PIPE_TX_DATA_MAX_LEN; }

	void setObserver(ReceivedObserver* receivedObserver);

private:
	void startAdvertising();

	const uint16_t adv_timeout;
	const uint16_t adv_interval;
	char device_name[PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE + 1];

	ReceivedObserver* receivedObserver = NULL;
	aci_state_t aci_state; // ACI state data.
	bool timing_change_done = false;
};

class BleStream : public Stream, private ReceivedObserver {
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

#endif /* BLE_UART_H_ */

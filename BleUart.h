#ifndef _BLE_UART_H_
#define _BLE_UART_H_

#include <Arduino.h>
#include "uart/services.h"

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

	bool write(uint8_t* buffer, uint8_t offset, size_t len);

	void setObserver(ReceivedObserver* receivedObserver);

private:
	void startAdvertising();

	const uint16_t adv_timeout;
	const uint16_t adv_interval;
	// Note: you can change the size of the GAP Device Name pipe but the fixed size of the advertising packet sets an upper size limit.
	char device_name[PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE + 1];
	ReceivedObserver* receivedObserver = NULL;
};

class BleStream : public Stream, private ReceivedObserver {
public:
	BleStream(BleUart* uart);
	virtual ~BleStream() { }

	// Print methods.
	virtual size_t write(const uint8_t* buffer, size_t size);
	virtual size_t write(uint8_t buffer);
	// Overriding Print:print(const __FlashStringHelper*) would also make sense as the default
	// implementation doesn't use the chunking write method - however it is not a virtual method.

	// Stream methods.
	virtual int available();
	virtual int read();
	virtual int peek();
	virtual void flush() { /* NOP */ }

private:
	BleUart* uart;
	virtual void received(const uint8_t* buffer, size_t len);
};

#endif /* _BLE_UART_H_ */

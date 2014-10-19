#ifndef _BLE_UART_H_
#define _BLE_UART_H_

#include <Arduino.h>
#include "uart/services.h"

class BleUart {
public:
	BleUart(const char* deviceName = "", uint16_t adv_timeout = 0, uint16_t adv_interval = 80);
	virtual ~BleUart() { }

	void begin(int8_t reqn_pin, int8_t rdyn_pin, int8_t reset_pin);
	void pollACI();

	bool write(uint8_t* buffer, uint8_t offset, size_t len);

protected:
	// Note: len will never be greater than ACI_PIPE_TX_DATA_MAX_LEN.
	virtual void receivedCallback(uint8_t* buffer, uint8_t len) = 0;

private:
	void startAdvertising();

	const uint16_t adv_timeout;
	const uint16_t adv_interval;
	char device_name[PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE + 1];
};

class BleStream : public BleUart, public Stream {
public:
	BleStream(
			const char* deviceName = "",
			uint16_t adv_timeout = 0,
			uint16_t adv_interval = 80) : BleUart(deviceName, adv_timeout, adv_interval) { }
	virtual ~BleStream() { }

	// Print methods.
	virtual size_t write(const uint8_t* buffer, size_t size);
	virtual size_t write(uint8_t buffer);
	// Overriding Print:print(const __FlashStringHelper*) would also make sense
	// as the default implementation doesn't use the chunking write method.

	// Stream methods.
	virtual int available();
	virtual int read();
	virtual int peek();
	virtual void flush() { /* NOP */ }

protected:
	virtual void receivedCallback(uint8_t* buffer, uint8_t len);
};

#endif /* _BLE_UART_H_ */

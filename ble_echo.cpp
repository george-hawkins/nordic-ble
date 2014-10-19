#include "BleUart.h"

#include "ble_echo.h"

#include <SPI.h>

const int8_t REQN_PIN = 10;
const int8_t RDYN_PIN = 2;
const int8_t RESET_PIN = 9;

//static class EchoUart : public BleUart {
//public:
//	EchoUart() { }
//
//	virtual void receivedCallback(uint8_t* buffer, uint8_t len) {
//		write(buffer, 0, len); // Just echo the same data back.
//	}
//} uart;

BleStream ble_stream;

void setup() {
	Serial.begin(9600);
	while (!Serial); // Wait for serial to become available on Leonardo and similar boards.
	Serial.println(F("nRF8001 echo demo..."));

	ble_stream.begin(REQN_PIN, RDYN_PIN, RESET_PIN);
//	uart.begin(REQN_PIN, RDYN_PIN, RESET_PIN); // Start BLE advertising.
}

void loop() {
//	uart.pollACI(); // Continually poll for new events.

	ble_stream.pollACI();

	int size = ble_stream.available();

	if (size > 0) {
		uint8_t buffer[size];

		ble_stream.readBytes(buffer, size);
		ble_stream.write(buffer, size);
	}
}

// An implementation of this method is required by the Nordic SDK.
void __ble_assert(const char *file, uint16_t line)
{
  Serial.print("ERROR ");
  Serial.print(file);
  Serial.print(": ");
  Serial.print(line);
  Serial.print("\n");
  abort();
}

#ifndef BLE_UART_H_
#define BLE_UART_H_

#include <Arduino.h>
#include <lib_aci.h>
#include <assert.h>

#include "uart/services.h"
#include "RingBuffer.h"

struct ReceivedObserver {
    virtual ~ReceivedObserver() { }

    // Note: len will never be greater than ACI_PIPE_TX_DATA_MAX_LEN.
    virtual void received(const uint8_t* buffer, size_t len) = 0;
};

class BleTiming {
private:
    bool change_done = false;

public:
    void reset() { change_done = false; }

    void changeTiming(aci_state_t* aci_state);
};

class BleDeviceName {
    const uint8_t pipe;
    const char* device_name;

public:
    // Usage: BleDeviceName(PIPE_GAP_DEVICE_NAME_SET, PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE, "DevName");
    BleDeviceName(uint8_t pipe, uint8_t max_size, const char* device_name) : pipe(pipe), device_name(device_name) {
        assert(strlen(device_name) <= max_size);
    }

    void changeName(aci_state_t* aci_state);
};

class BleUart {
private:
    const uint16_t adv_timeout;
    const uint16_t adv_interval;
    BleDeviceName* device_name = NULL;

    aci_state_t aci_state; // ACI state data.
    BleTiming timing;
    ReceivedObserver* received_observer = NULL;

    void startAdvertising();

public:
    // adv_timeout - maximum advertising time in seconds (0 means infinite).
    // adv_interval - advertising interval (in multiples of 0.625ms).
    BleUart(uint16_t adv_timeout = 0, uint16_t adv_interval = 80) : adv_timeout(adv_timeout), adv_interval(adv_interval) { }

    virtual ~BleUart() { }

    void begin(int8_t reqn_pin, int8_t rdyn_pin, int8_t reset_pin);
    void pollAci();

    bool write(const uint8_t* buffer, size_t len);
    size_t getMaxWriteLen() { return ACI_PIPE_TX_DATA_MAX_LEN; }

    void setObserver(ReceivedObserver* received_observer) { this->received_observer = received_observer; }
    void setDeviceName(BleDeviceName* device_name) { this->device_name = device_name; }
};

#endif /* BLE_UART_H_ */

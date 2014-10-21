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
private:
    const uint16_t adv_timeout;
    const uint16_t adv_interval;
    char device_name[PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE + 1];

    aci_state_t aci_state; // ACI state data.
    bool timing_change_done = false;
    ReceivedObserver* received_observer = NULL;

    void startAdvertising();

public:
    BleUart(const char* deviceName = "", uint16_t adv_timeout = 0, uint16_t adv_interval = 80);
    virtual ~BleUart() { }

    void begin(int8_t reqn_pin, int8_t rdyn_pin, int8_t reset_pin);
    void pollACI();

    bool write(const uint8_t* buffer, size_t len);
    size_t getMaxWriteLen() { return ACI_PIPE_TX_DATA_MAX_LEN; }

    void setObserver(ReceivedObserver* received_observer) { this->received_observer = received_observer; }
};

#endif /* BLE_UART_H_ */

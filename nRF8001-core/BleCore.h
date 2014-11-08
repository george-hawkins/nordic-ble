/**
 * Copyright 2014 George C. Hawkins
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef BLE_CORE_H_
#define BLE_CORE_H_

#include <lib_aci.h>
#include <assert.h>

struct ReceivedObserver {
    virtual ~ReceivedObserver() { }

    // Note: len will never be greater than ACI_PIPE_TX_DATA_MAX_LEN.
    virtual void received(uint8_t pipe, const uint8_t* buffer, size_t len) = 0;
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

class BleCore {
private:
    // The BLE section of Apple's Bluetooth Design Guidelines recommends 20ms, followed by a list of longer intervals - 152.5ms, 211.25ms etc.
    // A longer interval consumes far less power (Nokia show 96% less for 1s vs 20ms). See section 13.2 of the nRF8001 product specification.
    static const uint16_t DEFAULT_ADV_INTERVAL = 244; // 244 * 0.625ms = 152.5ms.

    const uint16_t adv_timeout;
    const uint16_t adv_interval;

    aci_state_t aci_state;
    BleTiming timing;
    ReceivedObserver* received_observer = NULL;
    BleDeviceName* device_name = NULL;

    void startAdvertising();

public:
    // adv_timeout - maximum advertising time in seconds (0 means infinite).
    // adv_interval - advertising interval (in multiples of 0.625ms).
    BleCore(uint16_t adv_timeout = 0, uint16_t adv_interval = DEFAULT_ADV_INTERVAL) : adv_timeout(adv_timeout), adv_interval(adv_interval) { }

    virtual ~BleCore() { }

    void begin(aci_setup_info_t& info, int8_t reqn_pin, int8_t rdyn_pin, int8_t reset_pin);
    void pollAci();

    bool write(uint8_t pipe, size_t max_len, const uint8_t* buffer, size_t len);

    void setReceivedObserver(ReceivedObserver* received_observer) { this->received_observer = received_observer; }
    void setDeviceName(BleDeviceName* device_name) { this->device_name = device_name; }
};

#endif /* BLE_CORE_H_ */

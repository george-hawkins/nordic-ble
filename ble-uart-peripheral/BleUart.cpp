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
#include "BleUart.h"

#include "uart/services.h"

// Store the nRF8001 setup in flash (this is required by the SDK and saves RAM).
const hal_aci_data_t setup_msgs[] PROGMEM = SETUP_MESSAGES_CONTENT;

aci_setup_info_t BleUart::setup_info = BleProfile::createSetupInfo(
#ifdef NUMBER_OF_PIPES
        (services_pipe_type_mapping_t[])SERVICES_PIPE_TYPE_MAPPING_CONTENT, NUMBER_OF_PIPES,
#endif
        setup_msgs, NB_SETUP_MESSAGES);

bool BleUart::write(const uint8_t* buffer, size_t len) {
    return getCore().write(PIPE_UART_OVER_BTLE_UART_TX_TX, getMaxWriteLen(), buffer, len);
}

size_t BleUart::getMaxWriteLen() { return PIPE_UART_OVER_BTLE_UART_TX_TX_MAX_SIZE; }

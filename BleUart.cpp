#include <lib_aci.h>
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


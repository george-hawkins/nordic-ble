#include <stddef.h>

#include "BleProfile.h"

aci_setup_info_t BleProfile::createSetupInfo(const hal_aci_data_t* setup_msgs, uint8_t num_setup_msgs) {
    return createSetupInfo(NULL, 0, setup_msgs, num_setup_msgs);
}

aci_setup_info_t BleProfile::createSetupInfo(
        services_pipe_type_mapping_t* services_pipe_type_mapping, uint8_t number_of_pipes,
        const hal_aci_data_t* setup_msgs, uint8_t num_setup_msgs) {
    return {
        services_pipe_type_mapping,
        number_of_pipes,
        const_cast<hal_aci_data_t*>(setup_msgs),
        num_setup_msgs
    };
}

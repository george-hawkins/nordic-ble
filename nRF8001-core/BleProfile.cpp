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

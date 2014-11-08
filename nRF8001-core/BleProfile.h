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
#ifndef BLEPROFILE_H_
#define BLEPROFILE_H_

#include "BleCore.h"

class BleProfile {
private:
    BleCore& core;
    aci_setup_info_t& setup_info;

protected:
    static aci_setup_info_t createSetupInfo(const hal_aci_data_t* setup_msgs, uint8_t num_setup_msgs);
    static aci_setup_info_t createSetupInfo(
            services_pipe_type_mapping_t* services_pipe_type_mapping, uint8_t number_of_pipes,
            const hal_aci_data_t* setup_msgs, uint8_t num_setup_msgs);

public:
    BleProfile(BleCore& core, aci_setup_info_t& setup_info) : core(core), setup_info(setup_info) { }

    BleCore& getCore() { return core; }

    void begin(int8_t reqn_pin, int8_t rdyn_pin, int8_t reset_pin) {
        core.begin(setup_info, reqn_pin, rdyn_pin, reset_pin);
    }
};

#endif /* BLEPROFILE_H_ */

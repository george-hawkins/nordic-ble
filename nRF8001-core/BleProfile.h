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

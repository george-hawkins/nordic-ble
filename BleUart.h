#ifndef BLEUART_H_
#define BLEUART_H_

#include "BleProfile.h"

class BleUart: public BleProfile {
private:
    static aci_setup_info_t setup_info;

public:
    BleUart(BleCore& core) : BleProfile(core, setup_info) { }

    bool write(const uint8_t* buffer, size_t len);
    size_t getMaxWriteLen();
};

#endif /* BLEUART_H_ */

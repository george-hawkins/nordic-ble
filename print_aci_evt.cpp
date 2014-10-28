#include <Streaming.h>
#include "print_aci_evt.h"

typedef struct {
    aci_evt_opcode_t key;
    const char* value;
} aci_evt_opcode_name_map_entry_t;

const char ACI_EVT_INVALID_STR[] PROGMEM = "INVALID";
const char ACI_EVT_DEVICE_STARTED_STR[] PROGMEM = "DEVICE_STARTED";
const char ACI_EVT_ECHO_STR[] PROGMEM = "ECHO";
const char ACI_EVT_HW_ERROR_STR[] PROGMEM = "HW_ERROR";
const char ACI_EVT_CMD_RSP_STR[] PROGMEM = "CMD_RSP";
const char ACI_EVT_CONNECTED_STR[] PROGMEM = "CONNECTED";
const char ACI_EVT_DISCONNECTED_STR[] PROGMEM = "DISCONNECTED (disconnected or advertising timed out)";
const char ACI_EVT_BOND_STATUS_STR[] PROGMEM = "BOND_STATUS";
const char ACI_EVT_PIPE_STATUS_STR[] PROGMEM = "PIPE_STATUS";
const char ACI_EVT_TIMING_STR[] PROGMEM = "TIMING";
const char ACI_EVT_DATA_CREDIT_STR[] PROGMEM = "DATA_CREDIT";
const char ACI_EVT_DATA_ACK_STR[] PROGMEM = "DATA_ACK";
const char ACI_EVT_DATA_RECEIVED_STR[] PROGMEM = "DATA_RECEIVED";
const char ACI_EVT_PIPE_ERROR_STR[] PROGMEM = "PIPE_ERROR";
const char ACI_EVT_DISPLAY_PASSKEY_STR[] PROGMEM = "DISPLAY_PASSKEY";
const char ACI_EVT_KEY_REQUEST_STR[] PROGMEM = "KEY_REQUEST";

const aci_evt_opcode_name_map_entry_t aci_evt_opcode_name_map[] PROGMEM = {
        { ACI_EVT_INVALID, ACI_EVT_INVALID_STR },
        { ACI_EVT_DEVICE_STARTED, ACI_EVT_DEVICE_STARTED_STR },
        { ACI_EVT_ECHO, ACI_EVT_ECHO_STR },
        { ACI_EVT_HW_ERROR, ACI_EVT_HW_ERROR_STR },
        { ACI_EVT_CMD_RSP, ACI_EVT_CMD_RSP_STR },
        { ACI_EVT_CONNECTED, ACI_EVT_CONNECTED_STR },
        { ACI_EVT_DISCONNECTED, ACI_EVT_DISCONNECTED_STR },
        { ACI_EVT_BOND_STATUS, ACI_EVT_BOND_STATUS_STR },
        { ACI_EVT_PIPE_STATUS, ACI_EVT_PIPE_STATUS_STR },
        { ACI_EVT_TIMING, ACI_EVT_TIMING_STR },
        { ACI_EVT_DATA_CREDIT, ACI_EVT_DATA_CREDIT_STR },
        { ACI_EVT_DATA_ACK, ACI_EVT_DATA_ACK_STR },
        { ACI_EVT_DATA_RECEIVED, ACI_EVT_DATA_RECEIVED_STR },
        { ACI_EVT_PIPE_ERROR, ACI_EVT_PIPE_ERROR_STR },
        { ACI_EVT_DISPLAY_PASSKEY, ACI_EVT_DISPLAY_PASSKEY_STR },
        { ACI_EVT_KEY_REQUEST, ACI_EVT_KEY_REQUEST_STR }
};

const size_t aci_evt_opcode_name_map_len = sizeof(aci_evt_opcode_name_map) / sizeof(aci_evt_opcode_name_map_entry_t);

#define F_ADDR(ptr) (reinterpret_cast<const __FlashStringHelper*>(ptr))

void printAciEvtOpcode(aci_evt_opcode_t event) {
    aci_evt_opcode_name_map_entry_t entry;

    for (int i = 0; i < aci_evt_opcode_name_map_len; i++) {
        memcpy_P(&entry, (aci_evt_opcode_name_map + i), sizeof(entry));
        if (entry.key == event) {
            Serial << F("ACI_EVT_") << F_ADDR(entry.value) << endl;
            return;
        }
    }

    Serial << F("Unknown aci_evt_opcode_t 0x") << _HEX(event) << endl;
}

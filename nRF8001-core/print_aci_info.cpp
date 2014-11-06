#include <Streaming.h>
#include "print_aci_info.h"

// From Bluetooth Core specification v4.0, Volume 2, Part D "Error Codes".
const uint8_t REMOTE_USER_TERMINATED_CONNECTION = 0x13;

struct Hex {
    long value;
    Hex(long value) : value(value) { };
};

// Print hex values with a leading 0x.
static Print& operator<<(Print& print, const Hex& hex) {
    print << F("0x") << _HEX(hex.value);
    return print;
}

static bool isAdvertising(aci_evt_params_cmd_rsp_t& rsp) {
    if (rsp.cmd_status != ACI_STATUS_SUCCESS) {
        return false;
    }

    aci_cmd_opcode_t code = rsp.cmd_opcode;

    return code == ACI_CMD_CONNECT || code == ACI_CMD_BOND || code == ACI_CMD_BROADCAST;
}

void printAciInfo(aci_state_t& aci_state, aci_evt_t& aci_evt) {
    switch (aci_evt.evt_opcode) {
    case ACI_EVT_CMD_RSP: {
        aci_evt_params_cmd_rsp_t& rsp = aci_evt.params.cmd_rsp;

        Serial << F("CMD_RSP (");
        if (isAdvertising(rsp)) {
            Serial << F("advertising started");
        } else {
            Serial << F("status=");
            if (rsp.cmd_status == ACI_STATUS_SUCCESS) {
                Serial << F("success");
            } else {
                Serial << Hex(rsp.cmd_status);
            }
        }
        Serial << F(", opcode=") << Hex(rsp.cmd_opcode) << F(")") << endl;
        break;
    }
    case ACI_EVT_PIPE_ERROR:
        // See the appendix in the nRF8001 Product Specification for details on the error codes.
        Serial << F("PIPE_ERROR (pipe=") << aci_evt.params.pipe_error.pipe_number <<
            F(", error_code=") << Hex(aci_evt.params.pipe_error.error_code) << endl;
        break;

    case ACI_EVT_HW_ERROR: {
        // Report the source file and line number where the error occurred in the nRF8001 Bluetooth stack.
        Serial << F("HW_ERROR (");
        Serial.write(aci_evt.params.hw_error.file_name, (aci_evt.len - 3));
        Serial << F(":") << aci_evt.params.hw_error.line_num << F(")") << endl;
        break;
    }
    case ACI_EVT_DEVICE_STARTED: {
        aci_device_operation_mode_t mode = aci_evt.params.device_started.device_mode;

        Serial << F("DEVICE_STARTED (mode=");
        if (mode == ACI_DEVICE_SETUP) {
            Serial << F("setup");
        } else if (mode == ACI_DEVICE_STANDBY) {
            Serial << F("standby");
        } else {
            Serial << Hex(mode);
        }
        Serial << F(")") << endl;
        break;
    }
    case ACI_EVT_TIMING:
        Serial <<
            F("TIMING (connection_interval=") << lib_aci_get_cx_interval_ms(&aci_state) <<
            F("ms, slave_latency=") << aci_state.slave_latency <<
            F(", supervision_timeout=") << aci_state.supervision_timeout * 10 << F("ms)") << endl;
        break;

    case ACI_EVT_DISCONNECTED: {
        aci_evt_params_disconnected_t& status = aci_evt.params.disconnected;

        Serial << F("DISCONNECTED (");
        if (status.aci_status == ACI_STATUS_EXTENDED) {
            if (status.btle_status == REMOTE_USER_TERMINATED_CONNECTION) {
                Serial << F("remote user terminated connection");
            } else {
                Serial << F("btle_status=") << Hex(status.btle_status);
            }
        } else {
            if (status.aci_status == ACI_STATUS_ERROR_ADVT_TIMEOUT) {
                Serial << F("advertising timeout");
            } else {
                Serial << F("aci_status=") << Hex(status.btle_status);
            }
        }
        Serial << F(")") << endl;
        break;
    }
    case ACI_EVT_DATA_RECEIVED: {
        aci_rx_data_t& data = aci_evt.params.data_received.rx_data;
        uint8_t len = aci_evt.len - 2;

        Serial << F("DATA_RECEIVED (pipe=") << data.pipe_number << F(", data=[");
        for (int i = 0; i < len; i++) {
            if (i != 0) {
                Serial << ", ";
            }
            Serial << Hex(data.aci_data[i]);
        }
        Serial << F("])") << endl;
        break;
    }
    case ACI_EVT_INVALID:
        Serial << F("INVALID") << endl;
        break;

    case ACI_EVT_ECHO:
        Serial << F("ECHO") << endl;
        break;

    case ACI_EVT_CONNECTED:
        Serial << F("CONNECTED") << endl;
        break;

    case ACI_EVT_BOND_STATUS:
        Serial << F("BOND_STATUS") << endl;
        break;

    case ACI_EVT_PIPE_STATUS:
        Serial << F("PIPE_STATUS") << endl;
        break;
    case ACI_EVT_DATA_CREDIT:
        Serial << F("DATA_CREDIT") << endl;
        break;

    case ACI_EVT_DATA_ACK:
        Serial << F("DATA_ACK") << endl;
        break;

    case ACI_EVT_DISPLAY_PASSKEY:
        Serial << F("DISPLAY_PASSKEY") << endl;
        break;

    case ACI_EVT_KEY_REQUEST:
        Serial << F("KEY_REQUEST") << endl;
        break;

    default:
        Serial << F("Unknown event with opcode ") << Hex(aci_evt.evt_opcode) << endl;
        break;
    }
}

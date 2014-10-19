#include "Arduino.h"
#include "debug.h"

typedef struct {
    aci_evt_opcode_t key;
    const char* value;
} aci_evt_opcode_name_map_entry_t;

static const char ACI_EVT_INVALID_STR[] PROGMEM = "ACI_EVT_INVALID";
static const char ACI_EVT_DEVICE_STARTED_STR[] PROGMEM = "ACI_EVT_DEVICE_STARTED";
static const char ACI_EVT_ECHO_STR[] PROGMEM = "ACI_EVT_ECHO";
static const char ACI_EVT_HW_ERROR_STR[] PROGMEM = "ACI_EVT_HW_ERROR";
static const char ACI_EVT_CMD_RSP_STR[] PROGMEM = "ACI_EVT_CMD_RSP";
static const char ACI_EVT_CONNECTED_STR[] PROGMEM = "ACI_EVT_CONNECTED";
static const char ACI_EVT_DISCONNECTED_STR[] PROGMEM = "ACI_EVT_DISCONNECTED (disconnected or advertising timed out)";
static const char ACI_EVT_BOND_STATUS_STR[] PROGMEM = "ACI_EVT_BOND_STATUS";
static const char ACI_EVT_PIPE_STATUS_STR[] PROGMEM = "ACI_EVT_PIPE_STATUS";
static const char ACI_EVT_TIMING_STR[] PROGMEM = "ACI_EVT_TIMING";
static const char ACI_EVT_DATA_CREDIT_STR[] PROGMEM = "ACI_EVT_DATA_CREDIT";
static const char ACI_EVT_DATA_ACK_STR[] PROGMEM = "ACI_EVT_DATA_ACK";
static const char ACI_EVT_DATA_RECEIVED_STR[] PROGMEM = "ACI_EVT_DATA_RECEIVED";
static const char ACI_EVT_PIPE_ERROR_STR[] PROGMEM = "ACI_EVT_PIPE_ERROR";
static const char ACI_EVT_DISPLAY_PASSKEY_STR[] PROGMEM = "ACI_EVT_DISPLAY_PASSKEY";
static const char ACI_EVT_KEY_REQUEST_STR[] PROGMEM = "ACI_EVT_KEY_REQUEST";

static const aci_evt_opcode_name_map_entry_t aci_evt_opcode_name_map[] PROGMEM = {
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

static const size_t aci_evt_opcode_name_map_len = sizeof(aci_evt_opcode_name_map) / sizeof(aci_evt_opcode_name_map_entry_t);

static void printFlashString(const char s[]) {
	unsigned char c;

	while ((c = pgm_read_byte(s++)) != 0) {
		Serial.print((char)c);
	}

	Serial.println();
}

void printAciEvtOpcode(aci_evt_opcode_t event) {
	aci_evt_opcode_name_map_entry_t entry;

	for (int i = 0; i < aci_evt_opcode_name_map_len; i++) {
		memcpy_P(&entry, (aci_evt_opcode_name_map + i), sizeof(entry));
		if (entry.key == event) {
			printFlashString(entry.value);
			return;
		}
	}

	Serial.print("Unknown aci_evt_opcode_t ");
	Serial.println((char)event, HEX);
}

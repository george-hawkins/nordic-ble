#include <SPI.h>
#include <lib_aci.h>
#include <aci_setup.h>
#include <assert.h>
#include "debug.h"

#include "BleUart.h"

// Get the service pipe data created in nRFGo Studio.
#ifdef SERVICES_PIPE_TYPE_MAPPING_CONTENT
	static services_pipe_type_mapping_t services_pipe_type_mapping[] = SERVICES_PIPE_TYPE_MAPPING_CONTENT;
#else
	static services_pipe_type_mapping_t* services_pipe_type_mapping = NULL;
#define NUMBER_OF_PIPES 0
#endif

const bool BLE_DEBUG = true;

// The SDK requires that the nRF8001 setup to be in flash (and this saves on RAM).
const hal_aci_data_t setup_msgs[NB_SETUP_MESSAGES] PROGMEM = SETUP_MESSAGES_CONTENT;

static aci_state_t aci_state; // ACI state data.
static hal_aci_evt_t aci_data; // Command buffer.
static bool timing_change_done = false;

// TODO: would this make a good class - with buffer size as an argument. Cf size diff class and non-class version.
// This is the UART RX buffer, which we manage internally when data is available.
const size_t RX_BUFFER_SIZE = 64;
uint8_t rx_buffer[RX_BUFFER_SIZE];
uint16_t rx_buffer_head = 0;
uint16_t rx_buffer_tail = 0;

// TODO:
// Make this a class that doesn't inherit from Stream.
// Make rx_callback a mandatory ctor argument.
// defaultRX, the ring buffer and the stream stuff etc. can be moved into a separate class that does subclass Stream
// and uses this class internally and passes defaultRX as rx_callback.
// Use this.xyz = xyz rather than alpha_beta = alphaBeta scheme.
// Rename badly named variables like rx_event.
// Look at how you pulled in printf previously.
// HAL_IO_RADIO_RESET etc. have to be C style data, but all the other stuff should be class data.
// The ring buffer variables don't need adafruit_ble_ in them - there's nothing Adafruit BLE breakout specific to them.
// Are all those includes necessary?
// Get rid of those back-to-front conditions, e.g. ACI_STATUS_TRANSACTION_COMPLETE != do_aci_setup(&aci_state).
// Look at unused #defines in service.h
// Use debug.cpp or get rid of it - get rid of callbackEcho.cpp.
// Put into git repo.

// Constructor for the UART service.
BleUart::BleUart(
		const char* deviceName,
		uint16_t adv_timeout,
		uint16_t adv_interval) : adv_timeout(adv_timeout), adv_interval(adv_interval) {
    assert(strlen(deviceName) < sizeof(device_name));

    memset(device_name, 0x00, sizeof(device_name));
    memcpy(device_name, deviceName, strlen(deviceName));
}

//    Configures the nRF8001 and starts advertising the UART Service
//
//    advTimeout - the advertising timeout in seconds (0 = infinite advertising).
//    advInterval - the delay between advertising packets in 0.625ms units.
//
void BleUart::begin(int8_t reqn_pin, int8_t rdyn_pin, int8_t reset_pin) {
	// Pull in the data generated by nRFGo Studio (services.h).
    aci_state.aci_setup_info.services_pipe_type_mapping = services_pipe_type_mapping;
    aci_state.aci_setup_info.number_of_pipes = NUMBER_OF_PIPES;
    aci_state.aci_setup_info.setup_msgs = const_cast<hal_aci_data_t*>(setup_msgs);
    aci_state.aci_setup_info.num_setup_msgs = NB_SETUP_MESSAGES;

    // Configure the MCU to nRF8001 pin connections.
    aci_state.aci_pins.board_name = BOARD_DEFAULT;
    aci_state.aci_pins.reqn_pin   = reqn_pin;
    aci_state.aci_pins.rdyn_pin   = rdyn_pin;
    aci_state.aci_pins.mosi_pin   = MOSI;
    aci_state.aci_pins.miso_pin   = MISO;
    aci_state.aci_pins.sck_pin    = SCK;

    aci_state.aci_pins.spi_clock_divider = SPI_CLOCK_DIV8; // 2MHz SPI speed.

    aci_state.aci_pins.reset_pin              = reset_pin;
    aci_state.aci_pins.active_pin             = UNUSED;
    aci_state.aci_pins.optional_chip_sel_pin  = UNUSED;

    aci_state.aci_pins.interface_is_interrupt = false;
    aci_state.aci_pins.interrupt_number       = 1;

    // Configure the host pins and SPI - the nRF8001 itself is only setup later by do_aci_setup(...).
    lib_aci_init(&aci_state, true);
}

// Handles low level ACI events.
void BleUart::pollACI() {
    // We enter the if statement only when there is a ACI event available to be processed.
    if (lib_aci_event_get(&aci_state, &aci_data)) {
        aci_evt_t& aci_evt = aci_data.evt;

        printAciEvtOpcode(aci_evt.evt_opcode);

        // Note: the term credit seen at various points in this method is related to flow control.
        // For more information see aci.h and the "Flow control" section of the nRF8001 Product Specification.

        switch (aci_evt.evt_opcode) {
        // As soon as you reset the nRF8001 you will get an ACI device started event.
        case ACI_EVT_DEVICE_STARTED:
            aci_state.data_credit_total = aci_evt.params.device_started.credit_available;
            switch (aci_evt.params.device_started.device_mode) {
            case ACI_DEVICE_SETUP:
                // Device is in setup mode.
                if (ACI_STATUS_TRANSACTION_COMPLETE != do_aci_setup(&aci_state)) {
                    if (BLE_DEBUG) {
                        Serial.println(F("Error in ACI Setup"));
                    }
                }
                break;

            case ACI_DEVICE_STANDBY:
                if (device_name[0] != 0x00) {
                    // Update the device name.
                    lib_aci_set_local_data(&aci_state, PIPE_GAP_DEVICE_NAME_SET, (uint8_t*)&device_name, strlen(device_name));
                }
                startAdvertising();
            }
            break;

        case ACI_EVT_CMD_RSP:
            // If an ACI command response event comes with an error then stop.
            if (ACI_STATUS_SUCCESS != aci_evt.params.cmd_rsp.cmd_status) {
                // ACI ReadDynamicData and ACI WriteDynamicData will have status codes of
                // TRANSACTION_CONTINUE and TRANSACTION_COMPLETE
                // all other ACI commands will have status code of ACI_STATUS_SUCCESS for a successful command.
                if (BLE_DEBUG) {
                    Serial.print(F("ACI command "));
                    Serial.println(aci_evt.params.cmd_rsp.cmd_opcode, HEX);
                    Serial.println(F(" failed - halting MCU."));
                }
                abort();
            }
            if (ACI_CMD_GET_DEVICE_VERSION == aci_evt.params.cmd_rsp.cmd_opcode) {
                // Write the device version information to the pipe corresponding to the Hardware Revision String characteristic.
                lib_aci_set_local_data(&aci_state,
                        PIPE_DEVICE_INFORMATION_HARDWARE_REVISION_STRING_SET,
                        (uint8_t*)&(aci_evt.params.cmd_rsp.params.get_device_version),
                        sizeof(aci_evt_cmd_rsp_params_get_device_version_t));
            }
            break;

        case ACI_EVT_CONNECTED:
            aci_state.data_credit_available = aci_state.data_credit_total;

            // Request the device version - response comes back with cmd_opcode ACI_CMD_GET_DEVICE_VERSION.
            lib_aci_device_version();
            break;

        case ACI_EVT_PIPE_STATUS:
            if (lib_aci_is_pipe_available(&aci_state, PIPE_UART_OVER_BTLE_UART_TX_TX) && !timing_change_done) {
                // Change the timing on the link as specified in the nRFgo studio -> nRF8001 conf. -> GAP.
                // Used to increase or decrease bandwidth
                lib_aci_change_timing_GAP_PPCP();

                timing_change_done = true;
            }
            break;

        case ACI_EVT_TIMING:
            // Link connection interval changed.
            break;

        case ACI_EVT_DISCONNECTED:
        	startAdvertising();
            break;

        case ACI_EVT_DATA_RECEIVED:
        	if (receivedObserver != NULL) {
        		receivedObserver->received(aci_evt.params.data_received.rx_data.aci_data, aci_evt.len - 2);
        	}
            break;

        case ACI_EVT_DATA_CREDIT:
            aci_state.data_credit_available = aci_state.data_credit_available + aci_evt.params.data_credit.credit;
            break;

        case ACI_EVT_PIPE_ERROR:
            // See the appendix in the nRF8001 Product Specification for details on the error codes.
            if (BLE_DEBUG) {
                Serial.print(F("ACI Evt Pipe Error: Pipe #:"));
                Serial.print(aci_evt.params.pipe_error.pipe_number, DEC);
                Serial.print(F("  Pipe Error Code: 0x"));
                Serial.println(aci_evt.params.pipe_error.error_code, HEX);
            }

            // Increment the credit available as the data packet was not sent
            aci_state.data_credit_available++;
            break;

        default:
            Serial.print(F("Unhandled event with opcode 0x"));
            Serial.println(aci_evt.evt_opcode, HEX);
            break;
        }
    } else {
        // There's no event in the ACI event queue. If there's also no event in the ACI command
    	// queue then the Arduino can go to sleep and wakeup from sleep from the RDYN line.
    }
}

void BleUart::startAdvertising() {
    // Start advertising ... first value is advertising time in seconds,
    // the second value is the advertising interval in 0.625ms units.
    lib_aci_connect(adv_timeout, adv_interval);
}

// len must be less than or equal to ACI_PIPE_TX_DATA_MAX_LEN.
bool BleUart::write(uint8_t* buffer, uint8_t offset, size_t len) {
	assert(len <= ACI_PIPE_TX_DATA_MAX_LEN);

	bool available = lib_aci_is_pipe_available(&aci_state, PIPE_UART_OVER_BTLE_UART_TX_TX);

	if (available) {
        lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (buffer + offset), len);
        aci_state.data_credit_available--;
        delay(35); // Required delay between sends.
	} else {
		pollACI();
	}

	return available;
}

void BleUart::setObserver(ReceivedObserver* receivedObserver) {
	this->receivedObserver = receivedObserver;
}

// -------------------------------------------------------------------------------------------------------
// Stream stuff
// -------------------------------------------------------------------------------------------------------

BleStream::BleStream(BleUart* uart) : uart(uart) {
	uart->setObserver(this);
}

int BleStream::available() {
    return (uint16_t) (RX_BUFFER_SIZE + rx_buffer_head - rx_buffer_tail) % RX_BUFFER_SIZE;
}

int BleStream::peek() {
	return (rx_buffer_head == rx_buffer_tail) ? -1 : rx_buffer[rx_buffer_tail];
}

int BleStream::read() {
	int c = peek();

	if (c != -1) {
		rx_buffer_tail = (rx_buffer_tail + 1) % RX_BUFFER_SIZE;
	}

	return c;
}

size_t BleStream::write(const uint8_t* buffer, size_t len) {
    uint8_t bytesThisPass, sent = 0;

    if (BLE_DEBUG) {
		Serial.print(F("\tWriting out to BTLE:"));
		for (uint8_t i = 0; i < len; i++) {
			Serial.print(F(" 0x"));
			Serial.print(buffer[i], HEX);
		}
		Serial.println();
    }

    // Note: this logic will loop forever if the data can't be completely sent.
    while (len) { // Parcelize into chunks
// TODO: make ACI_PIPE_TX_DATA_MAX_LEN a getable value on separate BLE logic class.
        bytesThisPass = len < ACI_PIPE_TX_DATA_MAX_LEN ? len : ACI_PIPE_TX_DATA_MAX_LEN;

        if (uart->write(const_cast<uint8_t*>(buffer), sent, bytesThisPass)) {
            len -= bytesThisPass;
            sent += bytesThisPass;
        }
    }

    return sent;
}

size_t BleStream::write(uint8_t buffer) {
	return uart->write(&buffer, 0, 1) ? 1 : 0;
}

void BleStream::received(uint8_t* buffer, uint8_t len) {
    for (int i = 0; i < len; i++) {
        uint16_t new_head = (uint16_t) (rx_buffer_head + 1) % RX_BUFFER_SIZE;

        if (new_head != rx_buffer_tail) {
            rx_buffer[rx_buffer_head] = buffer[i];

            rx_buffer_head = new_head;
        } else {
            // If we've reached the tail we'd overflow unless we just discard the data.
			Serial.print("Discarding 0x");
			Serial.println(buffer[i], HEX);
		}
    }
}
/******************************************************************************
 * This wrapper creates a standart interface for CAN communication
 * So it will be easier to migrate between various projects later
 *****************************************************************************/
struct can_wrapper_frame {
	uint32_t id;
	uint8_t  len;
	uint8_t  data[8];
};

#include "driver/gpio.h"
#include "driver/twai.h"

#define TWAI_BUS_0_TX GPIO_NUM_13
#define TWAI_BUS_0_RX GPIO_NUM_12

#define TWAI_BUS_1_TX GPIO_NUM_14
#define TWAI_BUS_1_RX GPIO_NUM_15

static twai_handle_t twai_bus_0;
static twai_handle_t twai_bus_1;

void can_wrapper_init_esp32_twai(twai_handle_t *bus)
{
	esp_err_t code;

	assert(bus == &twai_bus_0 || bus == &twai_bus_1);

	//Config
	//twai_handle_t *bus    = &twai_bus_0;
	gpio_num_t     bus_tx = (bus == &twai_bus_0) ?
				TWAI_BUS_0_TX : TWAI_BUS_1_TX;
	gpio_num_t     bus_rx = (bus == &twai_bus_0) ?
				TWAI_BUS_0_RX : TWAI_BUS_1_RX;
	uint8_t        bus_id = (bus == &twai_bus_0) ? 0 : 1;

	twai_general_config_t g_config =
		TWAI_GENERAL_CONFIG_DEFAULT(bus_tx, bus_rx, TWAI_MODE_NORMAL);
	twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS  ();
	twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
	
	/* g_config.tx_queue_len = 10; */

	// Install driver for TWAI bus 0
	g_config.controller_id = bus_id;
	code = twai_driver_install_v2(&g_config, &t_config, &f_config, bus);
	if (code == ESP_OK) {
		printf("TWAI driver installed\n");
	} else {
		printf("Failed to install driver (%s)\n",
			esp_err_to_name(code));
		return;
	}
	// Start TWAI driver
	code = twai_start_v2(*bus);
	if (code == ESP_OK) {
		printf("TWAI driver started\n");
	} else {
		printf("Failed to start driver (%s)\n", esp_err_to_name(code));
		return;
	}

	twai_reconfigure_alerts_v2(*bus, TWAI_ALERT_BUS_OFF, NULL);
}

/* Call only in bus off state */
void can_wrapper_kill_esp32_twai(twai_handle_t *bus)
{
	assert(bus == &twai_bus_0 || bus == &twai_bus_1);

	twai_driver_uninstall_v2(*bus);
}

/* TWAI TEST */
void can_wrapper_esp32_twai_print_status()
{
	//NOT IMPLEMENTED
}

void can_wrapper_esp32_twai_send(twai_handle_t *bus,
				         struct can_wrapper_frame *frame)
{
	assert(bus == &twai_bus_0 || bus == &twai_bus_1);

	int8_t i;

	// Configure message to transmit
	twai_message_t msg;
		
	// Message type and format settings
	// Standard vs extended format
	msg.extd = 0;

	// Data vs RTR frame         
	msg.rtr  = 0;

	// Whether the message is single shot (i.e., does not repeat on error)
	msg.ss   = 0;

	// Whether the message is a self reception request (loopback)
	msg.self = 0;

	// DLC is less or equal 8
	msg.dlc_non_comp = 0;
		    
	// Message ID and payload
	msg.identifier       = frame->id;
	msg.data_length_code = frame->len;

	/* Only send frames less or equal 8 */
	if (frame->len <= 8) {
		for (i = 0; i < frame->len; i++) {
			msg.data[i] = frame->data[i];
		}

		// Queue message for transmission
		twai_transmit_v2(*bus, &msg, 0);
	}
}

bool can_wrapper_esp32_twai_recv(twai_handle_t *bus,
					 struct can_wrapper_frame *frame)
{
	bool has_message;

	assert(bus == &twai_bus_0 || bus == &twai_bus_1);
	assert(frame != NULL);
						
	twai_message_t msg;

	if (twai_receive_v2(*bus, &msg, 0) == ESP_OK &&
	    msg.data_length_code <= 8) {
		int8_t i = 0;

		frame->id = msg.identifier;

		if (!(msg.rtr)) {
			for (i = 0; i < msg.data_length_code; i++) {
				frame->data[i] = msg.data[i];
			}
		}
		
		frame->len = i;
		has_message = true;
	} else {
		has_message = false;
	}
	
	return has_message;
}

/******************************************************************************
 * MAIN
 *****************************************************************************/
void can_wrapper_update(uint32_t delta_time_ms)
{
	uint32_t alerts;

	alerts = 0;
	twai_read_alerts_v2(twai_bus_0, &alerts, 0);

	if (alerts & TWAI_ALERT_BUS_OFF) {
		printf("TWAI_ALERT_BUS_OFF\n");

		/* Reset TWAI in case of bus off */
		twai_reconfigure_alerts_v2(twai_bus_0, 0, NULL);
		can_wrapper_kill_esp32_twai(&twai_bus_0);
		can_wrapper_init_esp32_twai(&twai_bus_0);
	}

	alerts = 0;
	twai_read_alerts_v2(twai_bus_1, &alerts, 0);

	if (alerts & TWAI_ALERT_BUS_OFF) {
		printf("TWAI_ALERT_BUS_OFF\n");

		/* Reset TWAI in case of bus off */
		twai_reconfigure_alerts_v2(twai_bus_1, 0, NULL);
		can_wrapper_kill_esp32_twai(&twai_bus_1);
		can_wrapper_init_esp32_twai(&twai_bus_1);
	}
}

bool can_wrapper_send_frame(uint8_t bus_id,
				    struct can_wrapper_frame *frame)
{	
	if (bus_id == 0) {
		can_wrapper_esp32_twai_send(&twai_bus_0, frame);
	} else if (bus_id == 1) {
		can_wrapper_esp32_twai_send(&twai_bus_1, frame);
	} else {}

	return true;
}

bool can_wrapper_recv_frame(uint8_t bus_id,
				    struct can_wrapper_frame *frame)
{
	bool has_frame;

	if (bus_id == 0) {
		has_frame = can_wrapper_esp32_twai_recv(&twai_bus_0,
								 frame);
	} else if (bus_id == 1) {
		has_frame = can_wrapper_esp32_twai_recv(&twai_bus_1,
								 frame);
	} else {
		has_frame = false;
	}
	
	return has_frame;
}

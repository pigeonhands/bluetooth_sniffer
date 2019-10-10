#include <string.h>
#include <stdio.h>


#include "uart.h"
#include "bt_radio.h"

#include "nrf_pwr_mgmt.h"
#include  "nrf_delay.h"

void init_uart();

void on_bt_packet(const ble_pdu_packet_t* packet, void *context) {
	//pass to uart.
	 PACKED_STRUCT {
		 uint8_t type;
		 uint8_t body[255];
	}payload;
	
	payload.type = packet->header.type;
	memcpy(payload.body, packet->body, packet->header.len);
	
	uart_write((uint8_t*)&payload, packet->header.len + 1);
	bt_radio_read_packet();
}

int main() {
	
	init_uart();
	bt_radio_init();
	bt_radio_on_packet(on_bt_packet, 0);
	bt_radio_read_packet();

	
	for (;;) {
		__WFE();
		__SEV();
		__WFE();
	}	
}


void init_uart() {
	nrfx_uart_config_t config = NRFX_UART_DEFAULT_CONFIG;
	//pins for nina-b1 dev kit
	config.pseltxd = 6;
	config.pselrts = 8;
	config.pselcts = 18;
	config.pselrxd = 5;
	
	//pins for nrf51 dev kit (rigato?)
	/*
	config.pseltxd = 6;
	config.pselrts = 5;
	config.pselcts = 7;
	config.pselrxd = 8;
	config.pselrxd = 8;
	*/
	uart_init(&config);
}
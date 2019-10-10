#include <string.h>
#include <stdio.h>


#include "uart.h"
#include "bt_radio.h"

#include "nrf_pwr_mgmt.h"
#include  "nrf_delay.h"

void init_uart();

void on_bt_packet(const bt_radio_message* packet, void *context) {
	//pass to uart.
	
	uart_write((uint8_t *)packet, packet->len + sizeof(packet->len));
	bt_radio_read_packet();
}

int main() {
	
	init_uart();
	bt_radio_init();
	bt_radio_on_packet(on_bt_packet, 0);
	bt_radio_read_packet();

	__enable_irq();
	for (;;) {
		__WFE();
		__SEV();
		__WFE();
		uint32_t state = NRF_RADIO->STATE;
		uint32_t u = state;
	}	
}


void init_uart() {
	nrfx_uart_config_t config = NRFX_UART_DEFAULT_CONFIG;
	//pins for nina-b1 dev kit
	/*
	config.pseltxd = 6;
	config.pselrts = 8;
	config.pselcts = 18;
	config.pselrxd = 5;
	*/
	//pins for nrf51 dev kit (rigato?)
	
	config.pseltxd = 6;
	config.pselrts = 5;
	config.pselcts = 7;
	config.pselrxd = 8;
	config.pselrxd = 8;
	
	uart_init(&config);
}
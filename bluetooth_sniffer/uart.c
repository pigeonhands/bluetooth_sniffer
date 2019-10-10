#include "uart.h"
#include  "error_check.h"
#include "nrf_log.h"
#include "circle_bufer.h"

static nrfx_uart_t uart = NRFX_UART_INSTANCE(0);
static bool tx_in_progress = false;

static void flush_messages();
static bool flushing_enabled = false;

static void _event_handler(nrfx_uart_event_t const * p_event, void * p_context) {
	
	switch (p_event->type) {
	case NRFX_UART_EVT_TX_DONE: {
			NRF_LOG_INFO("Uart sent %i bytes.", p_event->data.rxtx.bytes);
		
			/*
			 * handling the queued up writes.
			 **/
			tx_in_progress = false;
			flush_messages();
		}break;
	case NRFX_UART_EVT_RX_DONE: {
			NRF_LOG_INFO("Uart recieved %i bytes.", p_event->data.rxtx.bytes);
			flushing_enabled = true;
			flush_messages();
		}break;
	case NRFX_UART_EVT_ERROR: {
			NRF_LOG_ERROR("Uart error: %i", p_event->type);
		}break;
	}
	
}

void uart_init(nrfx_uart_config_t *config) {
	circle_buffer_init();
	
	uint32_t err = nrfx_uart_init(&uart, config, _event_handler);
	ERR_CHECK(err);
	nrfx_uart_rx_enable(&uart);
	nrfx_uart_rx(&uart, (uint8_t*)&flushing_enabled, 1);
	tx_in_progress = false;
}


void uart_read(uint8_t * rx_buffer, uint16_t bytes_to_read) {
	ret_code_t err = nrfx_uart_rx(&uart, rx_buffer, bytes_to_read);
	ERR_CHECK(err);
}

static void flush_messages() {
	if (tx_in_progress || !flushing_enabled) {
		return;
	}

	if (circle_buffer_len() > 0) {
		tx_in_progress = true;
		static uint8_t message_buffer[255];
		uint16_t msg_len = circle_buffer_read(message_buffer);
		uint32_t err = nrfx_uart_tx(&uart, message_buffer, msg_len);
	}
	
}


void uart_write(uint8_t *data, uint16_t len) {
	uint8_t message_buffer[255];
	message_buffer[0] = len;
	memcpy(&message_buffer[1], data, len);
	circle_buffer_write(message_buffer, len + 1);
	flush_messages();
}

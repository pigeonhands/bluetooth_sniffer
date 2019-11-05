#include "uart.h"
#include  "error_check.h"
#include "nrf_log.h"

static nrfx_uart_t uart = NRFX_UART_INSTANCE(0);
static bool tx_in_progress = false;

static void flush_messages();
static bool tx_enabled = false;
#define QUEUE_AMMOUNT 10

static uint16_t queue_read_index = 0;
static uint16_t queue_write_index = 0;
typedef struct {
	uint8_t tx_buffer[500];
	uint16_t len;
} queue_item;
static queue_item queue[QUEUE_AMMOUNT];


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
			tx_enabled = true;
			//flush_messages();
		}break;
	case NRFX_UART_EVT_ERROR: {
			NRF_LOG_ERROR("Uart error: %i", p_event->type);
		}break;
	}
	
}

void uart_init(nrfx_uart_config_t *config) {	
	uint32_t err = nrfx_uart_init(&uart, config, _event_handler);
	ERR_CHECK(err);
	nrfx_uart_rx_enable(&uart);
	nrfx_uart_rx(&uart, (uint8_t*)&tx_enabled, 1);
	tx_in_progress = false;
}


void uart_read(uint8_t * rx_buffer, uint16_t bytes_to_read) {
	ret_code_t err = nrfx_uart_rx(&uart, rx_buffer, bytes_to_read);
	ERR_CHECK(err);
}

static void flush_messages() {
	if (queue_read_index >= queue_write_index) {
		tx_in_progress = false;
		queue_read_index = 0;
		queue_write_index = 0;
		return;
	}
	

	queue_item* q = &queue[queue_read_index++];
	nrfx_uart_tx(&uart, q->tx_buffer, q->len);
}

void uart_write(uint8_t *data, uint16_t len) {
	if (!tx_enabled || tx_in_progress || (queue_write_index >= QUEUE_AMMOUNT)) {
		return;
	}
	queue_item* q = &queue[queue_write_index++];
	q->len = len;
	tx_in_progress = true;
	memcpy(q->tx_buffer, data, len);
	
	flush_messages();
}


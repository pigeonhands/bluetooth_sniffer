#pragma once
#include "nrfx_uart.h"


void uart_init(nrfx_uart_config_t *config);
void uart_write(uint8_t *data, uint16_t len);
void uart_read(uint8_t * rx_buffer, uint16_t bytes_to_read);

void uart_write_log(const char* msg);
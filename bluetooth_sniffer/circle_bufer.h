#pragma once
#include <stdint.h>

void circle_buffer_init();

uint16_t circle_buffer_read(uint8_t* out_data);
void circle_buffer_write(uint8_t*data, uint16_t len);
uint16_t circle_buffer_len();
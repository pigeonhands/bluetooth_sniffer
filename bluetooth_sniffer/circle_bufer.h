#pragma once
#include <stdint.h>

void circle_buffer_init();

uint16_t circle_buffer_read(void* out_data);
void circle_buffer_write(void*data, uint16_t len);
uint16_t circle_buffer_len();



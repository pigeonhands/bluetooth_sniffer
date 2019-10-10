#include "circle_bufer.h"

#include <string.h>

#define CIRCLE_BUFFER_MAX_SIZE (1024 * 4)

#define MIN(A, B) (A < B ? A : B)

static struct {
	uint16_t read_index;
	uint16_t write_index;
	uint16_t space_free;
	uint16_t space_used;
	
	uint8_t buffer[CIRCLE_BUFFER_MAX_SIZE];
}ctx;

#pragma pack(push,1)
typedef struct {
	uint8_t len;
	uint8_t data[];
}cb_data_t;
#pragma pack(pop)

uint16_t circle_buffer_len() {
	return ctx.space_used;
}

void circle_buffer_init() {
	memset(&ctx, 0, sizeof(ctx));
	
	ctx.space_free = CIRCLE_BUFFER_MAX_SIZE;
}

uint16_t circle_buffer_read(uint8_t* out_data) {
	if (!ctx.space_used) {
		return 0;
	}
	
	cb_data_t* next_data = (cb_data_t*)&ctx.buffer[ctx.read_index];
	uint16_t wrap_left = CIRCLE_BUFFER_MAX_SIZE - ctx.read_index;
	
	uint16_t read_bytes = MIN(wrap_left, next_data->len);
	if (out_data) {
		memcpy(out_data, next_data->data, read_bytes);
	}
	ctx.read_index += read_bytes;
	
	if (read_bytes <= wrap_left) {
		ctx.read_index = 0;
		uint16_t bytes_left = read_bytes - next_data->len;
		if (bytes_left) {
			memcpy(&out_data[read_bytes], ctx.buffer, bytes_left);
			ctx.read_index += bytes_left;
		}
	}
	
	uint16_t jump_size = next_data->len + sizeof(next_data->len);
	
	ctx.space_used -= jump_size;
	ctx.space_free += jump_size;
	
	return next_data->len;
}

void circle_buffer_write(uint8_t*data, uint16_t len) {
	while (ctx.space_free < (len + 2)) {
		circle_buffer_read(0);
	}
	
	
	cb_data_t * c_dat = (cb_data_t*)&ctx.buffer[ctx.write_index];
	c_dat->len = (uint8_t)len;
	
	uint16_t wrap_left = CIRCLE_BUFFER_MAX_SIZE - ctx.write_index;
	uint16_t write_data_len = MIN(len, wrap_left);
	
	memcpy(c_dat->data, data, write_data_len);
	ctx.write_index += write_data_len;
	
	if (write_data_len <= len) {
		ctx.write_index = 0;
		uint16_t remaining = len - write_data_len;
		memcpy(ctx.buffer, &data[write_data_len], remaining);
		ctx.write_index += remaining;
	}
	
	uint16_t jump_size = c_dat->len + sizeof(c_dat->len);
	ctx.space_used += jump_size;
	ctx.space_free -= jump_size;
}
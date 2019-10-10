#include "circle_bufer.h"

#include <string.h>

#define CIRCLE_BUFFER_MAX_SIZE (1024*4)

#define MIN(A, B) (A < B ? A : B)

static struct {
	uint16_t read_index;
	uint16_t write_index;
	uint16_t space_free;
	uint16_t space_used;
	
	uint8_t buffer[CIRCLE_BUFFER_MAX_SIZE];
}ctx;


uint16_t circle_buffer_len() {
	return ctx.space_used;
}

void circle_buffer_init() {
	memset(&ctx, 0, sizeof(ctx));
	
	ctx.space_free = CIRCLE_BUFFER_MAX_SIZE;
}

void raw_read(void*out, uint16_t len) {
	
	uint16_t wrap_left = CIRCLE_BUFFER_MAX_SIZE - ctx.read_index;
	uint8_t * data_ptr = &ctx.buffer[ctx.read_index];
	
	uint16_t read_bytes = MIN(wrap_left, len);
	if (out) {
		memcpy(out, data_ptr, read_bytes);
	}
	ctx.read_index += read_bytes;
	
	if (ctx.read_index >= CIRCLE_BUFFER_MAX_SIZE) {
		ctx.read_index = 0;
		uint16_t bytes_left = len - read_bytes;
		if (bytes_left) {
			if (out) {
				memcpy(&out[read_bytes], ctx.buffer, bytes_left);
			}
			
			ctx.read_index += bytes_left;
		}
	}
	
	ctx.space_used -= len;
	ctx.space_free += len;
}

uint16_t circle_buffer_read(void* out_data) {
	if (!ctx.space_used) {
		return 0;
	}
	
	uint16_t packet_len;
	raw_read(&packet_len, sizeof(packet_len));
	raw_read(out_data, packet_len);
	
	return packet_len;
}

void raw_write(void* data, uint16_t len) {
	uint8_t* data_ptr = &ctx.buffer[ctx.write_index];
	
	uint16_t wrap_left = CIRCLE_BUFFER_MAX_SIZE - ctx.write_index;
	uint16_t write_bytes = MIN(len, wrap_left);
	
	memcpy(data_ptr, data, write_bytes);
	ctx.write_index += write_bytes;
	
	if (ctx.write_index >= CIRCLE_BUFFER_MAX_SIZE) {
		ctx.write_index = 0;
		uint16_t remaining = len - write_bytes;
		memcpy(ctx.buffer, &data[write_bytes], remaining);
		ctx.write_index += remaining;
	}
	
	ctx.space_used += len;
	ctx.space_free -= len;
}

void circle_buffer_write(void*data, uint16_t len) {
	while (ctx.space_free < (len + 2)) {
		circle_buffer_read(0);
	}
	
	raw_write(&len, sizeof(len));
	raw_write(data, len);
}
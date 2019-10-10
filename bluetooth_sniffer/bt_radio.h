#pragma once
#include "pdu.h"
#include "message_type.h"

#pragma pack(push, 1)

typedef struct {
	uint16_t len;
	struct{
		uint8_t message_type;
		uint8_t rssi;
		uint8_t crc_ok;
	}header;
	
	ble_pdu_packet_t pdu;
}bt_radio_message;

#pragma pack(pop)

typedef void(*bt_radio_on_pdu_packet_f)(const bt_radio_message* packet, void*context);

void bt_radio_init();
void bt_radio_on_packet(bt_radio_on_pdu_packet_f cb, void *context);
void bt_radio_read_packet();
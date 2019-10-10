#pragma once
#include "pdu.h"

typedef void(*bt_radio_on_pdu_packet_f)(const ble_pdu_packet_t* packet, void*context);

void bt_radio_init();
void bt_radio_on_packet(bt_radio_on_pdu_packet_f cb, void *context);
void bt_radio_read_packet();
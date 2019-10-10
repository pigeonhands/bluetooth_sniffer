#pragma once
#include <stdint.h>
/*
 * https://microchipdeveloper.com/wireless:ble-link-layer-packet-types
 *
 **/
typedef enum  {
	PDU_ADV_TYPE_ADV_IND         = 0x00,
	PDU_ADV_TYPE_DIRECT_IND      = 0x01,
	PDU_ADV_TYPE_NONCONN_IND     = 0x02,
	PDU_ADV_TYPE_SCAN_REQ        = 0x03,
	PDU_ADV_TYPE_AUX_SCAN_REQ    = PDU_ADV_TYPE_SCAN_REQ,
	PDU_ADV_TYPE_SCAN_RSP        = 0x04,
	PDU_ADV_TYPE_CONNECT_IND     = 0x05,
	PDU_ADV_TYPE_AUX_CONNECT_REQ = PDU_ADV_TYPE_CONNECT_IND,
	PDU_ADV_TYPE_SCAN_IND        = 0x06,
	PDU_ADV_TYPE_EXT_IND         = 0x07,
	PDU_ADV_TYPE_AUX_ADV_IND     = PDU_ADV_TYPE_EXT_IND,
	PDU_ADV_TYPE_AUX_SCAN_RSP    = PDU_ADV_TYPE_EXT_IND,
	PDU_ADV_TYPE_AUX_SYNC_IND    = PDU_ADV_TYPE_EXT_IND,
	PDU_ADV_TYPE_AUX_CHAIN_IND   = PDU_ADV_TYPE_EXT_IND,
	PDU_ADV_TYPE_AUX_CONNECT_RSP = 0x08,
} pdu_adv_type_e;

#pragma pack(push, 1)

typedef struct {
	struct {
		uint8_t type : 4;
		uint8_t rfu : 1;
		uint8_t chan_sel : 1;
		uint8_t tx_addr : 1;
		uint8_t rx_addr : 1;

		uint8_t len : 8;
	
	}header;
	
	uint8_t body[255];
	
}ble_pdu_packet_t;

#pragma pack(pop)
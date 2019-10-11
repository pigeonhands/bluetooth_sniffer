#include "bt_radio.h"
#include "nrf.h"
#include "uart.h"
#include <string.h>


/*	Refrences 
 *	https://www.nordicsemi.com/-/media/DocLib/Other/Product_Spec/nRF52840PSv10.pdf
 *	https://cdn.sparkfun.com/datasheets/Wireless/Bluetooth/nRF52832_PS_v1.0.pdf
 *	https://github.com/bluekitchen/raccoon
 **/
static struct {
	bt_radio_message msg;
	void *context;
	bt_radio_on_pdu_packet_f on_packet;
}ctx; 

typedef enum {
	ADV_CHANNEL_1 = 37,
	ADV_CHANNEL_2 = 38,
	ADV_CHANNEL_3 = 39,
}adv_channels_e;

void bt_radio_on_packet(bt_radio_on_pdu_packet_f cb, void *context) {
	ctx.context = context;
	ctx.on_packet = cb;
}

static void disable_radio() {
	if (NRF_RADIO->STATE > 0) {
		NRF_RADIO->EVENTS_DISABLED = 0;
		NRF_RADIO->TASKS_DISABLE = 1;
		while (NRF_RADIO->EVENTS_DISABLED) {
		}
	}
}

static void radio_clock_init() {
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART    = 1;

	/* Wait for the external oscillator to start up */
	while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) {
		// Do nothing.
	}
	//clock enabled.
}

void bt_radio_read_packet() {
	NRF_RADIO->PACKETPTR = (uint32_t)&ctx.msg.pdu;
	
	if (NRF_RADIO->STATE == 0) {NRF_RADIO->EVENTS_READY = 0;
		NRF_RADIO->EVENTS_END = 0;
		NRF_RADIO->TASKS_RXEN = 1U;
	} else {
		NRF_RADIO->TASKS_START = 1U;
	}
}

static uint8_t channel_to_freq(int channel) {
	if (channel == 37) {
		return 2;
	} else if (channel == 38) {
		return 26;
	} else if (channel == 39) {
		return 80;
	} else if (channel < 11) {
		return 2*(channel + 2);
	} else {
		return 2*(channel + 3);
	}
}

static uint8_t adv_channel_to_freq(adv_channels_e ch) {
	switch (ch) {
	case ADV_CHANNEL_1:
		return 02;
	case ADV_CHANNEL_3:
		return 80;
	case ADV_CHANNEL_2:
		return 26;
	}
}

static void set_radio_to_channel(adv_channels_e ch) {
	NRF_RADIO->FREQUENCY = adv_channel_to_freq(ch);
	NRF_RADIO->DATAWHITEIV = (uint32_t)ch;
}

static void radio_configure() {
	
	NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_Pos4dBm << RADIO_TXPOWER_TXPOWER_Pos);
	NRF_RADIO->MODE = (RADIO_MODE_MODE_Ble_1Mbit << RADIO_MODE_MODE_Pos);
	
	set_radio_to_channel(ADV_CHANNEL_1);
	
	/*
	 *	0x555555 is default for BLE adverts
	 *	
	 *	CRCPOLY are the values 
	 *	used in the crc function 
	 *	(0x06 and 0x5B)
	 **/
	NRF_RADIO->CRCINIT = 0x555555;	/* Initial value of CRC */
	NRF_RADIO->CRCPOLY = 0x00065B;	/* CRC polynomial function */
	
	NRF_RADIO->CRCCNF  = (RADIO_CRCCNF_LEN_Three << RADIO_CRCCNF_LEN_Pos) |
	                     (RADIO_CRCCNF_SKIPADDR_Skip << RADIO_CRCCNF_SKIPADDR_Pos); /* Skip Address when computing CRC */
	
	/*
	 * logical address 'short form'?
	 * expands to logical addresses 0-7
	 * used in TXADDRESS/RXADDRESSES etc.
	 * 
	 *	Essentualy the mac address to send/recieve on.
	 **/
	
	NRF_RADIO->PREFIX0	=	(0x8E89BED6 >> 24) & RADIO_PREFIX0_AP0_Msk;
	NRF_RADIO->BASE0	=	(0x8E89BED6 << 8)  & RADIO_BASE0_BASE0_Msk;	
	
	
	NRF_RADIO->TXADDRESS	= 0; // transmit on logical address 0
    NRF_RADIO->RXADDRESSES	=	(RADIO_RXADDRESSES_ADDR0_Enabled << RADIO_RXADDRESSES_ADDR0_Pos);
							//	|(RADIO_RXADDRESSES_ADDR1_Enabled << RADIO_RXADDRESSES_ADDR1_Pos);
	
	/*
	 * Structure of the BLE packet to look for
	 *	1 byte of metadata
	 *	1 byte length
	 **/
	NRF_RADIO->PCNF0 = (
			(((1UL) << RADIO_PCNF0_S0LEN_Pos) & RADIO_PCNF0_S0LEN_Msk) |  /* Length of S0 field in bytes 0-1.    */
			(((0UL) << RADIO_PCNF0_S1LEN_Pos) & RADIO_PCNF0_S1LEN_Msk) |  /* Length of S1 field in bits 0-8.     */
			(((8UL) << RADIO_PCNF0_LFLEN_Pos) & RADIO_PCNF0_LFLEN_Msk)    /* Length of length field in bits 0-8. */
	);
	
	/*
	 * How to atcualy read the 
	 * ble packet once we find it.
	 *
	 **/
	NRF_RADIO->PCNF1 = (
		 (((250UL) << RADIO_PCNF1_MAXLEN_Pos) & RADIO_PCNF1_MAXLEN_Msk)   |                     /* Maximum length of payload in bytes [0-255] */
		 (((0UL) << RADIO_PCNF1_STATLEN_Pos) & RADIO_PCNF1_STATLEN_Msk)   |						/* Expand the payload with N bytes in addition to LENGTH [0-255] */
		 (((3UL) << RADIO_PCNF1_BALEN_Pos) & RADIO_PCNF1_BALEN_Msk)       |						/* Base address length in number of bytes. */
		 (((RADIO_PCNF1_ENDIAN_Little) << RADIO_PCNF1_ENDIAN_Pos) & RADIO_PCNF1_ENDIAN_Msk) |	/* Endianess of the S0, LENGTH, S1 and PAYLOAD fields. */
		 (((1UL) << RADIO_PCNF1_WHITEEN_Pos) & RADIO_PCNF1_WHITEEN_Msk)							/* Enable packet whitening */
	 );

	NRF_RADIO->MODECNF0     = (RADIO_MODECNF0_RU_Default << RADIO_MODECNF0_RU_Pos) |
                              (RADIO_MODECNF0_DTX_B1 << RADIO_MODECNF0_DTX_Pos);
	
	
}

static void radio_configure_irq() {
	
	NVIC_SetPriority(RADIO_IRQn, 0);
	
	NRF_RADIO->INTENSET =	(RADIO_INTENSET_END_Enabled << RADIO_INTENSET_END_Pos);//(RADIO_INTENSET_READY_Msk << RADIO_INTENSET_READY_Pos);// |		
	NRF_RADIO->SHORTS	= RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_ADDRESS_RSSISTART_Msk;
	
	NVIC_ClearPendingIRQ(RADIO_IRQn);
	NVIC_EnableIRQ(RADIO_IRQn);
}


void bt_radio_init() {
	memset(&ctx, 0, sizeof(ctx));
	radio_clock_init();
	
	radio_configure();
	
	radio_configure_irq();
}

#ifdef __cplusplus
extern "C" 
#endif
void RADIO_IRQHandler(void) {
	/*//Debugging. Short is set so no need to process.
	if (NRF_RADIO->EVENTS_READY) {	
		NRF_RADIO->EVENTS_READY = 0;
		uart_write_log("radio ready\n\r");
	}
	*/
	
	if (NRF_RADIO->EVENTS_END) {
		NRF_RADIO->EVENTS_END = 0U;
		ctx.msg.len = ctx.msg.pdu.header.len + sizeof(ctx.msg.pdu.header) + sizeof(ctx.msg.header);
		ctx.msg.header.message_type = MESSAGE_TYPE_RADIO;
		ctx.msg.header.rssi = NRF_RADIO->RSSISAMPLE;
		
		uint8_t crc_ok = (NRF_RADIO->CRCSTATUS & RADIO_CRCSTATUS_CRCSTATUS_Msk) ==
                 (RADIO_CRCSTATUS_CRCSTATUS_CRCOk << RADIO_CRCSTATUS_CRCSTATUS_Pos);
		ctx.msg.header.crc_ok = crc_ok;
		

		if (ctx.on_packet) {
			ctx.on_packet(&ctx.msg, ctx.context);
		}
	}
}
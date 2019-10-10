PDU_ADV_TYPE_ADV_IND         = 0x00
PDU_ADV_TYPE_DIRECT_IND      = 0x01
PDU_ADV_TYPE_NONCONN_IND     = 0x02
PDU_ADV_TYPE_SCAN_REQ        = 0x03
PDU_ADV_TYPE_AUX_SCAN_REQ    = PDU_ADV_TYPE_SCAN_REQ
PDU_ADV_TYPE_SCAN_RSP        = 0x04
PDU_ADV_TYPE_CONNECT_IND     = 0x05
PDU_ADV_TYPE_AUX_CONNECT_REQ = PDU_ADV_TYPE_CONNECT_IND
PDU_ADV_TYPE_SCAN_IND        = 0x06
PDU_ADV_TYPE_EXT_IND         = 0x07
PDU_ADV_TYPE_AUX_ADV_IND     = PDU_ADV_TYPE_EXT_IND
PDU_ADV_TYPE_AUX_SCAN_RSP    = PDU_ADV_TYPE_EXT_IND
PDU_ADV_TYPE_AUX_SYNC_IND    = PDU_ADV_TYPE_EXT_IND
PDU_ADV_TYPE_AUX_CHAIN_IND   = PDU_ADV_TYPE_EXT_IND
PDU_ADV_TYPE_AUX_CONNECT_RSP = 0x08

from app.pdu_messages import PduUnknown, PduAdvIndirect

PDU_TYPE_DICT = {
    PDU_ADV_TYPE_ADV_IND: PduAdvIndirect.PduAdvIndirected,
}


def get_message(data):
    pdu_type = data[0]
    if pdu_type in PDU_TYPE_DICT:
        return PDU_TYPE_DICT[pdu_type](data[1:])
    else:
        return PduUnknown.PduUnknown(pdu_type)

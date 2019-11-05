import app.pdu_messages.adv_data as adv_data
from app.pdu_messages.PduAdvType import PduAdvType

class PduAdvScanReponse(PduAdvType):
    def __init__(self, _data):
        self.packet_type = "scan resp"
        self.set_data(_data)


    


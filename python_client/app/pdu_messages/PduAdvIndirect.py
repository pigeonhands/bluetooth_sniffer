import app.pdu_messages.adv_data as adv_data
from app.pdu_messages.PduAdvType import PduAdvType

class PduAdvIndirected(PduAdvType):
    def __init__(self, _data):
        self.packet_type = "adv ind"
        self.set_data(_data)

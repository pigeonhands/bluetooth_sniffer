class PduUnknown:
    def __init__(self, _type):
        self.type =_type
        self.packet_type = 'unknown'

    def __repr__(self):
        return "{:<15} Type: {}".format(self.packet_type, self.type)
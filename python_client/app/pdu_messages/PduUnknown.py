class PduUnknown:
    def __init__(self, _type):
        self.type =_type

    def __repr__(self):
        return "<unknown> Type: {}".format(self.type)
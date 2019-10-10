
def get_local_name(adv_data):
    

class PduAdvIndirected:
    def __init__(self, _data):
        self.data = _data
        self.address = self.data[:6]
        self.adv_type = self.data[7]
        self.adv_data = self.data[8:]

    def get_name_hex(self):
        return "".join(format(x, 'x') for x in self.address)
    def __repr__(self):
        return "<adv ind> ({}).  ADV Type: {}".format(self.address, self.adv_type)


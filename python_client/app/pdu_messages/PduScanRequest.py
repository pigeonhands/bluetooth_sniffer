class PduScanRequest:
    def __init__(self, _data):
        self.packet_type = "scan req"
        self.data = _data
        self.adv_addr = self.data[:6]
        self.peer_adv = self.data[6:12]

    def addr_hex(self, addr_data):
        return ":".join('%02x'%x for x in addr_data[::-1])

    def addr_adv_hex(self):
        return self.addr_hex(self.adv_addr)

    def addr_peer_hex(self):
        return self.addr_hex(self.peer_adv)
        
    def __repr__(self):
        return "{:<15} ({}) -> ({})".format(self.packet_type, self.addr_adv_hex(), self.addr_peer_hex())
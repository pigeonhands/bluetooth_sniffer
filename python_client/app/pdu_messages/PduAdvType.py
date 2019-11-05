import app.pdu_messages.adv_data as adv_data

class PduAdvType:
    def set_data(self, _data):
        self.data = _data
        self.address = self.data[:6]
        self.adv_data = self.data[6:]
        self.name = ""
        if not hasattr(self, 'packet_type'):
            self.packet_type = "adv_unknown"

        adv_name_seg = adv_data.get_adv_section(self.adv_data, adv_data.BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME)
        if adv_name_seg is not None:
            self.name = str(bytes(adv_name_seg), encoding='utf8')

    def addr_hex(self):
        return ":".join('%02x'%x for x in self.address[::-1])

    def __repr__(self):
        return "{:<15} ({}) {}".format(self.packet_type, self.addr_hex(), self.name)
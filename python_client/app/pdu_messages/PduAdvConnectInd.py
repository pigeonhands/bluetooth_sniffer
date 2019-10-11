# http://rfmw.em.keysight.com/wireless/helpfiles/n7606/Content/Main/PDU_Payload_Setting_4.htm
import struct

class PduAdvConnectInd:
    def __init__(self, _data):
        self.data = _data
        self.rssi = 0
        (self.initor_addr,      #6
        self.advertiser_addr,   #6
        self.le_connect_addr,   #4
        self.crc_init,          #3
        self.window_size,       #1
        self.window_offset,     #2
        self.interval,          #2     
        self.latency,           #2
        self.timeout,           #2
        self.channel_map,       #5
        self.hop_sca            #1
        ) = struct.unpack("!6p6p4p3pcHHHH5pc", self.data)

    def addr_hex(self,addr):
        return ":".join('%02x'%x for x in addr[::-1])
    
    def __repr__(self):
        return "<conn ind> ({}) -> ({}) {}".format(
            self.addr_hex(self.initor_addr),
            self.addr_hex(self.advertiser_addr),
            self.rssi)
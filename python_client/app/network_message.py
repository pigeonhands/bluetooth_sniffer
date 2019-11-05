import app.pdu_messages
import struct

#pdu message handler


def handle(data):
    HEADER_SIZE = 4
    rssi, crc_ok, channel = struct.unpack("<b?h", data[:HEADER_SIZE])
    rssi = -data[0]
    crc_ok = data[1]
    if crc_ok == 0:
        #print("crc failed message type",(data[HEADER_SIZE]) & 0x0F)
        return
    msg = app.pdu_messages.get_message(data[HEADER_SIZE:])
    print(crc_ok, channel, rssi, msg)

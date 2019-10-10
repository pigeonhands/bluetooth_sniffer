import app.pdu_messages

#pdu message handler


def handle(data):
    rssi = -data[0]
    crc_ok = data[1]
    msg = app.pdu_messages.get_message(data[2:])
    setattr(msg,'rssi',rssi)
    print(crc_ok, msg)

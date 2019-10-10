import app.pdu_messages

#pdu message handler


ADVS = {}

def handle(data):
    rssi = data[0]
    msg = app.pdu_messages.get_message(data[1:])
    setattr(msg,'rssi',rssi)
    print(msg)
    
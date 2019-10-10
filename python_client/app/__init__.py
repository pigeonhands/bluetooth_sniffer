import serial
from app.sniffer import Sniffer
import app.network_message

MESSAGE_HANDLERS = {
    1: app.network_message.handle
}

def process_message(data):
    msg_type = data[0]
    if msg_type in MESSAGE_HANDLERS:
        MESSAGE_HANDLERS[msg_type](data[1:])


def run(com_port):
    print("Connecting to", com_port)
    sniffer = Sniffer(com_port)
    sniffer.enable()
    while True:
        data = sniffer.read_packet()
        process_message(data)
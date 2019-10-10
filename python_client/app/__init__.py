import serial
import app.pdu_messages

def process_packet(data):
    message = app.pdu_messages.get_message(data)
    print(message)


def run(com_port):
    print("Connecting to", com_port)
    s = serial.Serial(com_port, baudrate=115200)
    if s.is_open:
        print("Successfully connected. Reading...")
    s.write([1]) #write 1 byte to start uart tx
    while True:
        len_byte = s.read(size=1)[0]
        data = s.read(size=len_byte)
        process_packet(data)
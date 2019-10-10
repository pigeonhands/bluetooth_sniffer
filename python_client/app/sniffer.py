import serial

class Sniffer:
    def __init__(self, com_port):
        print("Starting sniffer on com: ",com_port)
        self.s = serial.Serial(com_port, baudrate=115200)
    
    def enable(self):
        self.s.write([1])

    def read_buffer(self,size):
        buff = []
        while len(buff) < size:
            r = self.s.read(size-len(buff))
            if len(r) > 0:
                buff += r
        return buff

    def read_length(self):
        length_buffer = self.read_buffer(2)
        packet_len = (length_buffer[0]) | (length_buffer[1]  << 8)
        return packet_len

    def read_packet(self):
       packet_size = self.read_length()
       packet = self.read_buffer(packet_size)
       return packet
    
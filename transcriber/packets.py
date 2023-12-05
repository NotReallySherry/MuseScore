import random, socket, time, sys, errno
from smbus2 import SMBus, i2c_msg
s = None
# PC, tempo, key:power, key:power

NUM_SENSORS = 8
HIGH_THRESHOLD = 170
MED_THRESHOLD = 120
LOW_THRESHOLD = 70
SEQ_NUM = 0
TEMPO = 100

bus = SMBus(1)

def pollRandom():
    sensor_data = []
    for _ in range(NUM_SENSORS):
        sensor_data.append(random.randrange(0,256))
    return sensor_data

def pollSensors():
    values = []
    for i in range(0, 16):
        addr = None
        if i <= 7:
            addr = 0x16
        else:
            addr = 0x17
        try:
            write_window = [0x08, 0x11, i % 8]
            msg = i2c_msg.write(addr, write_window)
            bus.i2c_rdwr(msg)
        except:
            print("error writing byte")
        try:
            data = bus.read_byte_data(addr, 0)
            values.append(data)
        except:
            print("remote i/o error :(")
            return None
    return values


def genAndSend(sensor_data, connection):
    global SEQ_NUM
    message = f"{int(round(time.time() * 1000))},"
    SEQ_NUM += 1
    for i in range(len(sensor_data)):
        power = 0
        if sensor_data[i] > HIGH_THRESHOLD:
            power = 3
        elif sensor_data[i] > MED_THRESHOLD:
            power = 2
        elif sensor_data[i] > LOW_THRESHOLD:
            power = 1
        message += f"{i}:{power},"
    print(message)
    try:
        connection.sendall(message.encode('utf-8'))
    except IOError as e:
        if e.errno == errno.EPIPE:
            return -1
    return 0


def socket_setup():
    global s
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    my_host = "169.254.88.83"
    my_port = 8080
    s.bind((my_host, my_port))
    s.listen(1)

    print(f"Server listening on {my_host}:{my_port}")

    connection, address = s.accept()
    print(f"Connection from {address}")
    data = connection.recv(1024).decode('utf-8')
    return connection

if __name__ == "__main__":
    while True:
        connection = socket_setup()
        while True:
            sensor_data = pollSensors()
            if sensor_data == None:
                break
            if genAndSend(sensor_data, connection) != 0:
                break
            time.sleep(0.1)
        connection.close()

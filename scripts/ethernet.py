import socket
import time
import serial.tools.list_ports

from serial_port import *
from logger import *

def GetIP(ser):
    logHeader("Получение IP")
    command = b'\xF0\x03\x13\xEC\x00\x14\x95\x95'
    try:
        ser.write(command)
        time.sleep(2)
        sout = bytearray()
        while ser.inWaiting() > 0:
            sout.extend(ser.read(1))
        ip = ""
        for x in sout[8:16:2]:
            ip += str(x) + "."
        logString("Получен ip " + ip[:-1])
        return ip[:-1]
    except Exception as e:
        logString(e)
    logString("Получить ip не удалось")
    return "0.0.0.0"
        

def TurnOnDHCP(e, ser):
    logHeader("Включение DHCP")
    command = b'\xF0\x10\x13\xEC\x00\x02\x04\x00\x01\x00\x01\xB3\x7D'
    try:
        ser.write(command)
        time.sleep(2)
        ser.read(100)
        time.sleep(7)
        e.SpiSetPowerState(False)
        time.sleep(2.5)
        e.SpiSetPowerState(True)
        ser.read(100)
        time.sleep(7)
    except Exception as e:
        logString(e) 
        
def TestEthernet(TCP_IP):
#    TCP_IP = '172.16.5.71'
    TCP_PORT = 502
    is_ok = False
    MESSAGE = b'\x00\x38\x00\x00\x00\x06\xF0\x04\x06\xA4\x00\x03'

    logHeader("Проверка Ethernet")
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        logString("Подключение к " + TCP_IP + ":" + str(TCP_PORT))
        s.connect((TCP_IP, TCP_PORT))
        s.send(MESSAGE)
        s.settimeout(3.0)
        data = s.recv(1024)
        print("Получен ответ " + str(data))
        if (len(data) > 14):
            if data[10] == 0x11 and data[12] == 0x22 and data[14] == 0x33:
                is_ok = True
    except Exception as e:
        logString(e)
#    s.close()
    if is_ok:
        logResult("OK")
        return s
    else:
        logResult("FAIL")
        return None
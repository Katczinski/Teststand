import serial.tools.list_ports
from logger import *

def GetPort():
    vid = 0x0403
    pid = 0x6015

    for _port in serial.tools.list_ports.comports():
        if _port.vid == vid and _port.pid == pid:
            port = _port
    if 'port' not in locals():
        logString("USB-порт не обнаружен")
        logResult("FAIL")
        return None
    print(port)
    ser = serial.Serial(port.name,
                    baudrate=115200,\
                    parity=serial.PARITY_NONE,\
                    stopbits=serial.STOPBITS_ONE,\
                    bytesize=serial.EIGHTBITS,\
                    timeout=0)
    ser.read(100)
    return ser
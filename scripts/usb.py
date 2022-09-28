import serial.tools.list_ports
import time
from logger import *

def TestUSB(e):
    logHeader("Проверка USB")
    vid = 0x0403
    pid = 0x6015

    for _port in serial.tools.list_ports.comports():
        if _port.vid == vid and _port.pid == pid:
            port = _port
    if 'port' not in locals():
        logString("USB-порт не обнаружен")
        return logResult("FAIL")
    print(port)

    command = b'\xF0\x04\x06\xA4\x00\x03\xE4\x41'
    ser = serial.Serial(port.name,
                        baudrate=115200,\
                        parity=serial.PARITY_NONE,\
                        stopbits=serial.STOPBITS_ONE,\
                        bytesize=serial.EIGHTBITS,\
                        timeout=0)
    time.sleep(2)
    ser.write(command)
    time.sleep(2)
    sout = bytearray()
    while ser.inWaiting() > 0:
        sout.extend(ser.read(1))
    print(sout)
    is_ok = False
    if (len(sout) > 8):
        if sout[4] == 0x11 and sout[6] == 0x22 and sout[8] == 0x33:
            is_ok = True
    ser.close()
    return logResult("OK" if is_ok else "FAIL")
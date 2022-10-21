import serial.tools.list_ports
import time
from logger import *
#from serial_port import *

def TestUSB(ser):
    logHeader("Проверка USB")

    command = b'\xF0\x04\x06\xA4\x00\x03\xE4\x41'
    try:
        
        time.sleep(2)
        ser.read(100)
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
        return logResult("OK" if is_ok else "FAIL")
    except:
        logString("Не удалось подключиться")
        return logResult("FAIL")
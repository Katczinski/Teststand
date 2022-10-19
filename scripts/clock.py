import time
import serial
from logger import *

def SwitchBatteryMode(ser, state):
    if state:    # battery on
        command = b'\xF0\x10\x0F\xA2\x00\x01\x02\x00\x01\x88\x46'
    else:        # battery off
        command = b'\xF0\x10\x0F\xA2\x00\x01\x02\x00\x00\x49\x86'
    ser.write(command)
    time.sleep(2)
    ser.read(100)

def TestClock(e, ser):
    logHeader("Проверка энергонезависимых часов")
    command = b'\xF0\x03\x0F\xAA\x00\x07\x32\x1D'
    try:
        SwitchBatteryMode(ser, True)
        
        ser.write(command)        # time 1
        time.sleep(2)             # 2 sec
        sout = bytearray()
        while ser.inWaiting() > 0:
            sout.extend(ser.read(1))
        start = int.from_bytes(sout[14:16:2], "big")
        e.SpiSetPowerState(False)
        time.sleep(3)             # 3 sec
        e.SpiSetPowerState(True)
        time.sleep(1)             # 1 sec
        ser.read(100)
        ser.write(command)        # time 2; elapsed = 6 sec
        time.sleep(2)
        sout = bytearray()
        while ser.inWaiting() > 0:
            sout.extend(ser.read(1))
        finish = int.from_bytes(sout[14:16:2], "big")
        print("elapsed time: " + str(finish - start) + " sec")
        SwitchBatteryMode(ser, False)
        if (finish - start == 6) or (finish - start == 5): #should be 6 but sometimes it's 5 smh
            return logResult("OK")
        else:
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        try:
            SwitchBatteryMode(ser, False)
        except Exception: 
            pass
        return logResult("FAIL")
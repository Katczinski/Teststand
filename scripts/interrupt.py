from libd2xx import *
from logger import *
from ethernet import *
import time

def TestInterrupt(e, ser):
    logHeader("Проверка линии interrupt")
    is_ok = True
    command = b'\xF0\x04\x09\xB0\x00\x01\x26\x90'
    
    ser.write(command)
    time.sleep(2)
    sout = bytearray()
    while ser.inWaiting() > 0:
        sout.extend(ser.read(1))
    if not sout[4] == 1:
        is_ok = False

    e.SpiDioDirection(12, Direction_out)
    e.SpiDioWrite(12, False)

    ser.write(command)
    time.sleep(2)
    sout = bytearray()
    while ser.inWaiting() > 0:
        sout.extend(ser.read(1))
    if not sout[4] == 0:
        is_ok = False

    e.SpiDioDirection(12, Direction_out)
    e.SpiDioWrite(12, True)

    ser.write(command)
    time.sleep(2)
    sout = bytearray()
    while ser.inWaiting() > 0:
        sout.extend(ser.read(1))
    if not sout[4] == 1:
        is_ok = False
    if is_ok:
        return logResult("OK")
    else:
        return logResult("FAIL")
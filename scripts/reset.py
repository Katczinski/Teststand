from logger  import *
from libd2xx import *
import time

def TestReset(e, ser):
    logHeader("Проверка линии reset")
#    write 02 to temp register
#    F0 10 06 A9 00 01 02 00 02 50 3C
    command = b'\xF0\x10\x06\xA9\x00\x01\x02\x00\x02\x50\x3C'
    
    ser.write(command)
    time.sleep(2)
    sout = ser.read(100)
#    logString(' '.join('{:02x}'.format(x) for x in sout))
#    set block reset true
#    F0 10 09 BA 00 01 02 00 01 ED 9E
    command = b'\xF0\x10\x09\xBA\x00\x01\x02\x00\x01\xED\x9E'
    ser.write(command)
    time.sleep(2)
    sout = ser.read(100)
#    logString(' '.join('{:02x}'.format(x) for x in sout))
#    reset
    e.SpiDioDirection(9, Direction_out)
    e.SpiDioWrite(9, False)
#    read F0 04 06 A9 00 01 F4 43 should be 00 02
    command = b'\xF0\x04\x06\xA9\x00\x01\xF4\x43'
    ser.write(command)
    time.sleep(2)
    sout = bytearray()
    while ser.inWaiting() > 0:
        sout.extend(ser.read(1))
#    logString(' '.join('{:02x}'.format(x) for x in sout))
    if not sout[4] == 2:
        return logResult("FAIL")
#    set block reset false
    command = b'\xF0\x10\x09\xBA\x00\x01\x02\x00\x02\xAD\x9F'
    ser.write(command)
    time.sleep(2)
    sout = ser.read(100)
#    reset
    e.SpiDioDirection(9, Direction_out)
    e.SpiDioWrite(9, False)
#    read F0 04 06 A9 00 01 F4 43 should be 00 00
    command = b'\xF0\x04\x06\xA9\x00\x01\xF4\x43'
    ser.write(command)
    time.sleep(2)
    sout = bytearray()
    while ser.inWaiting() > 0:
        sout.extend(ser.read(1))
    if not sout[4] == 0:
        return logResult("FAIL")
#    set block reset auto
    command = b'\xF0\x10\x09\xBA\x00\x01\x02\x00\x00\x2C\x5E'
    ser.write(command)
    time.sleep(2)
    sout = ser.read(100)
    return logResult("OK")

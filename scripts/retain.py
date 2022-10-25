import serial.tools.list_ports
import time
from logger import *

def TestSingleVar(e, ser, name, retain):
    readRetain = b'\xF0\x1C\x00\x00\xF2\xE2'
    retainAnsw = b'\xF0\x1B\x01\x02\x03\x04\x05\xE6\x5F'
    guid = b'\xF0\x1D\x00\x01\x01\x02\x03\x04\x05\x06\x07\x08\x01\x02\x03\x04\x05\x06\x07\x08\xB1\x4C'
    readGuid = b'\xF0\x1E\xC5\xB8'
    guidAnsw = b'\xF0\x1D\x01\x02\x03\x04\x05\xE6\x39'
    logString(name)
    
#   Write retain    
    ser.read(100)
    ser.write(retain)
    time.sleep(1)
    sout = bytearray()
    while ser.inWaiting() > 0:
        sout.extend(ser.read(1))
    exp = ' '.join(format(x, '02x') for x in retainAnsw)
    real = ' '.join(format(x, '02x') for x in sout)
    if sout != retainAnsw:
        logString("Ожидалось: " + str(exp))
        logString("Получено: " + str(real))
        logResult("FAIL")
        return False

#   Reset Power
    e.SpiSetPowerState(False)
    time.sleep(10)
    e.SpiSetPowerState(True)
    time.sleep(1)
    ser.read(100)
    
#   Read retain
    ser.write(readRetain)
    time.sleep(1)
    sout = bytearray()
    while ser.inWaiting() > 0:
        sout.extend(ser.read(1))
    exp = ' '.join(format(x, '02x') for x in retain[2:12])
    real = ' '.join(format(x, '02x') for x in sout[2:12])
    if retain[2:12] != sout[2:12]:
        logString("Ожидалось: " + str(exp))
        logString("Получено: " + str(real))
        logResult("FAIL")
        return False

    ser.write(guid)
    time.sleep(1)
    sout = bytearray()
    while ser.inWaiting() > 0:
        sout.extend(ser.read(1))
    exp = ' '.join(format(x, '02x') for x in guidAnsw)
    real = ' '.join(format(x, '02x') for x in sout)
    if sout != guidAnsw:
        logString("Ожидалось: " + str(exp))
        logString("Получено: " + str(real))
        logResult("FAIL")
        return False


    ser.write(readGuid)
    time.sleep(1)
    sout = bytearray()
    while ser.inWaiting() > 0:
        sout.extend(ser.read(1))
    exp = ' '.join(format(x, '02x') for x in guid[2:18])
    real = ' '.join(format(x, '02x') for x in sout[2:18])
    if guid[2:18] == sout[2:18]:
        logResult("OK")
        return True
    else:
        logString("Ожидалось: " + str(exp))
        logString("Получено: " + str(real))
        logResult("FAIL")
        return False
    

def TestRetain(e, ser):
    logHeader("Проверка энергонезависимой памяти")
    retain1 = b'\xF0\x1B\x00\x00\x01\x01\x02\x02\x03\x03\x04\x04\x0C\x8E'
    retain2 = b'\xF0\x1B\x00\x00\x01\x02\x03\x04\x05\x06\x07\x08\xA6\x23'
    
    if not TestSingleVar(e, ser, "Запись и чтение переменной, попытка №1", retain1):
        return
    if not TestSingleVar(e, ser, "Запись и чтение переменной, попытка №2", retain2):
        return
    
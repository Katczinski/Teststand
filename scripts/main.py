from libd2xx      import *
from I2C          import *
from modbus       import *
from usb          import *
from logger       import *
from ethernet     import *
from rs485        import *
from serial_port  import *
from uart         import *
from clock        import *
from retain       import *
from led          import *

import time

if len(sys.argv) > 2:
    module = sys.argv[1]    # 4
    modif  = sys.argv[2]    # 4
else:
    module = 4
    modif = 4
#============================definitions==========================

def PrintBytearray(array):
    res = ' '.join(format(x, '02x') for x in array)
    logString("array: " + str(res))

def setPowerState(state):
    e = D2xx()
    e.SpiSetPowerState(state)

def ResetPower(e):
    if not e:
        e = D2xx()
    e.SpiSetPowerState(False)
    time.sleep(2.5)
    e.SpiSetPowerState(True)

def getD2xxStatus(e):
    logString(e.FtdiGetStatus())
    logString(e.SpiGetStatus())
    logString(e.Modbus1GetStatus())
    logString(e.Modbus2GetStatus())

def SwitchMode(ser, mode):
    readMode = b'\xF0\x04\x23\x8C\x00\x01\xEE\x84'
    if mode:
        logHeader("Перевод в режим шлюза")
        message = b'\xF0\x10\x23\x96\x00\x01\x02\x01\x00\xA1\xA0'
        exp = b'\x01\x00'
    else:
        logHeader("Перевод в режим исполнения алгоритма")
        message = b'\xF0\x10\x23\x96\x00\x01\x02\x00\x00\xA0\x30'
        exp = b'\x00\x00'
    try:
        ser.write(message)
        time.sleep(2)
        ser.read(100)
        ser.write(readMode)
        time.sleep(2)
        sout = bytearray()
        while ser.inWaiting() > 0:
            sout.extend(ser.read(1))
        if exp == sout[3:5]:
            logResult("OK")
        else:
            logResult("FAIL")
    except Exception as e:
        logString(e)

def Test():
    e = D2xx()
    getD2xxStatus(e)
    e.SpiSetPowerState(True)
    time.sleep(1)
    try:
        ser = GetPort()
#        SwitchMode(ser, True)  
#==================Get ip and socket dynamically==================
#        TurnOnDHCP(e, ser)
#        ip = GetIP(ser)
#        TestEthernet(ip) #'192.168.0.111'
#=========================Use custom ip===========================
        ip = '192.168.0.124'
#=================================================================
#        TestUart(e, ip)
#        TestUSB(ser)
#        TestISquareC(e, module)
#        TestModbus1(e, ip)
#        TestModbus2(e, ip)
#        TestLeftRs(e, ip)
#        TestRightRs(e, ip) 
#        TestClock(e, ser)
#        TestRetain(e, ser)
        TestGreenLed(ser)
        TestRedLed(ser)
#        SwitchMode(ser, False)
        ser.close()
    except Exception as ex:
        logString(ex)
#    finally:
#        e.SpiSetPowerState(False)

#===============================main===============================
#ResetPower(None)
Test()
#setPowerState(False)
#setPowerState(True)

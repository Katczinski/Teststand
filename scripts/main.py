from libd2xx  import *
from I2C      import *
from modbus   import *
from usb      import *
from logger   import *
from ethernet import *
from rs485    import *
from serial_port import *
module = sys.argv[1]    # 4
modif = sys.argv[2]     # 4

#============================definitions=================================

def setPowerState(state):
    e = D2xx()
    e.SpiSetPowerState(state)

def ResetPower():
    e = D2xx()
    e.SpiSetPowerState(False)
    time.sleep(2.5)
    e.SpiSetPowerState(True)

def getD2xxStatus(e):
    logString(e.FtdiGetStatus())
    logString(e.SpiGetStatus())
    logString(e.Modbus1GetStatus())
    logString(e.Modbus2GetStatus())

def SwitchToGateway(ser):
    logHeader("Перевод в режим шлюза")
    message = b'\xF0\x10\x23\x96\x00\x01\x02\x01\x00\xA1\xA0'
    try:
        ser.write(message)
    except Exception as e:
        logString(e)
        ser.read(100)
        return logResult("FAIL")
    ser.read(100)
    return logResult("OK")

def Test():
    e = D2xx()
    getD2xxStatus(e)
    e.SpiSetPowerState(True)
    time.sleep(3)
#    ser = GetPort()
#    SwitchToGateway(ser)
    TestITwoC(e, module)
    ModbusSetSlave(e, 4)
    TestModbus(e, 2, 10)
    TestModbus(e, 1, 10)
#    TestUSB(e, ser)
#    TurnOnDHCP(e, ser)
#    ip = GetIP(ser)
#    TestEthernet(ip)

#    ser.close()
    time.sleep(1)
#    e.SpiSetPowerState(False)



#==================================test==================================
Test()
#setPowerState(False)
#time.sleep(5)
#setPowerState(True)
#ResetPower()
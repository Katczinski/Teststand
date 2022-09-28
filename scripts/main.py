from libd2xx import *
from I2C     import *
from modbus  import *
from usb     import *
from logger  import *

module = sys.argv[1]    # 4
modif = sys.argv[2]     # 4

#============================definitions=================================

def getD2xxStatus(e):
    logString(e.FtdiGetStatus())
    logString(e.SpiGetStatus())
    logString(e.Modbus1GetStatus())
    logString(e.Modbus2GetStatus())

def Test():
    e = D2xx()
    getD2xxStatus(e)
    e.SpiSetPowerState(True)
    time.sleep(3)
#    TestITwoC(e, module)
#    ModbusSetSlave(e, 4)
#    TestModbus(e, 2, 10)
#    TestModbus(e, 1, 10)
#    TestUSB(e)
#    time.sleep(1)
    e.SpiSetPowerState(False)

def setPowerState(state):
    e = D2xx()
    e.SpiSetPowerState(state)

def ResetPower():
    e = D2xx()
    e.SpiSetPowerState(False)
    time.sleep(2.5)
    e.SpiSetPowerState(True)

#==================================test==================================
Test()
#setPowerState(False)
#setPowerState(True)
#ResetPower()
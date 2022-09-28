from libd2xx import *
from I2C     import *
from modbus  import *
from usb     import *

module = sys.argv[1]   # 4
modif = sys.argv[2]    # 4
    
#============================definitions=================================
     
def Test():
    e = D2xx()
    e.SpiSetPowerState(True)
    time.sleep(3)
    print(TestITwoC(e, module))
    ModbusSetSlave(e, 4)
    print(TestModbus(e, 2, 10))
    print(TestModbus(e, 1, 10))
    print(TestUSB(e))
    time.sleep(1)
    e.SpiSetPowerState(False)

def setPowerState(state):
    e = D2xx()
    e.SpiSetPowerState(state)

def Reset():
    e = D2xx()
    e.SpiSetPowerState(False)
    time.sleep(2.5)
    e.SpiSetPowerState(True)

#===============================test=====================================
Test()
#setPowerState(False)
#setPowerState(True)
#Reset()
from logger import *
from modbus import *
from libd2xx import *
import serial.tools.list_ports
import time

def TestRs():
    e = D2xx()
    e.SpiSetPowerState(True)
#    time.sleep(3)
    
    ModbusSetSlave(e, 33)
    out = ByteArray()
    e.Modbus2ReadMessage(out)
    print("received: " + toString(out.toHex(' ').toUpper()))
#    return logResult("OK")
#TestRs()
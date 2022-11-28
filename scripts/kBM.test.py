from libd2xx import *
from logger import *
from utils import *
import time

e = D2xx()
logString(e.FtdiGetStatus())
logString(e.SpiGetStatus())
logString(e.Modbus1GetStatus())
logString(e.Modbus2GetStatus())
e.SpiSetPowerState(True)

def ModbusSend(num, cmd):
    message = ByteArray(cmd, 6)
    e.Modbus1Purge()
    e.Modbus2Purge()
    if num == 1:
        e.Modbus1Send(message)
    else:
        e.Modbus2Send(message)
    

def ModbusRead(num):
    if num == 1:
        out = ByteArray()
        e.Modbus1ReadMessage(out)
        logString(toString(out.toHex(' ').toUpper()))
    else:
        out = ByteArray()
        e.Modbus2ReadMessage(out)
        logString(toString(out.toHex(' ').toUpper()))

command = b'\x02\x04\x06\xAE\x00\x03'
e.Modbus1SetBaudRate(115200)
e.Modbus2SetBaudRate(115200)
e.Modbus1SetSlave(chr(2))
e.Modbus2SetSlave(chr(2))
time.sleep(2)

ModbusSend(2, command)
#time.sleep(0.05)
ModbusRead(1)
time.sleep(0.1)
ModbusRead(1)
time.sleep(0.1)
ModbusRead(1)
e.SpiSetPowerState(False)
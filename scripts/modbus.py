from libd2xx import *

def ModbusSetSlave(e, addr):
    if addr == 1:
        addr = 2 #base module addr is 2 instead of default 1
    elif addr == 4:
        addr = 240
    e.Modbus1SetSlave(chr(addr))
    e.Modbus2SetSlave(chr(addr))

def TestModbus(e, modbus, tests):
    tests_ok = 0
    if modbus == 1:
        for x in range(1, tests + 1):
            if e.Modbus1KomegaTestController():
                tests_ok += 1
    elif modbus == 2:
        for x in range(1, tests + 1):
            if e.Modbus2KomegaTestController():
                tests_ok += 1
    e.Modbus1Purge()
    e.Modbus2Purge()
    if tests_ok == tests:
        return "Modbus" + str(modbus) + " is ok"
    else:
        return "Modbus" + str(modbus) + " failed"
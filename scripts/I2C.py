import time
import sys
from libd2xx import *
from logger  import *

def TestISquareC(e, type):
    logHeader("Проверка I2C")
    addr = chr(120)
    if type == 1:
        addr = chr(2)
        type = 2
    read_modbus_address = chr(35)
    set_i2c_address = chr(36)
    set_i2c_addr_cmd = set_i2c_address + chr(1) + addr

    e.ResetI2c()
    time.sleep(0.3)
    read = e.SpiReadI2c(set_i2c_address, 3, I2C2, addr)
    if ord(read[0]) != 0x11 or ord(read[1]) != 0x22 or ord(read[2]) != 0x33:
        logString("Нет доступа к левому I2C (проверьте подключение)")
        return  logResult("FAIL")
    time.sleep(0.02)
    array = ByteArray(set_i2c_addr_cmd)
    e.SpiWriteI2c(array, I2C2, ord(addr))
    time.sleep(0.3)
    read = e.SpiReadI2c(set_i2c_address, 3, I2C1, addr)
    if ord(read[0]) != 0x11 or ord(read[1]) != 0x22 or ord(read[2]) != 0x33:
        logString("Нет доступа к правому I2C (проверьте подключение или гейт)")
        return logResult("FAIL")
    return logResult("OK")
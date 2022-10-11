from libd2xx import *
from logger  import *
from serial_port import *
import time

def ModbusSetSlave(e, addr):
    if addr == 1:
        addr = 2 #base module addr is 2 instead of default 1
    elif addr == 4:
        addr = 240
    e.Modbus1SetSlave(chr(addr))
    e.Modbus2SetSlave(chr(addr))

def TestModbus(e, sock):
    logHeader("Проверка Modbus")
    command = b'\x00\x38\x00\x00\x00\x06\x10\x04\x06\xA4\x00\x03'
    try:
        sock.send(command)
        res = ' '.join(format(x, '02x') for x in command)
#        logString("sent: " + str(res))
        ModbusSetSlave(e, 16)
        out = ByteArray()
        e.Modbus2ReadMessage(out)
#        logString('read: ' + toString(out.toHex(' ').toUpper()))
        cmd = "\x10\x04\x06\x01\x02\x03\x04\x05\x06"
        message = ByteArray(cmd)
        e.Modbus2Send(message)
        res = ' '.join(format(x, '02x') for x in str.encode(cmd))
#        logString("sent: " + str(res))
        sock.settimeout(3)
        data = sock.recv(1024)
        res = ' '.join(format(x, '02x') for x in data)
        print("read: " + str(res))
        if data[6:] == str.encode(cmd) :
            return logResult("OK")
        else:
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        return logResult("FAIL")
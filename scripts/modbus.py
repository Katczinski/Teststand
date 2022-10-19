from libd2xx import *
from logger  import *
from serial_port import *
import time

def ModbusSetSlave(e, addr):
#    if addr == 1:
#        addr = 2 #base module addr is 2 instead of default 1
#    elif addr == 4:
#        addr = 240
    e.Modbus1SetSlave(chr(addr))
    e.Modbus2SetSlave(chr(addr))

def TestModbus1(e, sock):
    logHeader("Проверка Modbus 1")
    command = b'\x00\x38\x00\x00\x00\x06\x10\x04\x06\xA4\x00\x03'
    cmd = "\x01\x04\x06\x01\x02\x03\x04\x05\x06"
    try:
        e.Modbus1Purge()
        e.Modbus1SetBaudRate(115200)
        e.Modbus1SetSlave(chr(1))
            
        out = ByteArray()
        message = ByteArray(cmd)
        
        sock.send(command)
        time.sleep(0.5)
#        res = ' '.join(format(x, '02x') for x in command)
#        logString("sent: " + str(res))
        
        e.Modbus1ReadMessage(out)
#        logString("read: " + toString(out.toHex(' ').toUpper()))
        if toString(out.toHex(' ').toUpper()) != "10 04 06 A4 00 03 F2 21":    # shouldn't it be 01 instead of 10?
            logString("Ожидалось: 10 04 06 A4 00 03 F2 21")
            logString("Получено: " + toString(out.toHex(' ').toUpper()))
            return logResult("FAIL")
        e.Modbus1Send(message)
#        res = ' '.join(format(x, '02x') for x in str.encode(cmd))
#        logString("sent: " + str(res))
        
        data = sock.recv(15)
        res = ' '.join(format(x, '02x') for x in data[6:])
#        logString("read: " + str(res))
        time.sleep(1)
        
        if data[6:] == b'\x10\x04\x06\x01\x02\x03\x04\x05\x06' :
            return logResult("OK")
        else:
            logString("Ожидалось: 10 04 06 01 02 03 04 05 06")
            logString("Получено: " + res)
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        return logResult("FAIL")
        
def TestModbus2(e, sock):
    logHeader("Проверка Modbus 2")
    command = b'\x00\x38\x00\x00\x00\x06\x10\x04\x06\xA4\x00\x03'
    cmd = "\x01\x04\x06\x01\x02\x03\x04\x05\x06"
    try:
        e.Modbus2Purge()
        e.Modbus2SetBaudRate(115200)
        e.Modbus2SetSlave(chr(1))
            
        out = ByteArray()
        message = ByteArray(cmd)
        
        sock.send(command)
        time.sleep(0.5)
#        res = ' '.join(format(x, '02x') for x in command)
#        logString("sent: " + str(res))
        
        e.Modbus2ReadMessage(out)
#        logString("read: " + toString(out.toHex(' ').toUpper()))
        if toString(out.toHex(' ').toUpper()) != "10 04 06 A4 00 03 F2 21":    # shouldn't it be 01 instead of 10?
            logString("Ожидалось: 10 04 06 A4 00 03 F2 21")
            logString("Получено: " + toString(out.toHex(' ').toUpper()))
            return logResult("FAIL")
        e.Modbus2Send(message)
#        res = ' '.join(format(x, '02x') for x in str.encode(cmd))
#        logString("sent: " + str(res))
        
        data = sock.recv(15)
        res = ' '.join(format(x, '02x') for x in data[6:])
#        logString("read: " + str(res))
        time.sleep(1)
        
        if data[6:] == b'\x10\x04\x06\x01\x02\x03\x04\x05\x06' :
            return logResult("OK")
        else:
            logString("Ожидалось: 10 04 06 01 02 03 04 05 06")
            logString("Получено: " + res)
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        return logResult("FAIL")
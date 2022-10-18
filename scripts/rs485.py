from logger import *
from modbus import *
from libd2xx import *
import serial.tools.list_ports
import time
import socket

def TestLeftRs(e, sock):
    logHeader("Проверка RS485 (Левый нижний)")
    command = b'\x00\x38\x00\x00\x00\x06\x21\x04\x06\xA4\x00\x03'
    cmd = "\x01\x04\x06\x01\x02\x03\x04\x05\x06"
    try:
        e.Modbus2Purge()
        e.Modbus2SetBaudRate(9600)
        e.Modbus2SetSlave(chr(1))
            
        out = ByteArray(chr(0), 1)
        message = ByteArray(cmd)
        
        sock.send(command)
        time.sleep(0.5)
#        res = ' '.join(format(x, '02x') for x in command)
#        logString("sent: " + str(res))
        
        e.Modbus2ReadMessage(out)
        logString("read: " + toString(out.toHex(' ').toUpper()))
#        TODO: compare to '\x01\x04\x06\xA4\x00\x03\xF1\x60'

        e.Modbus2Send(message)
#        res = ' '.join(format(x, '02x') for x in str.encode(cmd))
#        logString("sent: " + str(res))
        
        data = sock.recv(15)
        res = ' '.join(format(x, '02x') for x in data)
        logString("read: " + str(res))
        time.sleep(1)
        
        if data[6:] == b'\x21\x04\x06\x01\x02\x03\x04\x05\x06' :
            return logResult("OK")
        else:
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        return logResult("FAIL")
        
def TestRightRs(e, sock):
    logHeader("Проверка RS485 (Правый нижний)")
    command = b'\x00\x38\x00\x00\x00\x06\x41\x04\x06\xA4\x00\x03'
    cmd = "\x01\x04\x06\x01\x02\x03\x04\x05\x06"
    try:
        e.Modbus1Purge()
        e.Modbus1SetBaudRate(9600)
        e.Modbus1SetSlave(chr(1))
            
        out = ByteArray(chr(0), 1)
        message = ByteArray(cmd)
        
        sock.send(command)
        time.sleep(0.5)
#        res = ' '.join(format(x, '02x') for x in command)
#        logString("sent: " + str(res))
        
        e.Modbus1ReadMessage(out)
        logString("read: " + toString(out.toHex(' ').toUpper()))
#        TODO: compare to '\x01\x04\x06\xA4\x00\x03\xF1\x60'

        e.Modbus1Send(message)
#        res = ' '.join(format(x, '02x') for x in str.encode(cmd))
#        logString("sent: " + str(res))
        
        data = sock.recv(15)
        res = ' '.join(format(x, '02x') for x in data)
        logString("read: " + str(res))
        time.sleep(1)
        if data[6:] == b'\x41\x04\x06\x01\x02\x03\x04\x05\x06' :
            return logResult("OK")
        else:
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        return logResult("FAIL")
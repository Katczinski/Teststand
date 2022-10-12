from logger import *
from modbus import *
from libd2xx import *
import serial.tools.list_ports
import time
import socket

def TestLeftRs(e, sock):
    logHeader("Проверка RS485 (Левый нижний)")
    command = b'\x00\x38\x00\x00\x00\x06\x21\x04\x06\xA4\x00\x03'
    try:
        sock.settimeout(3)
        e.Modbus2Purge()
        sock.send(command)
        res = ' '.join(format(x, '02x') for x in command)
        logString("sent: " + str(res))
        e.Modbus2SetSlave(chr(33))
        out = ByteArray(chr(0), 1)
        e.Modbus2ReadMessage(out)
        logString('read: ' + toString(out.toHex(' ').toUpper()))
        cmd = "\x21\x04\x06\x01\x02\x03\x04\x05\x06"
        message = ByteArray(cmd)
        print("sending " + str(message.size()))
        e.Modbus2Send(message)
        res = ' '.join(format(x, '02x') for x in str.encode(cmd))
        logString("sent: " + str(res))
        data  = sock.recv(1024)        
        res = ' '.join(format(x, '02x') for x in data)
        logString("read: " + str(res))
        if data[6:] == str.encode(cmd) :
            return logResult("OK")
        else:
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        return logResult("FAIL")
        
def TestRightRs(e, sock):
    logHeader("Проверка RS485 (Правый нижний)")
    command = b'\x00\x38\x00\x00\x00\x06\x41\x04\x06\xA4\x00\x03'
    try:
        sock.settimeout(3)
        e.Modbus1Purge()
        sock.send(command)
        res = ' '.join(format(x, '02x') for x in command)
        logString("sent: " + str(res))
        e.Modbus1SetSlave(chr(65))
        out = ByteArray(chr(0), 1)
        e.Modbus1ReadMessage(out)
        logString("read: " + toString(out.toHex(' ').toUpper()))
        cmd = "\x41\x04\x06\x01\x02\x03\x04\x05\x06"
        message = ByteArray(cmd)
        print("sending " + str(message.size()))
        e.Modbus1Send(message)
        res = ' '.join(format(x, '02x') for x in str.encode(cmd))
        logString("sent: " + str(res))
        data = sock.recv(1024)
        res = ' '.join(format(x, '02x') for x in data)
        logString("read: " + str(res))
        if data[6:] == str.encode(cmd) :
            return logResult("OK")
        else:
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        return logResult("FAIL")
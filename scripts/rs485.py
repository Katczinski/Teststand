from logger import *
from modbus import *
from libd2xx import *
import serial.tools.list_ports
import time
import socket

def TestRs(e, sock):
    logHeader("Проверка RS485 (Левый нижний)")
    command = b'\x00\x38\x00\x00\x00\x06\x21\x04\x06\xA4\x00\x03'
    try:
        sock.send(command)
        res = ' '.join(format(x, '02x') for x in command)
        logString("sent: " + str(res))
        ModbusSetSlave(e, 33)
        out = ByteArray()
        e.Modbus2ReadMessage(out)
        logString('read: ' + toString(out.toHex(' ').toUpper()))
        cmd = "\x21\x04\x06\x01\x02\x03\x04\x05\x06"
        crc = " 43 94"
        message = ByteArray(cmd)
        e.Modbus2Send(message)
        res = ' '.join(format(x, '02x') for x in str.encode(cmd))
        logString("sent: " + str(res) + crc)
        sock.settimeout(3)
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
from logger   import *
from modbus   import *
from libd2xx  import *
from ethernet import *
import serial.tools.list_ports
import time
import socket

def TestLeftRs(e, ip):
    logHeader("Проверка RS485 (Левый нижний)")
    command = b'\x00\x38\x00\x00\x00\x06\x21\x04\x06\xA4\x00\x03'
    cmd = "\x01\x04\x06\x01\x02\x03\x04\x05\x06"
    
    sock = GetSocket(ip)
    try:
        e.Modbus1Purge()
        e.Modbus1SetBaudRate(9600)
        e.Modbus1SetSlave(chr(1))
            
        out = ByteArray()
        message = ByteArray(cmd)
        
        sock.send(command)
        time.sleep(0.5)
        res = ' '.join(format(x, '02x') for x in command)
        logString("sent: " + str(res))
        
        e.Modbus1ReadMessage(out)
        logString("read: " + toString(out.toHex(' ').toUpper()))
        if toString(out.toHex(' ').toUpper()) != "01 04 06 A4 00 03 F1 60":
            logString("Ожидалось: 01 04 06 A4 00 03 F1 60")
            logString("Получено: " + toString(out.toHex(' ').toUpper()))
            return logResult("FAIL")
        e.Modbus1Send(message)
        res = ' '.join(format(x, '02x') for x in str.encode(cmd))
        logString("sent: " + str(res))
        
        data = sock.recv(15)
        sock.close()
        res = ' '.join(format(x, '02x') for x in data[6:])
        logString("read: " + str(res))
        time.sleep(1)
        
        if data[6:] == b'\x21\x04\x06\x01\x02\x03\x04\x05\x06' :
            return logResult("OK")
        else:
            logString("Ожидалось: 21 04 06 01 02 03 04 05 06")
            logString("Получено: " + res)
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        return logResult("FAIL")
        
def TestRightRs(e, ip):
    logHeader("Проверка RS485 (Правый нижний)")
    command = b'\x00\x38\x00\x00\x00\x06\x41\x04\x06\xA4\x00\x03'
    cmd = "\x01\x04\x06\x01\x02\x03\x04\x05\x06"
    
    sock = GetSocket(ip)
    try:
        e.Modbus2Purge()
        e.Modbus2SetBaudRate(9600)
        e.Modbus2SetSlave(chr(1))
            
        out = ByteArray()
        message = ByteArray(cmd)
        
        sock.send(command)
        time.sleep(0.5)
        res = ' '.join(format(x, '02x') for x in command)
        logString("sent: " + str(res))
        
        e.Modbus2ReadMessage(out)
        logString("read: " + toString(out.toHex(' ').toUpper()))
        if toString(out.toHex(' ').toUpper()) != "01 04 06 A4 00 03 F1 60":
            logString("Ожидалось: 01 04 06 A4 00 03 F1 60")
            logString("Получено: " + toString(out.toHex(' ').toUpper()))
            return logResult("FAIL")
        e.Modbus2Send(message)
        res = ' '.join(format(x, '02x') for x in str.encode(cmd))
        logString("sent: " + str(res))
        
        data = sock.recv(15)
        sock.close()
        res = ' '.join(format(x, '02x') for x in data[6:])
        logString("read: " + str(res))
        time.sleep(1)
        if data[6:] == b'\x41\x04\x06\x01\x02\x03\x04\x05\x06' :
            return logResult("OK")
        else:
            logString("Ожидалось: 21 04 06 01 02 03 04 05 06")
            logString("Получено: " + res)
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        return logResult("FAIL")
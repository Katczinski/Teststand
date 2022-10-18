from libd2xx import *
from logger  import *
from serial_port import *
import time

def TestUart(e, sock):
    logHeader("Проверка Uart")
    command = b'\x00\x38\x00\x00\x00\x06\x61\x04\x06\xA4\x00\x03'
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
        
        if data[6:] == b'\x61\x04\x06\x01\x02\x03\x04\x05\x06' :
            return logResult("OK")
        else:
            return logResult("FAIL")
    except Exception as ex:
        logString(ex)
        return logResult("FAIL")
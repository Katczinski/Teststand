from libd2xx  import *
from I2C      import *
from modbus   import *
from usb      import *
from logger   import *
from ethernet import *
from rs485    import *
from serial_port import *

module = sys.argv[1]    # 4
modif = sys.argv[2]     # 4

#============================definitions=================================

def setPowerState(state):
    e = D2xx()
    e.SpiSetPowerState(state)

def ResetPower():
    e = D2xx()
    e.SpiSetPowerState(False)
    time.sleep(2.5)
    e.SpiSetPowerState(True)

def getD2xxStatus(e):
    logString(e.FtdiGetStatus())
    logString(e.SpiGetStatus())
    logString(e.Modbus1GetStatus())
    logString(e.Modbus2GetStatus())

def SwitchMode(ser, mode):
    if mode:
        logHeader("Перевод в режим шлюза")
        message = b'\xF0\x10\x23\x96\x00\x01\x02\x01\x00\xA1\xA0'
    else:
        logHeader("Перевод в режим исполнения алгоритма")
        message = b'\xF0\x10\x23\x96\x00\x01\x02\x00\x00 A0\x30'
    try:
        ser.write(message)
    except Exception as e:
        logString(e)
        return logResult("FAIL")
    ser.read(100)
    return logResult("OK")

def Test():
#    e = D2xx()
#    getD2xxStatus(e)
#    e.SpiSetPowerState(True)
#    time.sleep(3)
    
#    command = b'\x00\x01\xF0\x24'
#    ser = GetPort()
    TCP_IP = '192.168.0.109'
    TCP_PORT = 502
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((TCP_IP, TCP_PORT))
    sock.settimeout(3)
#    command = b'\x00\x38\x00\x00\x00\x06\x21\x04\x06\xA4\x00\x03'
    command = b'\x00\x38\x00\x00\x00\x06\x10\x04\x06\xA4\x00\x03'
    sock.send(command)
#    SwitchMode(ser, True)  
#    ModbusSetSlave(e, 4)

#    TestUSB(e, ser)
#    TurnOnDHCP(e, ser)
#    ip = GetIP(ser)
#    ip = '192.168.0.109'
#    sock = TestEthernet(ip)
#    TestITwoC(e, module)
#    TestModbus(e, sock) 
#    TestRs(e, sock)
#    SwitchMode(ser, False)
#    ser.close()
    sock.close()
#    time.sleep(1)
#    e.SpiSetPowerState(False)



#==================================test==================================
Test()
#setPowerState(False)
#time.sleep(5)
#setPowerState(True)
#ResetPower()
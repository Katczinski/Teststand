from libd2xx      import *
from I2C          import *
from modbus       import *
from usb          import *
from logger       import *
from ethernet     import *
from rs485        import *
from serial_port  import *
from uart         import *
from clock        import *
import time

if len(sys.argv) > 2:
    module = sys.argv[1]    # 4
    modif  = sys.argv[2]    # 4
else:
    module = 4
    modif = 4
#============================definitions=================================

def PrintBytearray(array):
    res = ' '.join(format(x, '02x') for x in array)
    logString("array: " + str(res))

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
    ser.read(100)

def Test():
    e = D2xx()
    getD2xxStatus(e)
    e.SpiSetPowerState(True)
    time.sleep(1)
    ser = GetPort()
    try:
#        SwitchMode(ser, True)  
#        TestUSB(e, ser)
#==================Get ip and socket dynamically==================
#        TurnOnDHCP(e, ser)
#        ip = GetIP(ser)
#        sock = TestEthernet(ip) #'192.168.0.111'
#=========================Use custom ip===========================
        TCP_IP = '192.168.0.114'
        TCP_PORT = 502
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((TCP_IP, TCP_PORT))
        sock.settimeout(1.0)
#=================================================================
#        TestUart(e, sock)
#        TestISquareC(e, module)
#        TestModbus(e, sock)
#        TestLeftRs(e, sock)
#        TestRightRs(e, sock)
        TestClock(e, ser)
        sock.close()
    except Exception as ex:
        logString(ex)
    finally:
#        SwitchMode(ser, False)
        ser.close()
#        e.SpiSetPowerState(False)

#==================================main==================================
#ResetPower()
Test()
#setPowerState(False)
#setPowerState(True)

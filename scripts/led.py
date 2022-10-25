from logger import *
import time

def TestGreenLed(ser):
    logHeader("Проверка зеленого светодиода")
    
    ledOn = b'\xF0\x10\x25\x81\x00\x01\x02\x00\x01\x04\xD7'
    ledOff = b'\xF0\x10\x25\x81\x00\x01\x02\x00\x02\x44\xD6'
    ledAuto = b'\xF0\x10\x25\x81\x00\x01\x02\x00\x00\xC5\x17'
    
    logString("Зеленый светодиод включен")
    ser.write(ledOn)
    time.sleep(5)
    ser.read(100)
    
    logString("Зеленый светодиод выключен")
    ser.write(ledOff)
    time.sleep(5)
    ser.read(100)
    
    logString("Зеленый светодиод в автоматическом режиме")
    ser.write(ledAuto)
    time.sleep(5)
    ser.read(100)
    
def TestRedLed(ser):
    logHeader("Проверка красного светодиода")
    
    ledOn = b'\xF0\x10\x25\x80\x00\x01\x02\x00\x01\x05\x06'
    ledOff = b'\xF0\x10\x25\x80\x00\x01\x02\x00\x02\x45\x07'
    ledAuto = b'\xF0\x10\x25\x80\x00\x01\x02\x00\x00\xC4\xC6'
    
    logString("Красный светодиод включен")
    ser.write(ledOn)
    time.sleep(5)
    ser.read(100)
    
    logString("Красный светодиод выключен")
    ser.write(ledOff)
    time.sleep(5)
    ser.read(100)
    
    logString("Красный светодиод в автоматическом режиме")
    ser.write(ledAuto)
    time.sleep(5)
    ser.read(100)
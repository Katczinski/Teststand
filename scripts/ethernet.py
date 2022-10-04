import socket
from logger import *

def TestEthernet():
    logHeader("Проверка Ehternet")
    TCP_IP = '192.168.0.254'
    TCP_PORT = 502
    BUFFER_SIZE = 1024
    MESSAGE = '00 02 00 12 00 13 00 10 00 15 00 95'
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        logString("Подключение к " + TCP_IP)
        s.connect((TCP_IP, TCP_PORT))
        s.send(MESSAGE)
        data = s.recv(BUFFER_SIZE)
        s.close()
        print("Получен ответ " + data)
        logResult("OK")
    except:
        logString("Не удалось подключиться")
        logResult("FAIL")

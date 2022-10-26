from logger  import *
from libd2xx import *
import time

def TestReset(e):
    logHeader("Проверка линии reset")
    e.SpiDioDirection(9, Direction_in)
    reset = e.SpiDiRead(9)
    if reset:
        logResult("OK")
    else:
        logResult("FAIL")
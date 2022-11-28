from logger import *

def PrintBytearray(array):
    res = ' '.join(format(x, '02x') for x in array)
    logString("array: " + str(res))
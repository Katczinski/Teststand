#ifndef FTDI_IO_H
#define FTDI_IO_H
#include "ftd2xx.h"
#include "utils.h"

class Ftdi_Io
{
public:
    Ftdi_Io(int dev);
    void initSequence(int dev);
    void SetIoLevel(bool is_5v);
    void ResetI2c();
    bool GetInitStatus(){return ftStatus == FT_OK;}
private:
    void SetIO(int num, int value);
    int GetIO(int num);
    int dev;
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    unsigned int sizeToTransfer;
    DWORD sizeTransfered;

    uint8_t     out_state = 0;
    uint8_t     in_state = 0;
    uint8_t     bitmode = 0;
};

#endif // FTDI_IO_H

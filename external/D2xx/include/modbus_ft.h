#ifndef MODBUS_FT_H
#define MODBUS_FT_H

#include "utils.h"
#include "modbus_helpers.h"
#include "ftd2xx.h"

class Modbus_ft
{
private:
    ByteArray   buf;
    DWORD       temp;
    FT_STATUS   ftStatus;               // Result of each D2XX call
    FT_HANDLE   ftHandle;               // Handle of the FTDI device
    DWORD       numDevs;
    BYTE        byOutputBuffer[8];      // Buffer to hold MPSSE commands and data to be sent to the FT2232H
    BYTE        byInputBuffer[255];     // Buffer to hold data read from the FT2232H
    DWORD       dwCount = 0;            // General loop index
    DWORD       dwNumBytesToSend = 0;   // Index to the output buffer
    DWORD       dwNumBytesSent = 0;     // Count of actual bytes sent - used with FT_Write
    DWORD       dwNumBytesToRead = 0;   // Number of bytes available to read in the driver's input buffer
    DWORD       dwNumBytesRead = 0;     // Count of actual bytes read - used with FT_Read
public:
    char        slave_addr;

    Modbus_ft(int num);
    ~Modbus_ft();
    bool        reinit(int num);
    bool        initSequence();
    bool        Fc2(int addr, std::vector<bool> &values);
    bool        Fc16(uint16_t addr, std::vector<uint16_t> values);
    bool        Fc16(uint16_t addr, uint16_t value);
    bool        Fc15(int addr, std::vector<bool> values);
    bool        Fc3(uint16_t addr, std::vector<uint16_t>& values);
    bool        Fc3(uint16_t addr, std::vector<float>& values);
    bool        Fc4(uint16_t addr, std::vector<uint16_t>& values);
    bool        Fc4(uint16_t addr, std::vector<float>& values);
    bool        Send(ByteArray &array);
    bool        Read(ByteArray &data);
    double      ReadAi(int num, bool &is_ok);
    bool        ReadDi(int num, bool& is_ok);
    double      ReadCoef(int type, bool &is_ok);
    void        Purge();
    bool        GetInitStatus() { return ftStatus == FT_OK; }
    bool        KomegaTestController();
};

#endif // MODBUS_FT_H

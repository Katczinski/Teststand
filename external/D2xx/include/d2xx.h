#pragma once
#ifndef D2XX_H
#define D2XX_H

#include <list>
#include <iostream>
#include "spi.h"
#include "modbus.h"
#include "ftdi_io.h"

struct FT_HANDLES {
    FT_HANDLE swd;
    FT_HANDLE spi;
    FT_HANDLE uart1;
    FT_HANDLE uart2;
};

class D2xx {
public:
    void            init();
    void            reinit();
    std::string     GetInitStatus();
//======================================spi======================================
    int     SpiDacWriteCalibratedValue(double value, bool is_I, bool is_first);
    int     SpiAdcSetRate(int rate_index, bool is_chop);
    double  SpiAdcReadCalibrated(int type, int sysGain = 1);
    double  SpiAdcReadCalibrated(CalibrationCoefficientsTypes type, int sysGain = 1);
    double  SpiAdcReadRoughIDAC(int gain = 1);
    double  SpiAdcReadRough(bool is_I, int gain = 1);
    double  SpiAdcReadVSensCalibrated();
//            SpiAdcReadVSens TODO
//            SpiAdcReadISens
//            Spi_ftIoReadReg16
//            Spi_ftIsGPIOInt
    double  SpiAdcReadISensCalibrated();
    double  SpiAdcReadAverage(const int channel, const ADC_TYPES type, const int averageBy, const int gain = 1);
    double  SpiAdcCalculateAverageRout(const int channel, const int averaging, RCORR_GAIN& gain, bool& timeoutOccured);
    double  SpiAdcCalculateAverageRin(const int channel, const int averaging, RCORR_GAIN& gain, bool& timeoutOccured);
    bool    SpiAdcSetGain(int gain);
    bool    SpiGetPowerDetectorState();
    bool    SpiDiRead(int index);
    bool    SpiGetPowerState();
    void    SpiAdcSetDefaultRate();
    void    SpiRdacWriteQuants(uint16 data, bool is_second);
    void    SpiAdcSetInputs(AIN positive, AIN negative);
    void    SpiAdcSetChannel(int index, ADC_TYPES type, int gain = 0);
    void    SpiAdcSaveRate(int rate_index, bool is_chop);
    void    SpiAdcRestoreRate();
    void    SpiPullupActivate(int num);
    void    SpiPullupDeactivate(int num);
    void    SpiDacSetType(int chan, bool is_I);
    void    SpiDacSetChannel(int channel);
    void    SpiDacSwitchOff(bool is_first);
    void    SpiRdacWriteCalibratedValue(double Rout, int type = -1);
    void    SpiRdacMuxControl(bool is_on1, bool is_on2);
    void    SpiRdacSetAB(bool is_b);
    void    SpiRdacSetChan(int chan);
    void    SpiSetPowerState(bool state);
    void    SpiCheckPower();
    void    SpiDioWrite(int index, bool value);
    void    SpiDioDirection(int index, Direction dir);   
//=====================================FT4232====================================
    double  FT4232DacCalculateRange(double& min, bool is_I, bool is_first);
    double  FT4232RdacCalculateRange(int range_num, double& min, double& max);
    double  FT4232RdacCalculateRange(double Rout);
//=====================================Modbus2===================================
    bool    Modbus2Fc4(uint16_t addr, std::vector<uint16_t>& values);
    bool    Modbus2Fc4(uint16_t addr, std::vector<float>& values);
    bool    Modbus2Fc16(uint16_t addr, std::vector<uint16_t> values);
    bool    Modbus2Fc16(uint16_t addr, uint16_t value);
    bool    Modbus2Fc15(int addr, std::vector<bool> values);
    bool    Modbus2WritePo(int num, bool is_on);
    bool    Modbus2WriteDo(int num, bool is_on);
    bool    Modbus2WriteAo(int num, double value);
    bool    Modbus2WriteCoef(int type, double value);
    bool    Modbus2SaveCoefs();
    bool    Modbus2CfgKomegaIO(int num, KomegaPinMode mode, bool is_in, int shift);
    bool    Modbus2CfgKomegaIO(std::vector<double>& pins, KomegaPinMode mode, bool is_in, int shift);
    bool    Modbus2KomegaRetainWrite(ByteArray value, int index = 0);
    bool    Modbus2KomegaRetainRead(ByteArray& value, int index = 0);
    bool    Modbus2KomegaWriteRetainNumAndGuid(int retainCount, ByteArray GUID);
    bool    Modbus2KomegaReadRetainNumAndGuid(int& retainCount, ByteArray& GUID);
    bool    Modbus2KomegaWriteTime(std::vector<uint16_t> &dateTime);
    bool    Modbus2KomegaReadTime(std::vector<uint16_t> &dateTime);
    bool    Modbus2SetContinuousMode(int num, int samples_per_second = 0xF);
    bool    Modbus2CfgKomegaDIO(int num, bool is_in);
    bool    Modbus2CfgKomegaDIO(int num, bool is_in, int shift);
    bool    Modbus2AcceptAlarms();
    bool    Modbus2KomegaTestController();
    bool    Modbus2ReadDi(int num, bool& is_ok);
    double  Modbus2ReadAi(int num, bool& is_ok);
    double  Modbus2ReadCoef(int type, bool& is_ok);
    void    Modbus2Purge();
//=====================================Modbus1===================================
    void    Modbus1Purge();
//=============================ftdi============================
    void    setIoLevel(bool level);
public:
    D2xx();
    ~D2xx();
    // obsolete FT_STATUS InitUart(FT_HANDLE &ftHandle, int num);
    // obsolete void init(FT_HANDLE &handle, int num);

    bool is_inited = true;
    FT_HANDLES handles;
    // obsolete FT_HANDLE ftHandle; // Handle of the FTDI device
    FT_STATUS ftStatus; // Result of each D2XX call
    DWORD numDevs;
    unsigned int uiDevIndex = 0xF; // The device in the list that we'll use
    BYTE byOutputBuffer[8]; // Buffer to hold MPSSE commands and data
    // to be sent to the FT2232H
    BYTE byInputBuffer[8]; // Buffer to hold data read from the FT2232H
    DWORD dwCount = 0; // General loop index
    DWORD dwNumBytesToSend = 0; // Index to the output buffer
    DWORD dwNumBytesSent = 0; // Count of actual bytes sent - used with FT_Write
    DWORD dwNumBytesToRead = 0; // Number of bytes available to read
    DWORD dwNumBytesRead = 0; // Count of actual bytes read - used with FT_Read

    std::vector<std::string> serial_numbers{};
    Ftdi_Io* ftdi_a = nullptr;
    Spi* spi = nullptr;
    Modbus* modbus2 = nullptr;
    Modbus* modbus1 = nullptr;
};

#endif // D2XX_H



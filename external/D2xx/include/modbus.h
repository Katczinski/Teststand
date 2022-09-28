#pragma once
#ifndef MODBUS_H
#define MODBUS_H

#include <cstring>
#include <vector>
#include "utils.h"
#include "ftd2xx.h"
#include "modbus_helpers.h"
#include "komega_pin_mode.h"
#include "modbus_ft.h"

class Modbus
{
public:
    Modbus(int num);
    virtual ~Modbus();
    bool reinit(int num);
    bool WritePo(int num, bool is_on);
    bool WriteDo(int num, bool is_on);
    bool WriteAo(int num, double value);
    double ReadAi(int num, bool& is_ok);
    bool ReadDi(int num, bool& is_ok);
    double ReadCoef(int type, bool& is_ok);
    bool WriteCoef(int type, double value);
    bool SaveCoefs();
    bool AcceptAlarms();
    bool SetContinuousMode(int num, int samples_per_second = 0xF);//0xF is the reserved value, which we won't handle
    void setSlaveAddress(char addr);

    bool Fc16(uint16_t addr, std::vector<uint16_t> values);
    bool Fc16(uint16_t addr, uint16_t value);
    void Fc2(int num, std::vector<bool> values);
    bool Fc15(int addr, std::vector<bool> values);
    void Purge();
    bool Fc3(uint16_t addr, std::vector<uint16_t>& values);
    bool Fc4(uint16_t addr, std::vector<uint16_t>& values);
    bool Fc4(uint16_t addr, std::vector<float>& values);
    bool GetInitStatus() { return modbus_ft->GetInitStatus(); }

    bool ReadAi(int first, std::vector<float>& result);

    bool KomegaRetainWrite(ByteArray value, int index = 0);
    bool KomegaRetainRead(ByteArray& value, int index = 0);
    bool KomegaWriteRetainNumAndGuid(int retainCount, ByteArray GUID);
    bool KomegaReadRetainNumAndGuid(int& retainCount, ByteArray& GUID);
    bool KomegaWriteBatteryMode(bool is_ON);
    bool KomegaReadBatteryMode(bool& is_ON);
    bool KomegaWriteTime(std::vector<uint16_t> &dateTime);
    bool KomegaReadTime(std::vector<uint16_t> &dateTime);
    bool KomegaTestController();
    bool CfgKomegaDIO(int num, bool is_in);
    bool CfgKomegaAIO(int type, int num, bool is_in);
    bool CfgKomegaDIO(int num, bool is_in, int shift);
    bool CfgKomegaIO(int num, KomegaPinMode mode, bool is_in, int shift);
    bool CfgKomegaIO(std::vector<double>& pins, KomegaPinMode mode, bool is_in, int shift);

    char slave_addr;
    struct KOMEGA_SHIFT {
        int dio;
        int aio;
    }komega_shift;

private:
    ByteArray buf;
    Modbus_ft *modbus_ft;

    void AddAddr(ByteArray& array);
    void Send(ByteArray& array);
    bool Read(ByteArray& data);
    bool isNumberInBufEqualTo(int startIndex, int endIndex, ByteArray& other) const;
};

#endif // MODBUS_H


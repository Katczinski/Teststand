#pragma once
#ifndef SPI_H
#define SPI_H

#include <map>
#include <list>
#include <inttypes.h>
#include "ain.h"
#include "spi_ft.h"
#include "ftdi_io.h"
#include "calibrations.h"
#include "crc8_107_hec.h"


enum class Direction {
    none = 0,
    in = 1,
    out = 2,
};
enum class RCORR_GAIN {
    Undefined = 0,
    Vdc = 1,
    None = 2,
    Virdac = 3,
    Both = 4
};
enum class ADC_TYPES {
    unused,
    v_dc,
    v_dc_16,
    i_dc,
    v_ac,
    i_ac,
    r,
    v_i_rdac,
};

class Spi {
public:
    Spi(int dev, Ftdi_Io* ftdi_a);
    ~Spi();
    void initSequence();
    bool GetInitStatus() { return is_self_test_ok; }
    std::list<std::string> GetTestLog() { return log; }

    Spi_ft* spi_ft;
    int ADC_delay = 121;
//    QTimer* m_timer;
    std::vector<std::pair<double, int>> Sps{
        {2.5, 1201},  //0
        {5, 601},     //1
        {10, 301},    //2
        {16.66, 181}, //3
        {20, 151},    //4
        {50, 61},     //5
        {60, 51},     //6
        {100, 31},    //7
        {200, 16},    //8
        {400, 8},     //9
        {800, 5},     //10
        {1000, 4},    //11
        {2000, 2},    //12
        {4000, 1},    //13
    };
    void PullupActivate(int num);
    void PullupDeactivate(int num);
    void DioDirection(int index, Direction dir);
    void SetCalibration(int type, double offset, double gain);
    void DiMassRead();
    void AdcSetInputs(AIN positive, AIN negative);

    double AdcReadCalibrated(int type, int sysGain = 1);
    double AdcReadCalibrated(CalibrationCoefficientsTypes type, int sysGain = 1);
    double AdcReadCalibratedIDAC(int type, int sysGain = 1);
    int AdcSetRate(int rate_index, bool is_chop);

    uint8_t cfg_aio_1{ 0 };
    uint8_t mux_enable_setting{ 0x88 }; //mux Enable both
    struct MUX_ENABLED {
        bool mux1 = true;
        bool mux2 = true;
    }mux_enabled;

    struct CURRENT_RATE {
        int index;
        bool chop;
    }current_rate, default_rate;

    enum class IO1 {
        PWR_DET,
        PWR_ON,
        CFG_RDAC,
        DAC_EN_1,
        IS_I_1,
        DAC_EN_2,
        IS_I_2,
        DC_SEL_CH,
        DC_SEL_UI,
        AC_SEL_CH,
        AC_SEL_UI,
        MUX_RO_A0,
        MUX_RO_A1,
        MUX_RO_A2,
        MUX_RO_EN1,
        MUX_RO_EN2,
    };
    enum class IO2 {
        muxAO1_A0,
        muxAO1_A1,
        muxAO1_A2,
        muxAO1_EN,
        muxAO2_A0,
        muxAO2_A1,
        muxAO2_A2,
        muxAO2_EN,
        muxAI1_A0,
        muxAI1_A1,
        muxAI1_A2,
        muxAI1_EN,
        muxAI2_A0,
        muxAI2_A1,
        muxAI2_A2,
        muxAI2_EN,
    };

    uint16_t io1_value = 0;
    uint16_t io2_value = 0;

    double AdcReadRough(bool is_I, int gain = 1);
    double AdcReadRoughIDAC(int gain = 1);
    double AdcReadVSens();
    double AdcReadISens();
    double AdcReadVSensCalibrated();
    double AdcReadISensCalibrated();
    void AdcReadRdacSelfValues(const int channel, double& Vdc, double& Virdac, const int gainVdc, const int gainVirdac);
    void AdcReadResistanceMeasurementValues(const int channel, double& Vdc, double& Vimon, const int gainVdc, const int gainVimon);
    double AdcCalculateAverageRout(const int channel, const int averaging, RCORR_GAIN& gain, bool& timeoutOccured);
    double AdcCalculateAverageRin(const int channel, const int averaging, RCORR_GAIN& gain, bool& timeoutOccured);
    double AdcReadAverage(const int channel, const ADC_TYPES type, const int averageBy, const int gain = 1);
    bool AdcSetGain(int gain);
    void DacSetChannel(int channel);
    int DacWriteCalibratedValue(double value, bool is_I, bool is_first);
    void DacWriteZero(bool is_first);
    void RdacMuxControl(bool is_on1, bool is_on2);
    void RdacSetAB(bool is_b);
    void RdacWriteValue(bool is_b, double quants1, double quants2);
    void RdacWriteValue(double Rout);
    void RdacWriteCalibratedValue(double Rout, int type = -1);
    void AdcRestoreRate();
    void AdcSetDefaultRate();
    void AdcSaveRate(int rate_index, bool is_chop);
    void DacSwitchOff(bool is_first);
    void DacSwitch(int chan, bool is_on);
    void RdacSetChan(int chan);
    bool DiRead(int index);
    bool ReadIO1(Spi::IO1 pos);
    void SetIO1Value(IO1 pos, bool value);
    void SetIO2Value(IO2 pos, bool value);
    void SetPowerState(bool is_on);
    void SaveIO1();
    void SaveIO2();
    bool GetPowerDetectorState();
    bool GetPowerState();
    void PreInit();
//    void StartIOTimer();
    bool TestIo(std::list<std::string>& out);
    void SelfTest(bool (Spi::* function)(std::list<std::string>&), std::string test_device);
    void RdacFinish(Cs_device rdac);
    void AdcControlMux(bool is_mux1_enabled, bool is_mux2_enabled);
    void AdcResetFlPor();
    void DioWrite(int num, bool value);
    void AdcSetChannel(int index, ADC_TYPES type, int gain = 0);
    void DacSetType(int chan, bool is_I);
    void DacWriteQuants(double value, bool is_first);

    const int io_size = 16;
    uint16_t direction = 0;
    uint16_t in = 0;
private:
    std::list<std::string> log;
    bool was_U_last_ADC;  
    uint16_t gpio_values = 0;   
    uint16_t bits_interrupt = 0;   
    Ftdi_Io* ftdi_a;

    bool InitFlash(std::list<std::string>& out);
    bool InitIO(std::list<std::string>& out);
    bool InitADC(std::list<std::string>& out);
    void InitDAC();
    bool InitRDAC(Cs_device rdac, std::list<std::string>& out);
    bool InitI2c(Cs_device i2c, std::list<std::string>& out);

    int AdcReadBuf();
    bool InitI2c1(std::list<std::string>& out);
    bool InitI2c2(std::list<std::string>& out);
    bool InitRDAC1(std::list<std::string>& out);
    bool InitRDAC2(std::list<std::string>& out);
    bool is_self_test_ok = true;
    uint8_t I2cCalculateFreqReg(double freq, double* real_freq = nullptr);
    int WriteI2cRaw(uint8_t* buf, int size, Cs_device dev);
public:
    struct FT4232_electric {
        double DacCalculateVQuants(double Vout) const;
        double DacCalculateIQuants(double Iout) const;
        double DacCalculateRange(double& min, bool is_I, bool is_first);
        double RdacCalculateK1(int range_num) const;
        double RdacCalculateK2Quants(double Rout, int range_num) const;
        double RdacCalculateRange(int range_num, double& min, double& max);
        double RdacCalculateRange(double Rout);
        double AdcCalculateValue(double rough, bool is_I);
        double AdcCalculateIDac(const int quants);

        Calibrations*   calibrations;
        // ADC
        const int Adc_bit_resolution = 24;
        const double AdcVref = 2.5, R113 = 100, VoltageDivider = 10;
        const std::map<int, uint8_t> Adc_gain_reg = { {1, 0b11101000},{2, 0b11101001},{4, 0b11101010},{8, 0b11101011},{16, 0b11101100},
                                                 {32, 0b11101101},{64, 0b11101110},{128, 0b11101111} };// DELAY=1*tMOD, PGA=enabled


        // DAC
        const int Dac_bit_resolution = 16;
        const double DacVref = 2.5, Rimon = 560;
        const double IcopyMultiplier = 10;// XTR300 Current Copy scale: "A 1:10 copy of the output current of the OPA can be monitored at the Imon pin"
        const double Gain = 2;// DAC internal amplifier
        const double Ros = 1100;// R24 and R25 kT.TS
        const double Rgain = 10000;// R31 and R32 on kT.TS: XTR300 neighbourhood

        // RDAC
        const int Rdac_bit_resolution = 10;
        const double R46 = 100, K0 = 1.1, Rmux = 4;
        const double K1a_min = 0.004, K1a_max = 2.002;// K1 A-circuit coefficient range
        const double K1b_min = 0.2, K1b_max = 100.1;// K1 B-circuit coefficient range
        const double K2_min = 1.0083, K2_max = 5.15;// K2 coefficient range
        const int RdacRangesCount = 9, RdacBFirstRange = 6;
        const std::vector<int> Rdac1_quants = { 0, 16, 63, 183, 475, 1023, 63, 127, 383, 1023 };// -10..56600 Ohm
    } FT4232;

    struct ADC {
        int current_gain = 0;
    }adc;


    int WriteI2c(ByteArray data, Cs_device dev, int addr);
    ByteArray ReadI2c(char reg_to_read, int size, Cs_device dev, char addr);
};

#endif // SPI_H


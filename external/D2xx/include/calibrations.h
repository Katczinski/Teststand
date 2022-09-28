#ifndef Calibrations_H
#define Calibrations_H
#include <vector>
#include <map>
#include "spi_ft.h"
#include "md5.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

void ErrorMessage(std::string message);
double Absolute(double value);
struct Values
{
    double setting;// what we use to set the I/O channel
    double measured;// what we get
    double calculated = 0;// what we expect to get
};
enum class CalibrationCoefficientsTypes : int {
    unused = 0,
    InVDC1 = 1,
    InVDC1_16 = 2,
    InIDC1 = 3,
    InVAC1 = 4,
    InIAC1 = 5,
    InIDAC1L = 6,
    InIDAC1H = 7,
    InIDAC2L = 8,
    InIDAC2H = 9,
    SensV = 10,
    SensI = 11,
    OutVDC1 = 12,
    OutVDC2 = 13,
    OutIDC1 = 14,
    OutIDC2 = 15,
    OutRA1 = 16,
    OutRA2 = 17,
    OutRA3 = 18,
    OutRA4 = 19,
    OutRA5 = 20,
    OutRB1 = 21,
    OutRB2 = 22,
    OutRB3 = 23,
    OutRB4 = 24,
    OutRCorr1 = 25,
    OutRCorr2 = 26,
};

struct OffsetGain {
    std::string id;
    std::string GuiTitle;
    CalibrationCoefficientsTypes type;
    double offset;
    double gain;
};
class Calibrations
{
    std::vector<OffsetGain*> calibrationsVector;// iterative
    std::map<std::string, OffsetGain*> calibrationsMap;// searchable by key (id)

public:
    std::string    error = "";
    Spi_ft *spi_ft;
    Calibrations(Spi_ft* spi_ft);
    ~Calibrations();
    void FillContainers();

    void Calculate(const std::vector<Values> &values, OffsetGain *type, double load = 0);
    void Calculate(const std::vector<Values> &values, int type, double load = 0);

    void AddOffsetGain(std::string id, CalibrationCoefficientsTypes type, std::string title, double offset = 0, double gain = 1);
    OffsetGain* FindOffsetGain(bool is_In, bool is_I, bool is_First);
    double Apply(double value, OffsetGain *type);
    double Apply(double value, int type);
    double ApplyReversed(double value, OffsetGain *type);
    double ApplyReversed(double value, int type);

    int GetCount();
    OffsetGain* GetOffsetGain(int type);
    OffsetGain* GetOffsetGain(std::string id);
    double GetGain(int type);
    double GetGain(std::string id);
    double GetOffset(int type);
    double GetOffset(std::string id);
    std::string GetGuiTitle(int type);
    std::string GetGuiTitle(std::string id);

    bool ReadFromFlash(ByteArray& data, bool useVendorCalibrations);
    bool WriteToFlash(ByteArray& data, bool useVendorCalibrations);
    bool ReadJson(bool useVendorCalibrations);
    bool WriteJson(bool useVendorCalibrations);

    void SetDefault(OffsetGain *type);
    void SetAllDefault();
    void ErrorMessage(std::string message);
private:
    // user data starts at 0x000000 (Sector 0 beginning), hash and size at the end of Sector 6
    const uint32 user_data_hash_addr {0x06F000};
    const uint32 user_data_size_addr {0x06F080};
    const uint32 user_data_addr {0x000000};
    // vendor data starts at 0x070000 (Sector 7 beginning), hash and size at the end of Sector 7
    const uint32 vendor_data_hash_addr {0x07F000};
    const uint32 vendor_data_size_addr {0x07F080};
    const uint32 vendor_data_addr {0x070000};

    void CalibrationsFromJson(const json& jsonObject);
    void ParseJson(const json& jsonObject);
};
#endif // Calibrations_H

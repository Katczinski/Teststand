#ifndef AIN_H
#define AIN_H
enum class AIN : unsigned char {
    TRUERMS = 0,
    DC = 1,
    I_RDACSENS = 2,
    I_PWR_SENS = 3,
    I_DACSENS_2 = 4,
    I_DACSENS_1 = 5,
    V_PWRSENS = 6,
    AGND4 = 7,
    AGND3 = 8,
    AGND2 = 9,
    AGND1 = 10,
    DC_T = 11,
    GND = 12,
};
#endif // AIN_H

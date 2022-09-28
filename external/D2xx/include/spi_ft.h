#ifndef SPI_FT_H
#define SPI_FT_H

#include <mutex>
#include "ftd2xx.h"
#include "utils.h"

#define uint8 uint8_t
#define uint16 uint16_t
#define uint32 uint32_t

struct FLASH_STATUS_REG{
    uint8 reg;
    bool is_ready;
    bool is_write_enabled;
};

enum Io_device : unsigned char { //real addresses is {0, 1, 2}. They shift one bit up to be convenient with addr command
    CFG_AIO_1 = 0, // CFG_DAC + CFG_ADC + CFG_MUX_AO_1
    CFG_AIO_2 = 2, // CFG_MUX_AO_2 + CFG_MUX_RO + CFG_MUX_AI
    CFG_DIO = 4, // IO8_1 + IO8_2
};
enum Cs_device : unsigned char {
    FLASH = 0,
    IO = 1,
    RDAC1 = 2,
    RDAC2 = 3,
    I2C1 = 4,
    I2C2 = 5,
    ADC = 6,
    DAC = 7,
};
enum Io_Regs : unsigned char {
    IO_DIR_A = 0x00, //DIRECTION, 0 = out, 1 = in
    IO_DIR_B = 0x01,
    I_POL_A =  0x02, //input polariry, 1 = reversed
    I_POL_B =  0x03,
    GP_INT_EN_A = 0x04, //interrupt enabled
    GP_INT_EN_B = 0x05,
    DEF_VAL_A = 0x06, //default value for interrupt on comp
    DEF_VAL_B = 0x07,
    INT_CON_A = 0x08, //interrupt compared with: 0 = previous, 1 = DEF_VAL
    INT_CON_B = 0x09,
    IO_CON_A = 0x0A,  //config register, b just mirrors it
    IO_CON_B = 0x0B,
    GP_PU_A = 0x0C,  //pull-up, 1 = enabled
    GP_PU_B = 0x0D,
    INT_F_A = 0x0E, //interrupt flag, 1 = occured
    INT_F_B = 0x0F,
    INT_CAP_A = 0x10, //captured state of port during interrupt
    INT_CAP_B = 0x11,
    GPIO_A = 0x12, // state, 1 = high
    GPIO_B = 0x13,
    OLAT_A = 0x14, //latch
    OLAT_B = 0x15,
};
enum ADC_Cmd : unsigned char {
    NOP = 0, //No operation
    WAKEUP = 2,
    POWERDOWN = 4, //enter powerdown mode
    RESET = 6,
    START = 8,
    STOP = 0x0A,
    RDATA = 0x12
};
class Spi_ft
{
public:
    Spi_ft(int dev);
    ~Spi_ft();

    void RdacMyWriteAny(uint8 a, uint8 b, bool is_second);
    void RdacWriteQuants(uint16 data, bool is_second);
    void DacWriteQuants(uint16 value, bool is_first);
    bool ADCWriteAndCheck(uint8_t reg, uint8_t value);
    void FlashReadID(uint8 *answer);
    void FlashReadMem(uint32_t addr, int size, uint8 *answer);
    void FlashSendCmd(uint8 cmd);
    void FlashWriteBase(uint32_t addr, int size, uint8 *data);
    void FlashBlockErase4k(uint32_t addr);
    FLASH_STATUS_REG FlashReadStatusReg1();
    void MyRead(uint8* buf, int write_size, int read_size, Cs_device device);
    int GetMode(Cs_device dev);
    void MyWrite(uint8 *buf, int size, Cs_device device);
    void RdacWriteReg(uint16_t data, Cs_device device);
    uint8_t I2cReadReg(uint8_t reg, Cs_device device);
    void I2cWriteReg(uint8_t reg, uint8_t value, Cs_device device);
    bool GetInitStatus(){return ftStatus == FT_OK;}
    uint16_t RdacWriteRead(uint16_t data, Cs_device device);
    bool FlashEnableWrite();
    void FlashWaitReady();
    bool FlashWaitWriteDisabled();
public:
    uint16 IoReadReg16(uint8_t addr, Io_device device);
    void IoWriteReg16(uint8_t addr, Io_device device, uint16_t value);
    uint8_t IoReadReg(uint8_t addr, Io_device device);
    void IoWriteReg(uint8_t addr, Io_device device, uint8_t value);
    void DacWriteReg(char cmd, char addr, uint16_t value);
    ByteArray ADCReadData();
    void ADCCmd(uint8_t cmd);
    void ADCWriteReg(uint8_t addr, uint8_t value);
    uint8_t ADCReadReg(uint8_t addr);
    bool IsGPIOInt();
private:
#define MAX_BYTE_SIZE 260
    uint8 buf[MAX_BYTE_SIZE] = {0};
    bool lock = true;
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;

    std::mutex mutex;
    uint32 sizeToTransfer;
    DWORD sizeTransfered;
    Cs_device current_cs_device;
    int dev;
    int baud_rate;
#define WORD_SIZE 16
    uint8 msg[WORD_SIZE*MAX_BYTE_SIZE+2+2]{0};// 2 is preamble size, 2 is EOFrame size

    void RdacRelease(Cs_device rdac);
    void RdacWriteRegInternal(uint16_t data, Cs_device device);
    int SetupDevice(Cs_device dev);
};

#endif // SPI_FT_H

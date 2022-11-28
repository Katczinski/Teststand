#include <string.h>
#include "crc.h"

const uint32_t crc_32[] = {
        0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9,	0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
        0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
        0x4C11DB70, 0x48D0C6C7,	0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B,	0x569796C2, 0x52568B75,
        0x6A1936C8, 0x6ED82B7F,	0x639B0DA6, 0x675A1011,	0x791D4014, 0x7DDC5DA3,	0x709F7B7A, 0x745E66CD,
        0x9823B6E0, 0x9CE2AB57,	0x91A18D8E, 0x95609039,	0x8B27C03C, 0x8FE6DD8B,	0x82A5FB52, 0x8664E6E5,
        0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81,	0xAD2F2D84, 0xA9EE3033,	0xA4AD16EA, 0xA06C0B5D,
        0xD4326D90, 0xD0F37027,	0xDDB056FE, 0xD9714B49,	0xC7361B4C, 0xC3F706FB,	0xCEB42022, 0xCA753D95,
        0xF23A8028, 0xF6FB9D9F,	0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43,	0xE8BCCD9A, 0xEC7DD02D,
        0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
        0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16,	0x018AEB13, 0x054BF6A4,	0x0808D07D, 0x0CC9CDCA,
        0x7897AB07, 0x7C56B6B0,	0x71159069, 0x75D48DDE,	0x6B93DDDB, 0x6F52C06C,	0x6211E6B5, 0x66D0FB02,
        0x5E9F46BF, 0x5A5E5B08,	0x571D7DD1, 0x53DC6066,	0x4D9B3063, 0x495A2DD4,	0x44190B0D, 0x40D816BA,
        0xACA5C697, 0xA864DB20,	0xA527FDF9, 0xA1E6E04E,	0xBFA1B04B, 0xBB60ADFC,	0xB6238B25, 0xB2E29692,
        0x8AAD2B2F, 0x8E6C3698,	0x832F1041, 0x87EE0DF6,	0x99A95DF3, 0x9D684044,	0x902B669D, 0x94EA7B2A,
        0xE0B41DE7, 0xE4750050,	0xE9362689, 0xEDF73B3E,	0xF3B06B3B, 0xF771768C,	0xFA325055, 0xFEF34DE2,
        0xC6BCF05F, 0xC27DEDE8,	0xCF3ECB31, 0xCBFFD686,	0xD5B88683, 0xD1799B34,	0xDC3ABDED, 0xD8FBA05A,
        0x690CE0EE, 0x6DCDFD59,	0x608EDB80, 0x644FC637,	0x7A089632, 0x7EC98B85,	0x738AAD5C, 0x774BB0EB,
        0x4F040D56, 0x4BC510E1,	0x46863638, 0x42472B8F,	0x5C007B8A, 0x58C1663D,	0x558240E4, 0x51435D53,
        0x251D3B9E, 0x21DC2629,	0x2C9F00F0, 0x285E1D47,	0x36194D42, 0x32D850F5,	0x3F9B762C, 0x3B5A6B9B,
        0x0315D626, 0x07D4CB91,	0x0A97ED48, 0x0E56F0FF,	0x1011A0FA, 0x14D0BD4D,	0x19939B94, 0x1D528623,
        0xF12F560E, 0xF5EE4BB9,	0xF8AD6D60, 0xFC6C70D7,	0xE22B20D2, 0xE6EA3D65,	0xEBA91BBC, 0xEF68060B,
        0xD727BBB6, 0xD3E6A601,	0xDEA580D8, 0xDA649D6F,	0xC423CD6A, 0xC0E2D0DD,	0xCDA1F604, 0xC960EBB3,
        0xBD3E8D7E, 0xB9FF90C9,	0xB4BCB610, 0xB07DABA7,	0xAE3AFBA2, 0xAAFBE615,	0xA7B8C0CC, 0xA379DD7B,
        0x9B3660C6, 0x9FF77D71,	0x92B45BA8, 0x9675461F,	0x8832161A, 0x8CF30BAD,	0x81B02D74, 0x857130C3,
        0x5D8A9099, 0x594B8D2E,	0x5408ABF7, 0x50C9B640,	0x4E8EE645, 0x4A4FFBF2,	0x470CDD2B, 0x43CDC09C,
        0x7B827D21, 0x7F436096,	0x7200464F, 0x76C15BF8,	0x68860BFD, 0x6C47164A,	0x61043093, 0x65C52D24,
        0x119B4BE9, 0x155A565E,	0x18197087, 0x1CD86D30,	0x029F3D35, 0x065E2082,	0x0B1D065B, 0x0FDC1BEC,
        0x3793A651, 0x3352BBE6,	0x3E119D3F, 0x3AD08088,	0x2497D08D, 0x2056CD3A,	0x2D15EBE3, 0x29D4F654,
        0xC5A92679, 0xC1683BCE,	0xCC2B1D17, 0xC8EA00A0,	0xD6AD50A5, 0xD26C4D12,	0xDF2F6BCB, 0xDBEE767C,
        0xE3A1CBC1, 0xE760D676,	0xEA23F0AF, 0xEEE2ED18,	0xF0A5BD1D, 0xF464A0AA,	0xF9278673, 0xFDE69BC4,
        0x89B8FD09, 0x8D79E0BE,	0x803AC667, 0x84FBDBD0,	0x9ABC8BD5, 0x9E7D9662,	0x933EB0BB, 0x97FFAD0C,
        0xAFB010B1, 0xAB710D06,	0xA6322BDF, 0xA2F33668,	0xBCB4666D, 0xB8757BDA,	0xB5365D03, 0xB1F740B4
};


struct Crc_instance {
    enum crc_type type;			/*!< @~russian Вид контрольной суммы.*/
    uint8_t size;				/*!< @~russian Размер в битах контрольной суммы.*/
    uint32_t poly;				/*!< @~russian Полином контрольной суммы.*/
    const uint32_t * const crc_table;	/*!< @~russian Для 32 разрядной контрольной суммы.*/
    uint32_t init;              /*!< @~russian Для 32 разрядной контрольной суммы.*/				/*!< @~russian Начальное значение контрольной суммы. В зависимости от размерности используются разная размерность.*/
    bool reflect_in;			/*!< @~russian Используется ли отображение (биты симметрично меняются местами) входных данных при вычислении контрольной суммы.*/
    bool reflect_out;			/*!< @~russian Используется ли отображение  (биты симметрично меняются местами) контрольной суммы по завершении вычисления.*/
    bool complement;			/*!< @~russian Используется ли инверсия контрольной суммы по завершении вычисления.*/
};

struct Crc_instance crc_instance = {
        .poly = 0x04C11DB7,
        .crc_table = crc_32,
        .init = 0xFFFFFFFF,
        .reflect_in = true,
        .reflect_out = true,
        .complement = true,
};

static uint8_t reverse8(uint8_t val);

static uint32_t reverse32(uint32_t val);

uint32_t crc32_calc(const volatile uint8_t *nData, int wLength)
{
    uint32_t crc = crc_instance.init; // seed
    volatile uint8_t revIn;
    for (int i = 0; i < wLength; i++) {
        if (crc_instance.reflect_in == true) //reflecting in
            revIn = reverse8(nData[i]);
        else
            revIn = nData[i];
        volatile uint8_t pos = (uint8_t) ((crc ^ (uint32_t) revIn << 24) >> 24);
        crc = (uint32_t) ((crc << 8) ^ (crc_instance.crc_table[pos]));
    }
    if (crc_instance.reflect_out == true) //reflecting out
        crc = reverse32(crc);

    if (crc_instance.complement == true) // xor out
        crc = crc ^ 0xFFFFFFFF;
    return crc;
}

//reflecting byte
static uint8_t reverse8(uint8_t b)
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

//reflecting 4 bytes
static uint32_t reverse32(uint32_t b)
{
    uint8_t x1 = reverse8((uint8_t) (b & 0x000000FF));
    uint8_t x2 = reverse8((uint8_t) ((b & 0x0000FF00) >> 8));
    uint8_t x3 = reverse8((uint8_t) ((b & 0x00FF0000) >> 16));
    uint8_t x4 = reverse8((uint8_t) ((b & 0xFF000000) >> 24));
    return (uint32_t) (x1 << 24 | x2 << 16 | x3 << 8 | x4);
}

WORD CRC16 (const BYTE *nData, WORD wLength)
{
    static const WORD wCRCTable[] = {
       0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
       0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
       0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
       0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
       0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
       0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
       0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
       0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
       0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
       0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
       0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
       0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
       0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
       0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
       0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
       0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
       0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
       0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
       0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
       0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
       0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
       0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
       0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
       0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
       0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
       0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
       0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
       0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
       0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
       0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
       0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
       0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

    BYTE nTemp;
    WORD wCRCWord = 0xFFFF;

    while (wLength--)
    {
        nTemp = *nData++ ^ wCRCWord;
        wCRCWord >>= 8;
        wCRCWord  ^= wCRCTable[nTemp];
    }
    return wCRCWord;
} // End: CRC16

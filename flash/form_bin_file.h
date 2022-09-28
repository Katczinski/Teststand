#ifndef FORM_BIN_FILE_H
#define FORM_BIN_FILE_H
#include <QString>

#define LIBS_FLASH_SERIAL_NUMBER_ADDR    (0x100FE000 + 0*4) /*!< Серийный номер устройства */
#define LIBS_FLASH_REVISION				 (0x100FE000 + 1*4) /*!< Номер ревизии. */
#define LIBS_FLASH_EPOCH_TIME			 (0x100FE000 + 2*4) /*!< Время последней заводской прошивки. */
#define LIBS_FLASH_MODIF_ADDR 			 (0x100FE000 + 3*4) /*!< Адрес модификации модуля. */
#define LIBS_FLASH_CIPHER_ADDR           (0x100FE000 + 4*4) /*!< Аппаратный шифр устройства */
#define LIBS_FLASH_CIPHER_EXT_ADDR       (0x100FE000 + 4*4 + 64) /*!< Extended production cipher. Адрес расширенного аппаратного шифра. */
#define LIBS_FLASH_MAC_ADDR              (0x100FE000 + 4*4 + 64 + 12) /*! MAC адрес устройства  (8 байт) */
#define LIBS_FLASH_IP_ADDR               (0x100FE000 + 4*4 + 64 + 12 + 8) /*! IP адрес устройства  (4 байта) */
#define LIBS_FLASH_IP_MASK_ADDR          (0x100FE000 + 4*4 + 64 + 12 + 2*8) /*! IP-маска устройства  (4 байта) */
#define LIBS_FLASH_IP_GW_ADDR            (0x100FE000 + 4*4 + 64 + 12 + 3*8) /*! Адрес шлюза сети  (4 байта) */
#define LIBS_FLASH_FW_INFO_ADDR          (0x100FE000 + 4*4 + 64 + 12 + 4*8) /*! Структура с информацией о прошивке записанной на внешний чип Flash (8 байт) */

#pragma pack(1)
struct kb_eg {
    uint32_t    serialnum;
    uint32_t    rev;
    uint32_t    since_epoch;
    uint32_t    modif;
    uint32_t    cipher[16];
    char        ext_cipher[12];
    uint32_t    mac[2]; // 00-FF-38-34-96-A5 (random)
    uint32_t    ip;     // 172.16.7.77
    uint32_t    mask;   // 255.255.252.0
    uint32_t    gateway;// 172.16.4.2
    uint32_t    info[2];
};
#pragma pack()

#pragma pack(1)
struct bin {
    char        cipher[124];
    uint32_t    serialnum;
    uint32_t    rev;
    uint32_t    since_epoch[2];
    uint32_t    modif;
};
#pragma pack()

bool Form_bin_file(QStringList info); // cipher, serialnum, modification, type, mac, ip, mask, gateway

#endif // FORM_BIN_FILE_H

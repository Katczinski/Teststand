#ifndef CRC_H
#define CRC_H

/*******************************************************************************
*           Includes
*******************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef unsigned long DWORD;   // DWORD = unsigned 32 bit value
typedef uint16_t WORD;    // WORD = unsigned 16 bit value
typedef uint8_t BYTE;     // BYTE = unsigned 8 bit value

#ifndef LIBS_CRC_USE_HARDWARE
#define LIBS_CRC_USE_HARDWARE (0)	/*!<Использование аппаратного подсчета контрольной суммы.*/
#endif


//Настройки при использовании аппаратного подсчета контрольной суммы
#if LIBS_CRC_USE_HARDWARE == 1

#ifndef LIBS_CRC_USE_SOFTWARE_MODBUS
#define LIBS_CRC_USE_SOFTWARE_MODBUS (0) /*!<Использовать программный метод подсчета контрольной суммы modbus.*/
#endif

#ifndef LIBS_CRC_USE_SOFTWARE_XMODEM
#define LIBS_CRC_USE_SOFTWARE_XMODEM (0) /*!<Использовать программный метод подсчета контрольной суммы xmodem.*/
#endif

#ifndef LIBS_CRC_USE_SOFTWARE_CRC8
#define LIBS_CRC_USE_SOFTWARE_CRC8 (1) /*!<Использовать программный метод подсчета контрольной суммы crc8.*/
#endif

#ifndef LIBS_CRC_USE_SOFTWARE_CRC32
#define LIBS_CRC_USE_SOFTWARE_CRC32 (0) /*!<Использовать программный метод подсчета контрольной суммы crc32.*/
#endif

#else

#ifndef LIBS_CRC_USE_SOFTWARE_MODBUS
#define LIBS_CRC_USE_SOFTWARE_MODBUS (1) /*!<Использовать программный метод подсчета контрольной суммы modbus.*/
#endif

#ifndef LIBS_CRC_USE_SOFTWARE_XMODEM
#define LIBS_CRC_USE_SOFTWARE_XMODEM (1) /*!<Использовать программный метод подсчета контрольной суммы xmodem.*/
#endif

#ifndef LIBS_CRC_USE_SOFTWARE_CRC8
#define LIBS_CRC_USE_SOFTWARE_CRC8 (1) /*!<Использовать программный метод подсчета контрольной суммы crc8.*/
#endif

#ifndef LIBS_CRC_USE_SOFTWARE_CRC32
#define LIBS_CRC_USE_SOFTWARE_CRC32 (1) /*!<Использовать программный метод подсчета контрольной суммы crc32.*/
#endif

#endif

#ifndef LIBS_CRC_USE_TABLE_MODBUS
#define LIBS_CRC_USE_TABLE_MODBUS (1) /*!<Использовать табличный метод подсчета контрольной суммы modbus.*/
#endif
#ifndef LIBS_CRC_USE_TABLE_XMODEM
#define LIBS_CRC_USE_TABLE_XMODEM (1) /*!<Использовать табличный метод подсчета контрольной суммы xmodem.*/
#endif
#ifndef LIBS_CRC_USE_TABLE_CRC8
#define LIBS_CRC_USE_TABLE_CRC8 (1) /*!<Использовать табличный метод подсчета контрольной суммы crc8.*/
#endif
#ifndef LIBS_CRC_USE_TABLE_CRC32
#define LIBS_CRC_USE_TABLE_CRC32 (1) /*!<Использовать табличный метод подсчета контрольной суммы crc32.*/
#endif

/*!
 * @~russian
 * @brief Вид контрольной суммы. Дополнительные параметры сумм см. crc.c.
 */
enum crc_type {
    CRC_MODBUS = 0,		/*!< @~russian CRC16 modbus.*/
    CRC_XMODEM = 1,		/*!< @~russian CRC16 xmodem.*/
    CRC_8 = 2,		/*!< @~russian CRC8.*/
    CRC_32 = 3,		/*!< @~russian CRC32.*/
    CRC_COUNT 		/*!< @~russian Вспомогательное перечисление.*/
};

uint32_t        crc32_calc(const volatile uint8_t *data, int data_length);
WORD            CRC16 (const BYTE *nData, WORD wLength);
#endif /* CRC_H_ */

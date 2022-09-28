#ifndef CRC8_107_HEC_H
#define CRC8_107_HEC_H
#include <inttypes.h>

uint8_t Crc8( uint8_t crc, uint8_t data );

uint8_t Crc8Block( uint8_t crc, uint8_t *data, uint8_t len );

#endif // CRC8_107_HEC_H

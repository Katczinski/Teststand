#pragma once
#ifndef MODBUS_HELPERS_H
#define MODBUS_HELPERS_H

#include <stdint.h>
#include <vector>
#include <iostream>
#include "utils.h"

void AddU8(ByteArray& array, int value);
void AddU16(ByteArray& array, int value);
void AddU32(ByteArray& array, uint32_t value);
void AddFloat(ByteArray& array, float value);

uint8_t ExtractU8(const ByteArray& array, int addr);
uint16_t ExtractU16(const ByteArray& array, int addr);
uint32_t ExtractU32(const char* array, int len, int addr);
float ExtractFloat(const char* array, int len, int addr);

inline uint16_t bswap_16(uint16_t x)
{
#ifdef __GNUC__
    return __builtin_bswap16(x);
# else
    return uint16_t((x >> 8) | (x << 8));
# endif
}

inline uint32_t bswap_32(uint32_t x)
{
#ifdef __GNUC__
    return __builtin_bswap32(x);
#else
    return uint32_t(bswap_16(x & 0xffff) << 16) | (bswap_16(x >> 16));
#endif
}

uint16_t Crc16modbus(const ByteArray& array);
void AddCrc(ByteArray& array);
std::vector<uint16_t> GetRegsFromBits(std::vector<bool> data);

#endif // MODBUS_HELPERS_H


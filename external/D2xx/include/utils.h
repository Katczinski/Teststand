#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <cstdlib>
#include "bytearray.h"

inline void msleep(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
inline void usleep(int us) { std::this_thread::sleep_for(std::chrono::microseconds(us)); }

template<class T>
void setBit(T *value, int n, bool state)
{
    *value ^= (-state ^ *value) & ((T)1 << n); // set n'th bit of value to state
}
template <class T>
bool bitAt(T value, int num) {
    return (value & (1 << num)) >> num;
}
std::string toString(ByteArray array);
std::string toUpper(const std::string& str);
uint16_t    Checksum(const char *data, unsigned int len);
uint16_t    Checksum(const char *data, unsigned int len, ChecksumType standard);

#endif

#pragma once
#include <cstdint>
#include <vector>
#include "math/uint256.hpp"

uint256 littleEndianToInt(const std::vector<uint8_t>&);
uint256 bigEndianToInt(std::vector<uint8_t>&);
std::vector<uint8_t> intToLittleEndian(uint64_t);
std::vector<uint8_t> intToLittleEndian(uint256, uint);
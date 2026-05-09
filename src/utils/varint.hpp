#pragma once
#include "utils/byte_stream.hpp"
#include <cstdint>

uint64_t readVarint(ByteStream&);
std::vector<uint8_t> encodeVarint(uint64_t);
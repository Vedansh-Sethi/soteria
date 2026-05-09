#include "utils/varint.hpp"
#include "utils/byte_to_int.hpp"
#include <algorithm>
#include <bit>
#include <stdexcept>

uint64_t readVarint(ByteStream &stream)
{
    uint8_t marker = stream.read(1)[0];

    if (marker == 0xfd)
    {
        std::vector<uint8_t> intBytes = stream.read(2);
        return uint64_t(littleEndianToInt(intBytes));
    }

    if (marker == 0xfe)
    {
        std::vector<uint8_t> intBytes = stream.read(4);
        return uint64_t(littleEndianToInt(intBytes));
    }

    if (marker == 0xff)
    {
        std::vector<uint8_t> intBytes = stream.read(8);
        return uint64_t(littleEndianToInt(intBytes));
    }

    return marker;
}

std::vector<uint8_t> encodeVarint(uint64_t i)
{
    if(i < 0xfd)
    {
        return intToLittleEndian(i);
    }
    else if(i < 0x10000)
    {
        std::vector<uint8_t> bytes = intToLittleEndian(i);
        std::reverse(bytes.begin(), bytes.end());
        bytes.push_back(0xfd);
        std::reverse(bytes.begin(), bytes.end());
        return bytes;
    }
    else if(i < 0x100000000)
    {
        std::vector<uint8_t> bytes = intToLittleEndian(i);
        std::reverse(bytes.begin(), bytes.end());
        bytes.push_back(0xfe);
        std::reverse(bytes.begin(), bytes.end());
        return bytes;
    }
    else if(i < 0x1000000000000)
    {
        std::vector<uint8_t> bytes = intToLittleEndian(i);
        std::reverse(bytes.begin(), bytes.end());
        bytes.push_back(0xff);
        std::reverse(bytes.begin(), bytes.end());
        return bytes;
    }
    else throw std::out_of_range("Given Integer is Out of Range for Varint");
}
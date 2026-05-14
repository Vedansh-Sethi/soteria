#include "utils/byte_to_int.hpp"
#include <algorithm>

uint256 littleEndianToInt(const std::vector<uint8_t> &bytes)
{
    uint256 intNum = 0;
    int i = 0;
    for (uint8_t byte : bytes)
    {
        intNum |= (uint256(1) << (8 * i)) * byte;
        i++;
    }
    return intNum;
}

uint256 bigEndianToInt(const std::vector<uint8_t> &bytes)
{
    std::vector<uint8_t> temp = bytes;
    std::reverse(temp.begin(), temp.end());
    return littleEndianToInt(temp);
}

std::vector<uint8_t> intToLittleEndian(uint64_t num)
{
    std::vector<uint8_t> bytes;
    do
    {
        bytes.push_back(uint8_t(num & 0xff));
        num >>= 8;
    } while(num > 0);
    return bytes;
}

std::vector<uint8_t> intToLittleEndian(uint256 num, uint byteNum)
{
    std::vector<uint8_t> bytes;
    for(uint i  = 0; i < byteNum; i++)
    {
        bytes.push_back(uint8_t(num & 0xff));
        num >>= 8;
    }
    return bytes;
}
#pragma once
#include "utils/uint256/uint256.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <string>

inline std::string hexify(uint256 num)
{
    std::string hexer = "0123456789abcdef";
    std::string hexed;
    for(int i = 0;i < 64; i++)
    {
        hexed += hexer[int(num & 0xf)];
        num >>= 4;
    }
    std::reverse(hexed.begin(),  hexed.end());
    return hexed;
}

inline uint256 dehexify(std::string hex)
{
    uint256 dehexed = 0;
    for(size_t i = 0; i < hex.size(); i++)
    {
        if(std::isdigit(hex[i]))
        {
            uint256 bitShift = uint256(1)<<((hex.size() - i - 1)*4);
            dehexed += (hex[i] - '0')*bitShift;
        }
        if('a' <= hex[i] && hex[i] <= 'f')
        {
            uint256 bitShift = uint256(1)<<((hex.size() - i - 1)*4);
            dehexed += (hex[i] - 'a' + 10)*bitShift;
        }
    }
    return dehexed;
}

template<typename ByteContainers>
inline std::string hexifyBytes(const ByteContainers& bytes)
{
    std::string hexer = "0123456789abcdef";
    std::string result;

    for(std::byte byte : bytes)
    {
        uint8_t val = std::to_integer<uint8_t>(byte);

        result += hexer[val >> 4];
        result += hexer[val & 0xf];
    }

    return result;
}
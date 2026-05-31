#pragma once
#include "math/uint256.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <string>

inline std::string hexify(uint256 num)
{
    std::string hexer = "0123456789abcdef";
    std::string hexed;
    for (int i = 0; i < 64; i++)
    {
        hexed += hexer[uint(num & 0xf)];
        num >>= 4;
    }
    std::reverse(hexed.begin(), hexed.end());
    return hexed;
}

inline uint256 dehexify(std::string hex)
{
    uint256 dehexed = 0;
    for (size_t i = 0; i < hex.size(); i++)
    {
        if (std::isdigit(hex[i]))
        {
            uint256 bitShift = uint256(1) << ((hex.size() - i - 1) * 4);
            dehexed += (hex[i] - '0') * bitShift;
        }
        if ('a' <= hex[i] && hex[i] <= 'f')
        {
            uint256 bitShift = uint256(1) << ((hex.size() - i - 1) * 4);
            dehexed += (hex[i] - 'a' + 10) * bitShift;
        }
    }
    return dehexed;
}

template <typename ByteContainers>
inline std::string hexifyBytes(const ByteContainers &bytes)
{
    std::string hexer = "0123456789abcdef";
    std::string result;

    for (std::byte byte : bytes)
    {
        uint8_t val = std::to_integer<uint8_t>(byte);

        result += hexer[val >> 4];
        result += hexer[val & 0xf];
    }

    return result;
}

inline std::string hexifyBytes(const std::vector<uint8_t> &bytes)
{
    std::string hexer = "0123456789abcdef";
    std::string result;

    for (uint8_t byte : bytes)
    {
        result += hexer[byte >> 4];
        result += hexer[byte & 0xf];
    }

    return result;
}

inline std::vector<uint8_t> dehexifyString(const std::string &hex)
{
    std::vector<uint8_t> bytes;
    for (size_t pointer = 0; pointer < hex.length(); pointer += 2)
    {
        std::string byteString = hex.substr(pointer, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

inline std::vector<uint8_t> stringToBytes(const std::string &inp)
{
    return std::vector<uint8_t>(inp.begin(), inp.end());
}
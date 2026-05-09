#include "signing/signature/signature.hpp"
#include <cstddef>

std::vector<std::byte> concat(std::vector<std::byte> a, std::vector<std::byte> b)
{
    std::vector<std::byte> result;
    result.reserve(a.size() + b.size());
    result.insert(result.end(), a.begin(), a.end());
    result.insert(result.end(), b.begin(), b.end());

    return result;
}

std::vector<std::byte> Signature::serialize() const 
{
    std::vector<std::byte> serialized;
    std::byte marker{0x30};
    std::byte numMarker{0x2};
    serialized.push_back(marker); // 1st byte
    serialized.push_back(std::byte{0x0}); // for string length (2nd byte)

    std::vector<std::byte> rBytes;
    std::vector<std::byte> sBytes;

    uint256 rTemp = r.data;

    do
    {
        std::byte byte = std::byte{uint8_t(rTemp & 0xff)};
        rBytes.push_back(byte);
        rTemp >>= 8;
    } while(rTemp > 0);
    serialized.push_back(numMarker); // 3rd byte
    serialized.push_back(std::byte{uint8_t(rBytes.size())}); // for r length (4th byte)
    std::reverse(rBytes.begin(), rBytes.end());
    if(std::to_integer<uint8_t>(rBytes[0]) >= 128)
    {
        uint8_t size = rBytes.size() + 1;
        serialized.back() = std::byte{size};
        serialized.push_back(std::byte{0x0}); // before pushing the actual bytes of r
    }
    serialized = concat(serialized, rBytes);

    uint256 sTemp = s.data;
    do
    {
        std::byte byte = std::byte{uint8_t(sTemp & 0xff)};
        sBytes.push_back(byte);
        sTemp >>= 8;
    } while(sTemp > 0);
    std::reverse(sBytes.begin(), sBytes.end());
    serialized.push_back(numMarker);
    serialized.push_back(std::byte{uint8_t(sBytes.size())});
    if(std::to_integer<uint8_t>(sBytes[0]) >= 128)
    {
        uint8_t size = sBytes.size() + 1;
        serialized.back() = std::byte{size};
        serialized.push_back(std::byte{0x0});
    }
    serialized = concat(serialized, sBytes);
    serialized[1] = std::byte{uint8_t(serialized.size() - 2)};

    return serialized;
}
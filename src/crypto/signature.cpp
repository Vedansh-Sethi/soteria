#include "crypto/signature.hpp"
#include "crypto/crypto.hpp"
#include "crypto/secp256k1.hpp"
#include "utils/byte_to_int.hpp"
#include <algorithm>
#include <format>

Crypto* signatureInstance = Crypto::GetInstance();

std::vector<std::byte> concat(std::vector<std::byte> a,
                              std::vector<std::byte> b)
{
    std::vector<std::byte> result;
    result.reserve(a.size() + b.size());
    result.insert(result.end(), a.begin(), a.end());
    result.insert(result.end(), b.begin(), b.end());

    return result;
}

std::vector<uint8_t> Signature::serialize() const
{
    std::vector<uint8_t> serialized;
    uint8_t marker{0x30};
    uint8_t numMarker{0x2};
    serialized.push_back(marker); // 1st byte
    serialized.push_back(0x0);    // for string length (2nd byte)

    std::vector<uint8_t> rBytes;
    std::vector<uint8_t> sBytes;

    uint256 rTemp = r.data;

    do
    {
        uint8_t byte = uint8_t(rTemp & 0xff);
        rBytes.push_back(byte);
        rTemp >>= 8;
    } while (rTemp > 0);
    serialized.push_back(numMarker);              // 3rd byte
    serialized.push_back(uint8_t(rBytes.size())); // for r length (4th byte)
    std::reverse(rBytes.begin(), rBytes.end());
    if (rBytes[0] >= 128)
    {
        uint8_t size = rBytes.size() + 1;
        serialized.back() = size;
        serialized.push_back(0x0); // before pushing the actual bytes of r
    }
    serialized.insert(serialized.end(), rBytes.begin(), rBytes.end());

    uint256 sTemp = s.data;
    do
    {
        uint8_t byte = uint8_t(sTemp & 0xff);
        sBytes.push_back(byte);
        sTemp >>= 8;
    } while (sTemp > 0);
    std::reverse(sBytes.begin(), sBytes.end());
    serialized.push_back(numMarker);
    serialized.push_back(uint8_t(sBytes.size()));
    if (sBytes[0] >= 128)
    {
        uint8_t size = sBytes.size() + 1;
        serialized.back() = size;
        serialized.push_back(0x0);
    }
    serialized.insert(serialized.end(), sBytes.begin(), sBytes.end());
    serialized[1] = uint8_t(serialized.size() - 2);

    return serialized;
}

Signature Signature::parse(const std::vector<uint8_t> &serial)
{
    if (serial[0] != 0x30)
        throw std::format_error("wrong marker for signature");
    if (serial[2] != 0x2)
        throw std::format_error("wrong marker for Num Bytes");
    uint8_t rLength = serial[3];
    std::vector<uint8_t> rBytes(serial.begin() + 4,
                                serial.begin() + 4 + rLength);
    S256Field r = bigEndianToInt(rBytes);
    if (serial[4 + rLength] != 0x2)
        throw std::format_error("wrong marker for Num Bytes");
    uint8_t sLength = serial[5 + rLength];
    std::vector<uint8_t> sBytes(serial.begin() + 6 + rLength,
                                serial.begin() + 6 + rLength + sLength);
    ScalarField s = bigEndianToInt(sBytes);
    return Signature(r, s);
}

std::string PrivateKey::RNG256()
{
    const std::string hexer = "0123456789abcdef";
    std::string hexedRandom = "0x";
    for (int i = 0; i < 64; i++)
    {
        hexedRandom += hexer[std::rand() % 16];
    }
    return hexedRandom;
}

Signature PrivateKey::sign(uint256 z) const
{
    ScalarField zField = ScalarField(z);
    ScalarField k = dehexify(RNG256());
    S256Field r = (Genesis * k).x.value().data;
    ScalarField s = (zField + secret * ScalarField(r.data)) / k;
    if (s.data > N / 2)
    {
        s.data = N - s.data;
    }
    return Signature(r, s);
}

std::array<std::byte, 34> PrivateKey::wif(bool testnet) const
{
    std::byte marker;
    if (testnet)
        marker = std::byte{0xef};
    else
        marker = std::byte{0x80};
    std::array<std::byte, 34> secretBytes;

    uint256 secretTemp = secret.data;
    for (int i = 32; i > 0; i--)
    {
        secretBytes[i + 1] = std::byte{uint8_t(secretTemp & 0xff)};
        secretTemp >>= 8;
    }

    secretBytes[0] = marker;
    secretBytes[33] = std::byte{0x01};

    return secretBytes;
}

Script PrivateKey::p2pkhScript() const
{
    auto h160 = signatureInstance->hash160(publicKey.serialize());
    std::vector<Token> cmds;
    cmds.push_back({true, {}, 0x76});
    cmds.push_back({true, {}, 0xa9});
    cmds.push_back({false, h160, 0x00});
    cmds.push_back({true, {}, 0x88});
    cmds.push_back({true, {}, 0xac});
    return Script(cmds);
}
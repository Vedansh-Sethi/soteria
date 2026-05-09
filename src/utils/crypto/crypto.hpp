#pragma once
#include <array>
#include <cstdint>
#include <vector>

class Crypto
{
  private:
    Crypto() {}
    static Crypto *instance;
    std::vector<uint8_t> InnerHash160(const std::vector<uint8_t> &input) const;
    std::vector<uint8_t> InnerSha256(const std::vector<uint8_t> &input) const;

  public:
    Crypto(const Crypto &) = delete;
    void operator=(const Crypto &) = delete;
    static Crypto *GetInstance();

    template <typename ByteContainer>
    std::array<std::byte, 32> hash256(const ByteContainer &input) const
    {
        std::vector<uint8_t> midInp;
        for (std::byte byte : input)
        {
            midInp.push_back(std::to_integer<uint8_t>(byte));
        }
        std::vector<uint8_t> h256 = InnerSha256(InnerSha256(midInp));
        std::array<std::byte, 32> hashFinal;
        for (int i = 0; i < 32; i++)
        {
            hashFinal[i] = std::byte{h256[i]};
        }

        return hashFinal;
    }

    template <typename ByteContainer>
    std::array<std::byte, 20> hash160(const ByteContainer &input) const
    {
        std::vector<uint8_t> midInp;
        for (std::byte byte : input)
        {
            midInp.push_back(std::to_integer<uint8_t>(byte));
        }
        std::vector<uint8_t> h160 = InnerHash160(midInp);
        std::array<std::byte, 20> hashFinal;
        for (int i = 0; i < 20; i++)
        {
            hashFinal[i] = std::byte{h160[i]};
        }

        return hashFinal;
    }
};
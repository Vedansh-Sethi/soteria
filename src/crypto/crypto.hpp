#pragma once
#include <array>
#include <cstdint>
#include <cstring>

class Crypto
{
  private:
    Crypto() {}
    static Crypto *instance;
    void InnerHash160(const uint8_t *input, size_t len, uint8_t *output) const;
    void InnerSha256(const uint8_t *input, size_t len, uint8_t *output) const;

  public:
    Crypto(const Crypto &) = delete;
    void operator=(const Crypto &) = delete;
    static Crypto *GetInstance();

    template <typename ByteContainer>
    std::array<std::byte, 32> hash256(const ByteContainer &input) const
    {
        std::array<std::byte, 32> hash1;
        InnerSha256(reinterpret_cast<const uint8_t *>(input.begin()),
                    input.size(), reinterpret_cast<uint8_t *>(hash1.begin()));
        std::array<std::byte, 32> hash2;
        InnerSha256(reinterpret_cast<const uint8_t *>(hash1.begin()), 32,
                    reinterpret_cast<uint8_t *>(hash2.begin()));

        return hash2;
    }

    template <typename ByteContainer>
    std::array<std::byte, 20> hash160(const ByteContainer &input) const
    {
        std::array<std::byte, 20> hash;
        InnerHash160(reinterpret_cast<const uint8_t *>(input.begin()),
                     input.size(), reinterpret_cast<uint8_t *>(hash.begin()));
        return hash;
    }
};
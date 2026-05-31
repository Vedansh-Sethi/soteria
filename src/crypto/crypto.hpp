#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

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
    std::vector<uint8_t> hash256(const ByteContainer &input) const
    {
        std::vector<uint8_t> hash1(32);
        InnerSha256(reinterpret_cast<const uint8_t *>(input.data()),
                    input.size(), reinterpret_cast<uint8_t *>(hash1.data()));
        std::vector<uint8_t> hash2(32);
        InnerSha256(reinterpret_cast<const uint8_t *>(hash1.data()), 32,
                    reinterpret_cast<uint8_t *>(hash2.data()));

        return hash2;
    }

    std::vector<uint8_t> hash256(const std::string &input) const;

    template <typename ByteContainer>
    std::vector<uint8_t> hash160(const ByteContainer &input) const
    {
        std::vector<uint8_t> hash(20);
        InnerHash160(reinterpret_cast<const uint8_t *>(input.data()),
                     input.size(), reinterpret_cast<uint8_t *>(hash.data()));
        return hash;
    }
};
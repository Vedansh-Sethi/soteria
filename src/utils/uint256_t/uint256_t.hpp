#pragma once
#include <array>
#include <cstdint>
#include <string>

template <unsigned int BITS> class base_bigInt
{
  protected:
    std::array<uint8_t, BITS / 8> data;

  public:
    base_bigInt() { data.fill(0); }
    base_bigInt(const std::array<uint8_t, BITS / 8> &data) : data(data) {}

    bool operator==(const base_bigInt<BITS> &other) const
    {
        return data == other.data;
    }

    bool operator<(const base_bigInt<BITS> &other) const
    {
        return data < other.data;
    }

    std::string GetHex() const
    {
        const char *lookUp = "0123456789abcdef";
        std::string s;
        s.reserve((BITS / 8) * 2);
        for (int i = 0; i < BITS / 8; i++)
        {
            uint8_t byte = data[i];
            s.push_back(lookUp[byte >> 4]);
            s.push_back(lookUp[byte & 0x0f]);
        }
        return s;
    }

    const uint8_t* begin() const {return data.data();}
    const uint8_t* end() const {return data.data() + data.size();}
};

typedef base_bigInt<256> uint256_t;
typedef base_bigInt<160> uint160_t;
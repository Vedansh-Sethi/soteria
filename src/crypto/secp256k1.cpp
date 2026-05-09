#include "crypto/secp256k1.hpp"
#include "crypto/crypto.hpp"
#include "crypto/signature.hpp"
#include <stdexcept>

Crypto *instance = Crypto::GetInstance();

S256Point S256Point::operator*(const uint256 coeff) const
{
    uint256 coef = coeff % N;
    return FFECP::operator*(coef);
}

S256Point S256Point::operator*(const ScalarField &coefficient) const
{
    uint256 coeff = coefficient.data;
    return *this * coeff;
}

bool S256Point::verify(uint256 z, Signature sign) const
{
    S256Field r = sign.r;
    ScalarField s = sign.s;
    ScalarField zField = ScalarField(z);

    if (s.data == 0 || r.data == 0)
        return false;

    ScalarField u = zField / s;
    ScalarField v = ScalarField(r.data) / s;

    S256Point target = Genesis * u.data + *this * v.data;

    if (!target.x.has_value())
        return false;

    return target.x.value() == r;
}

std::array<std::byte, 33> S256Point::serialize() const
{
    std::byte marker = (y.value().data % 2) ? std::byte{0x3} : std::byte{0x2};

    std::array<std::byte, 32> xBytes;
    if (!x.has_value())
        throw std::invalid_argument("Cannot serialize points at infinity");
    uint256 xTemp = x.value().data;
    for (int i = 0; i < 32; i++)
    {
        xBytes[i] = std::byte{uint8_t(xTemp & 0xff)};
        xTemp >>= 8;
    }
    reverse(xBytes.begin(), xBytes.end());

    std::array<std::byte, 33> result;

    result[0] = marker;
    std::copy(xBytes.begin(), xBytes.end(), result.begin() + 1);

    return result;
}

S256Point S256Point::parse(std::array<std::byte, 33> secBytes)
{
    std::array<std::byte, 32> xBytes;
    std::copy(secBytes.begin() + 1, secBytes.end(), xBytes.begin());
    S256Field xNum = hexifyBytes(xBytes);
    S256Field alpha = xNum.pow(3) + B;
    S256Field beta = alpha.sqrt();
    S256Field evenBeta = 0, oddBeta = 0;
    if (beta.data % 2 == 0)
    {
        evenBeta = beta;
        oddBeta = S256Field(0) - beta;
    }
    else
    {
        evenBeta = S256Field(0) - beta;
        oddBeta = beta;
    }
    if (secBytes[0] == std::byte{0x2})
        return S256Point(xNum, evenBeta);
    else
        return S256Point(xNum, oddBeta);
}

std::array<std::byte, 21> S256Point::address(bool testnet) const
{
    std::array<std::byte, 33> serial = this->serialize();
    std::array<std::byte, 20> h160 = instance->hash160(serial);
    std::reverse(h160.begin(), h160.end());
    std::array<std::byte, 21> address;
    std::copy(h160.begin(), h160.end(), address.begin());
    if (testnet)
    {
        address[20] = std::byte{0x6f};
    }
    else
    {
        address[20] = std::byte{0x00};
    }
    std::reverse(address.begin(), address.end());
    return address;
}

S256Field S256Field::sqrt() const
{
    return this->pow((prime + 1) / 4);
}
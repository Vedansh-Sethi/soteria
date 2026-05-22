#include "crypto/secp256k1.hpp"
#include "crypto/crypto.hpp"
#include "crypto/signature.hpp"
#include <format>
#include <stdexcept>

Crypto *secp256k1Instance = Crypto::GetInstance();

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

std::vector<uint8_t> S256Point::serialize() const
{
    uint8_t marker = (y.value().data % 2) ? 0x3 : 0x2;

    std::vector<uint8_t> xBytes;
    if (!x.has_value())
        throw std::invalid_argument("Cannot serialize points at infinity");
    uint256 xTemp = x.value().data;
    for (int i = 0; i < 32; i++)
    {
        xBytes.push_back(uint8_t(xTemp & 0xff));
        xTemp >>= 8;
    }
    reverse(xBytes.begin(), xBytes.end());

    std::vector<uint8_t> result;

    result.push_back(marker);
    result.insert(result.end(), xBytes.begin(), xBytes.end());

    return result;
}

S256Point S256Point::parse(std::vector<uint8_t> secBytes)
{
    std::vector<uint8_t> xBytes(secBytes.begin() + 1, secBytes.end());
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
    if (secBytes[0] == 0x2)
        return S256Point(xNum, evenBeta);
    else if (secBytes[0] == 0x3)
        return S256Point(xNum, oddBeta);
    else
        throw std::format_error("Wrong SEC Format");
}

std::vector<uint8_t> S256Point::address(bool testnet) const
{
    std::vector<uint8_t> serial = this->serialize();
    std::vector<uint8_t> h160 = secp256k1Instance->hash160(serial);

    std::vector<uint8_t> address;
    address.push_back(testnet ? 0x6f : 0x00);
    std::copy(h160.begin(), h160.end(), address.begin() + 1);
    return address;
}

S256Field S256Field::sqrt() const { return this->pow((prime + 1) / 4); }
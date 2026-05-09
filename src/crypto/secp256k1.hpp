#pragma once
#include "math/ECP.hpp"
#include "math/field_element.hpp"
#include "math/uint256.hpp"
#include "utils/hexer.hpp"
#include <optional>

class Signature;

const uint256 P = dehexify(
    "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");

const uint256 N = dehexify(
    "0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");

class S256Field : public FieldElement
{
  public:
    S256Field(uint256 data) : FieldElement(data % P, P) {}
    S256Field(int data) : FieldElement(uint256(data) % P, P) {}
    S256Field(std::string hexcode) : FieldElement(dehexify(hexcode) % P, P) {}
    S256Field(const char *hexcode) : FieldElement(dehexify(hexcode) % P, P) {}
    S256Field(FieldElement fe) : FieldElement(fe)
    {
        fe.data %= P;
        fe.prime = P;
    }
    friend std::ostream &operator<<(std::ostream &os, const S256Field &S256)
    {
        os << "secp256k1(" << hexify(S256.data) << ")";
        return os;
    }

    S256Field sqrt() const;
};

class ScalarField : public FieldElement
{
  public:
    ScalarField(uint256 data) : FieldElement(data % N, N) {}
    ScalarField(int data) : FieldElement(uint256(data) % N, N) {}
    ScalarField(std::string hexcode) : FieldElement(dehexify(hexcode) % N, N) {}
    ScalarField(const char *hexcode) : FieldElement(dehexify(hexcode) % N, N) {}
    ScalarField(FieldElement fe) : FieldElement(fe)
    {
        fe.data %= N;
        fe.prime = N;
    }
    friend std::ostream &operator<<(std::ostream &os, const ScalarField &scalar)
    {
        os << "scalar(" << hexify(scalar.data) << ")";
        return os;
    }
};

const S256Field A = uint256(0);
const S256Field B = uint256(7);

class S256Point : public FFECP
{
  private:
    S256Point(FFECP point) : FFECP(point)
    {
        point.a = A;
        point.b = B;
        if (!point.x.has_value())
        {
            this->x = std::nullopt;
            this->y = std::nullopt;
        }
        else
        {
            std::optional<S256Field> xNew = std::make_optional(point.x.value());
            std::optional<S256Field> yNew = std::make_optional(point.y.value());
            this->x = xNew;
            this->y = yNew;
        }
    }

  public:
    S256Point(uint256 x, uint256 y)
        : FFECP(A, B, std::make_optional<S256Field>(x),
                std::make_optional<S256Field>(y))
    {
    }
    S256Point(S256Field x, S256Field y) : FFECP(A, B, x, y) {}
    S256Point(std::nullopt_t, std::nullopt_t)
        : FFECP(A, B, std::nullopt, std::nullopt)
    {
    }
    S256Point operator*(const uint256) const;
    S256Point operator*(const ScalarField &) const;
    bool verify(uint256, Signature) const;
    std::array<std::byte, 33> serialize() const;
    static S256Point parse(std::array<std::byte, 33>);
    std::array<std::byte, 21> address(bool testnet = false) const;
    friend std::ostream &operator<<(std::ostream &os, const S256Point &S256)
    {
        if (!S256.x.has_value())
        {
            os << "S256Point(Infinity)";
            return os;
        }
        os << "S256Point(" << hexify(S256.x.value().data) << ","
           << hexify(S256.y.value().data);
        return os;
    }
};

const S256Point Genesis = S256Point(
    "0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798",
    "0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8");

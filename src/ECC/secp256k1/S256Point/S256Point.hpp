#pragma once
#include "ECC/ECP/ECP.hpp"
#include "ECC/secp256k1/S256Field/S256Field.hpp"
#include "signing/scalarField/scalarField.hpp"
#include "signing/signature/signature.hpp"
#include "utils/hexer/hexer.hpp"
#include <optional>

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
    S256Point(std::nullopt_t, std::nullopt_t) : FFECP(A, B, std::nullopt, std::nullopt) {}
    S256Point operator*(const uint256) const;
    S256Point operator*(const ScalarField&) const;
    bool verify(uint256, Signature) const;
    std::array<std::byte, 33> serialize() const;
    static S256Point parse(std::array<std::byte, 33>);
    friend std::ostream &operator<<(std::ostream &os, const S256Point &S256)
    {
        if (!S256.x.has_value())
        {
            os << "S256Point(Infinity)";
            return os;
        }
        os << "S256Point(" << hexify(S256.x.value().data) << "," << hexify(S256.y.value().data);
        return os;
    }
};

const S256Point Genesis = S256Point(
    "0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798",
    "0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8");
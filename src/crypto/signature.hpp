#pragma once
#include "math/field_element.hpp"
#include "utils/hexer.hpp"
#include "crypto/secp256k1.hpp"
#include <cstddef>

class Signature
{
  public:
    S256Field r;
    ScalarField s;
    
    friend std::ostream& operator<<(std::ostream& os, const Signature& sign)
    {
        os << "Signature(" << sign.r << "," << sign.s << ")";
        return os;
    }
    
    std::vector<std::byte> serialize() const;
};


class PrivateKey
{
  private:
    ScalarField secret;
    S256Point publicKey = Genesis * secret.data;

  public:
    static std::string RNG256();
    std::pair<S256Field, ScalarField> sign(uint256 z) const;
};
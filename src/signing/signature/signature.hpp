#pragma once

#include "ECC/secp256k1/S256Field/S256Field.hpp"
#include "signing/scalarField/scalarField.hpp"
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
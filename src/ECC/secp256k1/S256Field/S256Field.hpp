#pragma once
#include "ECC/field_element/field_element.hpp"
#include "utils/hexer/hexer.hpp"

const uint256 P = dehexify(
    "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");

class S256Field : public FieldElement
{
  public:
    S256Field(uint256 data) : FieldElement(data % P, P) {}
    S256Field(int data) : FieldElement(uint256(data) % P, P) {}
    S256Field(std::string hexcode) : FieldElement(dehexify(hexcode) % P, P) {}
    S256Field(const char* hexcode) : FieldElement(dehexify(hexcode) % P, P) {}
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
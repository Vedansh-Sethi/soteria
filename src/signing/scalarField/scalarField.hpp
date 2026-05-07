#pragma once
#include "ECC/field_element/field_element.hpp"
#include "utils/hexer/hexer.hpp"

const uint256 N = dehexify(
    "0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");

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
#include "ECC/secp256k1/S256Field/S256Field.hpp"

S256Field S256Field::sqrt() const
{
    return this->pow((prime + 1) / 4);
}
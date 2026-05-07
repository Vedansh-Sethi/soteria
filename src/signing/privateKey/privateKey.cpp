#include "signing/privateKey/privateKey.hpp"
#include "ECC/secp256k1/S256Field/S256Field.hpp"
#include "signing/scalarField/scalarField.hpp"

std::string PrivateKey::RNG256()
{
    const std::string hexer = "0123456789abcdef";
    std::string hexedRandom = "0x";
    for (int i = 0; i < 64; i++)
    {
        hexedRandom += hexer[std::rand() % 16];
    }
    return hexedRandom;
}

std::pair<S256Field, ScalarField> PrivateKey::sign(uint256 z) const
{
    ScalarField zField = ScalarField(z);
    ScalarField k = dehexify(RNG256());
    S256Field r = (Genesis * k).x.value().data;
    ScalarField s = (zField + secret * ScalarField(r.data)) / k;
    if(s.data > N/2)
    {
        s.data = N - s.data;
    }
    return std::pair<S256Field, ScalarField>(r, s);
}

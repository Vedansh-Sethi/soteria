#pragma once
#include "ECC/secp256k1/S256Point/S256Point.hpp"
#include "signing/scalarField/scalarField.hpp"

class PrivateKey
{
  private:
    ScalarField secret;
    S256Point publicKey = Genesis * secret.data;

  public:
    static std::string RNG256();
    std::pair<S256Field, ScalarField> sign(uint256 z) const;
};

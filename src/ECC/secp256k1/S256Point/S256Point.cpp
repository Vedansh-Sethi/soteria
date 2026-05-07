#include "ECC/secp256k1/S256Point/S256Point.hpp"
#include "signing/scalarField/scalarField.hpp"

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

    // ECDSA rules require checking both r and s are non-zero
    if (s.data == 0 || r.data == 0)
        return false;

    ScalarField u = zField / s;
    ScalarField v = ScalarField(r.data) / s;

    // Evaluate the target point
    S256Point target = Genesis * u.data + *this * v.data;

    // Safety Catch: If the target is Infinity, the signature is invalid
    if (!target.x.has_value())
        return false;

    // The moment of truth
    return target.x.value() == r;
}
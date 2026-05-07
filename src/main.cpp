
#include "ECC/secp256k1/S256Field/S256Field.hpp"
#include "ECC/secp256k1/S256Point/S256Point.hpp"
#include "signing/privateKey/privateKey.hpp"
#include "signing/scalarField/scalarField.hpp"
#include "utils/hexer/hexer.hpp"
#include "utils/sha256/sha256.hpp"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <pthread.h>

int main()
{
    SHA256 *instance = SHA256::GetInstance();
    uint256 e = dehexify(instance->sha256("my secret"));
    uint256 z = dehexify(instance->sha256("my message"));
    uint256 k = 1234567890;
    S256Field r = (Genesis * k).x.value();
    ScalarField s = (ScalarField(z) + ScalarField(r.data) * ScalarField(e)) /
                    ScalarField(k);
    S256Point publicKey = Genesis * e;
    Signature sign = Signature(r, s);
    std::cout << publicKey.verify(z, sign) << std::endl;
}
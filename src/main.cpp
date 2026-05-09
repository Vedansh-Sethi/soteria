
#include "crypto/secp256k1.hpp"
#include "crypto/signature.hpp"
#include "utils/hexer.hpp"

int main()
{   
    S256Field r = "37206a0610995c58074999cb9767b87af4c4978db68c06e8e6e81d282047a7c6";
    ScalarField s = "8ca63759c1157ebeaec0d03cecca119fc9a75bf8e6d0fa65c841c8e2738cdaec";

    Signature sign{r, s};

    std::cout << hexifyBytes(sign.serialize()) << std::endl;
}
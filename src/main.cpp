#include "ECC/ECP/ECP.hpp"
#include "ECC/field_element/field_element.hpp"
#include "utils/hexer/hexer.hpp"

int main()
{
    uint256 prime = dehexify(
        "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    FieldElement a = FieldElement(0, prime);
    FieldElement b = FieldElement(7, prime);
    uint256 gx = dehexify(
        "0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798");
    uint256 gy = dehexify(
        "0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8");
    FieldElement gxFE = FieldElement(gx, prime);
    FieldElement gyFE = FieldElement(gy, prime);
    FFECP genesis = FFECP(a, b, gxFE, gyFE);
    uint256 n = dehexify(
        "0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");
    
    std::cout << genesis * n << std::endl;
}
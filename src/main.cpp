#include <iostream>
#include <optional>
#include "utils/ECP/ECP.hpp"
#include "utils/field_element/field_element.hpp"

int main()
{
    uint256 prime = 223;
    FieldElement a = FieldElement(0, prime);
    FieldElement b = FieldElement(7, prime);
    std::cout << a + b << std::endl;

    std::optional<FieldElement> x1 = std::make_optional<FieldElement>(192, prime);
    std::optional<FieldElement> y1 = std::make_optional<FieldElement>(105, prime);
    std::optional<FieldElement> x2 = std::make_optional<FieldElement>(17, prime);
    std::optional<FieldElement> y2 = std::make_optional<FieldElement>(56, prime);

    FFECP p1 = FFECP(a, b, x1, y1);
    FFECP p2 = FFECP(a, b, x2, y2);

    std::cout << p1 + p2 << std::endl;
}

#include "ECC/field_element/field_element.hpp"
#include <ostream>
#include <stdexcept>
#include <sys/types.h>

std::ostream& operator<<(std::ostream& os, const FieldElement& fe)
{
    return os << "FieldElement_" << fe.prime << "(" << fe.data << ")";
}

bool FieldElement::operator==(const FieldElement &other) const
{
    if (other.prime != prime)
        return false;
    if (other.data != data)
        return false;
    return true;
}

FieldElement FieldElement::operator+(const FieldElement &other) const
{
    if (other.prime != prime)
    {
        std::string error = "cannot add two numbers in different fields";
        throw std::invalid_argument(error);
    }
    uint512 result = (uint512(other.data) + uint512(data)) % prime;
    return FieldElement(uint256(result), prime);
}

FieldElement FieldElement::operator-(const FieldElement &other) const
{
    if (other.prime != prime)
    {
        std::string error = "cannot subtract two numbers in different fields";
        throw std::invalid_argument(error);
    }
    uint512 bigInt = uint512(prime) + uint512(data);
    uint512 result = (bigInt - other.data) % prime;
    return FieldElement(uint256(result), prime);
}

FieldElement FieldElement::pow(uint256 exponent) const
{
    uint512 result = 1;
    uint512 base = this->data;
    while(exponent > 0)
    {
        if(exponent & 1)
        {
            result = (result * base) % prime;
        }

        base = (base * base) % prime;
        exponent >>= 1;
    }

    return FieldElement(uint256(result), prime);
}

FieldElement FieldElement::operator*(const FieldElement &other) const
{
    if (other.prime != prime)
    {
        std::string error = "cannot multiply numbers in two different fields";
        throw std::invalid_argument(error);
    }
    uint512 result = (uint512(other.data) * uint512(data)) % prime;

    return FieldElement(uint256(result), prime);
}

FieldElement FieldElement::operator*(const uint256 &other) const
{
    uint512 bigInt = uint512(data) * uint512(other);
    uint512 result = bigInt % prime;
    return FieldElement(uint256(result), prime);
}

FieldElement FieldElement::operator/(const FieldElement &other) const
{
    if (other.prime != prime)
    {
        std::string error = "cannot divide numbers in two different fields";
        throw std::invalid_argument(error);
    }

    if (other.data == 0)
    {
        std::string error = "cannot divide by 0";
        throw std::invalid_argument(error);
    }

    FieldElement inv = other.pow(prime - 2);

    return (*this)*inv;
}
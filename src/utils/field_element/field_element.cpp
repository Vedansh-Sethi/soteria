#include "utils/field_element/field_element.hpp"
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
    uint256 result = (other.data + data) % prime;
    return FieldElement(result, prime);
}

FieldElement FieldElement::operator-(const FieldElement &other) const
{
    if (other.prime != prime)
    {
        std::string error = "cannot subtract two numbers in different fields";
        throw std::invalid_argument(error);
    }
    return FieldElement((prime + data - other.data) % prime, prime);
}

FieldElement FieldElement::pow(uint256 exponent) const
{
    exponent %= prime - 1;
    uint256 result = 1;
    uint256 base = data;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
        {
            result = uint256((uint512(result) * base) % prime);
            exponent--;
        }

        base = uint256(uint512((base)*base) % prime);
        exponent /= 2;
    }

    return FieldElement(result, prime);
}

FieldElement FieldElement::operator*(const FieldElement &other) const
{
    if (other.prime != prime)
    {
        std::string error = "cannot multiply numbers in two different fields";
        throw std::invalid_argument(error);
    }

    uint256 result = uint256((uint512(other.data) * data) % prime);

    return FieldElement(result, prime);
}

FieldElement FieldElement::operator*(const int &other) const
{
    return FieldElement(uint256((uint512(data) *  other)% prime), prime);
}

FieldElement FieldElement::operator*(const uint &other) const
{
    return FieldElement(uint256((uint512(data) *  other)% prime), prime);
}

FieldElement FieldElement::operator*(const uint256 &other) const
{
    return FieldElement(uint256((uint512(data) *  other)% prime), prime);
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
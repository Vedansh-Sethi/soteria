#pragma once
#include "math/uint256.hpp"
#include <stdexcept>
#include <string>

class FieldElement 
{
    public:
        uint256 data;
        uint256 prime;
        FieldElement(uint256 data,uint256 prime)
        {
            if(data >= prime || data < 0)
            {
                std::string error = "data " + data.str() + " is not in field range 0 to " + prime.str();
                throw std::invalid_argument(error);
            }
            this->data = data;
            this->prime = prime;
        }

        bool operator==(const FieldElement& other) const;
        FieldElement operator+(const FieldElement& other) const;
        FieldElement operator-(const FieldElement& other) const;
        FieldElement operator*(const FieldElement& other) const;
        FieldElement operator*(const uint256& other) const;
        FieldElement pow(uint256 exponent) const;
        FieldElement operator/(const FieldElement& other) const;

        friend std::ostream& operator<<(std::ostream& os, const FieldElement& fe);
};
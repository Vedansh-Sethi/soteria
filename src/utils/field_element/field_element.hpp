#pragma once
#include "utils/uint256/uint256.hpp"
#include <stdexcept>
#include <string>

class FieldElement 
{
    private:
        uint256 data;
        uint256 prime;
    public:
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

        std::string toString() const;
        bool operator==(const FieldElement& other) const;
        FieldElement operator+(const FieldElement& other) const;
        FieldElement operator-(const FieldElement& other) const;
        FieldElement operator*(const FieldElement& other) const;
        FieldElement pow(uint256 exponent) const;
        FieldElement operator/(const FieldElement& other) const;
};
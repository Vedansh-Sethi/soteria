#pragma once
#include "utils/field_element/field_element.hpp"
#include <optional>
#include <stdexcept>
#include <variant>

using None = std::monostate;

template <typename T> class GenericECP
{
  private:
    T a;
    T b;
    std::optional<T> x;
    std::optional<T> y;

  public:
    GenericECP(T a, T b, std::optional<T> x, std::optional<T> y)
    {
        if (!x.has_value() && !y.has_value()) return;
        T xVal = x.value();
        T yVal = y.value();
        if (yVal * yVal != xVal*xVal*xVal + a*xVal + b)
        {
            throw std::invalid_argument("Points do not lie on elliptic curve");
        }
        this->a = a;
        this->b = b;
        this-> x = xVal;
        this->y = yVal;
    }

    friend std::ostream &operator<<(std::ostream &os, const GenericECP<T>& ECP)
    {
        if(!ECP.x.has_value())
        {
            return os << "ECP(Infinity)_" << ECP.a << "_" << ECP.b;
        }
        return os << "ECP(" << ECP.x << "," << ECP.y << ")_" << ECP.a << "_" << ECP.b << ")";
    }
    bool operator==(const GenericECP<T> &other) const;
    bool operator!=(const GenericECP<T> &other) const;
    GenericECP<T> operator+(const GenericECP<T> &other) const;
    std::string toString() const;
};

using FFECP = GenericECP<FieldElement>;
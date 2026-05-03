#pragma once
#include "utils/field_element/field_element.hpp"
#include <optional>
#include <stdexcept>
#include <variant>

using None = std::monostate;

template <typename T> class GenericECP
{
  public:
    T a;
    T b;
    std::optional<T> x;
    std::optional<T> y;

    GenericECP(const T &a, const T &b, const std::optional<T> &x,
               const std::optional<T> &y)
        : a(a), b(b), x(std::nullopt), y(std::nullopt)
    {
        if (!x.has_value() && !y.has_value())
            return;
        T xVal = x.value();
        T yVal = y.value();
        if (yVal * yVal != xVal * xVal * xVal + a * xVal + b)
        {
            std::cout << xVal << " " << yVal << std::endl;
            throw std::invalid_argument("point does not lie on elliptic curve");
        }
        this->x = x;
        this->y = y;
    }

    friend std::ostream &operator<<(std::ostream &os, const GenericECP<T> &ECP)
    {
        if (!ECP.x.has_value())
        {
            return os << "ECP(Infinity)_" << ECP.a << "_" << ECP.b;
        }
        return os << "ECP(" << ECP.x.value() << "," << ECP.y.value() << ")_"
                  << ECP.a << "_" << ECP.b << ")";
    }

    bool operator==(const GenericECP<T> &other) const;
    bool operator!=(const GenericECP<T> &other) const;
    GenericECP<T> operator+(const GenericECP<T> &other) const;
    std::string toString() const;
};

using FFECP = GenericECP<FieldElement>;
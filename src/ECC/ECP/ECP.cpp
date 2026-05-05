#include "ECC/ECP/ECP.hpp"
#include "ECC/field_element/field_element.hpp"
#include <optional>
#include <ostream>
#include <sstream>
#include <stdexcept>

template <typename T>
bool GenericECP<T>::operator==(const GenericECP<T> &other) const
{
    return (other.x == x) && (other.y == y) && (other.a == a) && (other.b == b);
}

template <typename T>
bool GenericECP<T>::operator!=(const GenericECP<T> &other) const
{
    return !(other == *this);
}

template <typename T>
GenericECP<T> GenericECP<T>::operator+(const GenericECP<T> &other) const
{
    if (other.a != a || other.b != b)
    {
        std::stringstream error;
        error << "Points " << this << " , " << other
              << " do not lie on the same curve";
        throw std::invalid_argument(error.str());
    }

    if (!this->x.has_value())
        return other;
    if (!other.x.has_value())
        return *this;

    T xVal = x.value();
    T yVal = y.value();

    if (other == *this)
    {
        if (x == std::nullopt)
            return GenericECP<T>(a, b, std::nullopt, std::nullopt);

        if (y.value() == x.value() * 0)
            return GenericECP<T>(a, b, std::nullopt, std::nullopt);

        T m = (xVal * xVal * 3 + a) / (yVal * 2);
        T resultX = m * m - (xVal * 2);
        T resultY = (xVal - resultX) * m - yVal;
        return GenericECP<T>(a, b, resultX, resultY);
    }

    if(x.value() == other.x.value() && y.value() != other.y.value())
        return GenericECP<T>(a, b, std::nullopt, std::nullopt);

    T m = (other.y.value() - yVal) / (other.x.value() - xVal);
    T resultX = m * m - other.x.value() - xVal;
    T resultY = m * (other.x.value() - resultX) - other.y.value();

    return GenericECP<T>(a, b, resultX, resultY);
}

template <typename T> void GenericECP<T>::operator+=(const GenericECP<T> &other)
{
    *this = *this + other;
}

template <typename T>
GenericECP<T> GenericECP<T>::operator*(uint256 coeff) const
{   
    GenericECP<T> mult = *this;
    GenericECP<T> result = GenericECP<T>(a, b, std::nullopt, std::nullopt);
    while (coeff > 0)
    {
        if (coeff & 1)
        {
            result = result + mult;
        }

        mult = mult + mult;
        coeff >>= 1;
    }

    return result;
}

template class GenericECP<FieldElement>;
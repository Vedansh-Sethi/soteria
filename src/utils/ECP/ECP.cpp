#include "utils/ECP/ECP.hpp"
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

    if(!this->x.has_value()) return other;
    if(!other.x.has_value()) return this;

    if (other == this)
    {
        if (x == std::nullopt)
            return GenericECP<T>(a, b, std::nullopt, std::nullopt);

        if (y == 0 * x)
            return GenericECP<T>(a, b, std::nullopt, std::nullopt);

        T m = (3 * x * x + a) / (2 * y);
        T resultX = m * m - 2 * x;
        T resultY = m(x - resultX) - y;
        return GenericECP<T>(a, b, resultX, resultY);
    }

    T m = (other.y - y) / (other.x - x);
    T resultX = m * m - other.x - x;
    T resultY = m(other.x - resultX) - other.y;

    return GenericECP<T>(a, b, resultX, resultY);
}
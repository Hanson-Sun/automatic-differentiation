#pragma once
#include <vector>
#include <functional>
#include <cmath>

namespace AD {

template<typename T = double>
struct Dual {
    T real;
    T dual;
    constexpr Dual() : real(0), dual(0) {}
    constexpr Dual(T real) : real(real), dual(0) {}
    constexpr Dual(T real, T dual) : real(real), dual(dual) {}
    constexpr Dual(const Dual&) = default;
    constexpr Dual(Dual&&) = default;
    Dual& operator=(const Dual&) = default;
    Dual& operator=(Dual&&) = default;
    ~Dual() = default;
};

template<typename T = double>
inline constexpr Dual<T> zero(T(0), T(0));

template<typename T = double>
inline constexpr Dual<T> one(T(1), T(0));

template<typename T = double>
inline constexpr Dual<T> minus(T(-1), T(0));

template<typename T = double>
Dual<T> operator+(const Dual<T>& a, const Dual<T>& b) {
    return {a.real + b.real, a.dual + b.dual};
}

template<typename T = double>
inline Dual<T> operator+=(Dual<T>& a, const Dual<T>& b) {
    a.real += b.real;
    a.dual += b.dual;
    return a;
}

template<typename T = double>
inline Dual<T> operator-(const Dual<T>& a, const Dual<T>& b) {
    return {a.real - b.real, a.dual - b.dual};
}

template<typename T = double>
inline Dual<T> operator-=(Dual<T>& a, const Dual<T>& b) {
    a.real -= b.real;
    a.dual -= b.dual;
    return a;
}

template<typename T = double>
inline Dual<T> operator*(const Dual<T>& a, const Dual<T>& b) {
    return {a.real * b.real, a.real * b.dual + a.dual * b.real};
}

template<typename T = double>
inline Dual<T> operator*=(Dual<T>& a, const Dual<T>& b) {
    a.real *= b.real;
    a.dual = a.real * b.dual + a.dual * b.real;
    return a;
}

template<typename T = double>
inline Dual<T> operator/(const Dual<T>& a, const Dual<T>& b) {
    return {a.real / b.real, (a.dual * b.real - a.real * b.dual) / (b.real * b.real)};
}

template<typename T = double>
inline Dual<T> operator/=(Dual<T>& a, const Dual<T>& b) {
    a.real /= b.real;
    a.dual = (a.dual * b.real - a.real * b.dual) / (b.real * b.real);
    return a;
}

template<typename T = double, T(*logFunc)(T) = std::log>
inline Dual<T> log(const Dual<T>& a) {
    return {logFunc(a.real), a.dual / a.real};
}

template<typename T = double, T(*expFunc)(T) = std::exp>
inline Dual<T> exp(const Dual<T>& a) {
    double e = expFunc(a.real);
    return {e, a.dual * e};
}

template<typename T = double, T(*logFunc)(T) = std::exp, T(*powFunc)(T, T) = std::pow>
inline Dual<T> pow(const Dual<T>& a, const Dual<T>& b) {
    double p = powFunc(a.real, b.real);
    return {p, p * (b.dual * logFunc(a.real) + b.real * a.dual / a.real)};
}

template<typename T = double, T(*sinFunc)(T) = std::sin, T(*cosFunc)(T) = std::cos>
inline Dual<T> sin(const Dual<T>& a) {
    return {sinFunc(a.real), a.dual * cosFunc(a.real)};
}

template<typename T = double, T(*sinFunc)(T) = std::sin, T(*cosFunc)(T) = std::cos>
inline Dual<T> cos(const Dual<T>& a) {
    return {cosFunc(a.real), -a.dual * sinFunc(a.real)};
}

template<typename T = double, T(*tanFunc)(T) = std::tan, T(*cosFunc)(T) = std::cos>
inline Dual<T> tan(const Dual<T>& a) {
    double t = tanFunc(a.real);
    return {t, a.dual / (cosFunc(a.real) * cosFunc(a.real))};
}

template<typename T = double, T(*asinFunc)(T) = std::asin, T(*sqrtFunc)(T) = std::sqrt>
inline Dual<T> asin(const Dual<T>& a) {
    return {asinFunc(a.real), a.dual / sqrtFunc(1 - a.real * a.real)};
}

template<typename T = double, T(*acosFunc)(T) = std::acos, T(*sqrtFunc)(T) = std::sqrt>
inline Dual<T> acos(const Dual<T>& a) {
    return {acosFunc(a.real), -a.dual / sqrtFunc(1 - a.real * a.real)};
}

template<typename T = double, T(*atanFunc)(T) = std::atan>
inline Dual<T> atan(const Dual<T>& a) {
    return {atanFunc(a.real), a.dual / (1 + a.real * a.real)};
}

template<typename T = double, typename F>
std::vector<double> gradient(F&& f, const std::vector<double>& x) {
    std::vector<Dual<T>> dx;
    for (double xi : x) {
        dx.emplace_back(xi, 1);
    }

    std::vector<double> grad(x.size());
    for (int i = 0; i < x.size(); i++) {
        dx[i].dual = 1;
        grad[i] = f(dx).dual;
        dx[i].dual = 0;
    }

    return grad;
}

};  // namespace AD

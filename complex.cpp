#include "complex.h"

Complex::Complex(double r, double i) : real(r), imag(i) {}

Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::operator*(double constant) const {
    return Complex(real * constant, imag * constant);
}

double Complex::absolute() const {
    return std::sqrt(real * real + imag * imag);
}

void Complex::display() const {
    std::cout << real << " + " << imag << "i";
}

#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include "src/Lightning/namespace/lightning.hpp"
#include <ostream>


/*!
 * @brief Complex numbers implementation
 */
template<typename T>
class lightning::Complex {
public:
	explicit Complex(void);

	explicit Complex(T, T);

	friend std::ostream& operator<<(std::ostream& out, const Complex<T>& expr) {
		if (expr.real == 0) {
			out << "(" << expr.image << "i" << ")";
		} else {
			out << "(" << expr.real << " + " << expr.image << "i" << ")";
		}
		return out;
	}

	Complex<T>& operator+=(const Complex<T>&) noexcept;

	Complex<T> operator+(const Complex<T>&) noexcept;

	Complex<T>& operator-=(const Complex<T>&) noexcept;

	Complex<T> operator-(const Complex<T>&) noexcept;

	Complex<T>& operator*=(const Complex<T>&) noexcept;

	Complex<T> operator*(const Complex<T>&) noexcept;

	Complex<T>& operator/=(const Complex<T>&) noexcept;

	Complex<T> operator/(const Complex<T>&) noexcept;

	bool operator==(const Complex<T>&) noexcept;

	bool operator!=(const Complex<T>&) noexcept;

private:
	T real;
	T image;
};

#include "complex.tcc"

#endif // COMPLEX_HPP

#include "src/Lightning/Complex/complex.hpp"

using lightning::Complex;

template<typename T>
	Complex<T>::Complex() {
		real = 0.0;
		image = 0.0;
	}

template<typename T>
	Complex<T>::Complex(T m_real, T m_image) {
		real = m_real;
		image = m_image;
	}

template<typename T>
	Complex<T>& Complex<T>::operator+=(const Complex<T>& expr) noexcept {
		this->real += expr.real;
		this->image += expr.image;
		return *this;
	}

template<typename T>
	Complex<T> Complex<T>::operator+(const Complex<T>& expr) noexcept {
		return Complex<T>(*this) += expr;
	}

template<typename T>
	Complex<T>& Complex<T>::operator-=(const Complex<T>& expr) noexcept {
		this->real -= expr.real;
		this->image -= expr.image;
		return *this;
	}

template<typename T>
	Complex<T> Complex<T>::operator-(const Complex<T>& expr) noexcept {
		return Complex<T>(*this) -= expr;
	}

template<typename T>
	Complex<T>& Complex<T>::operator*=(const Complex<T>& expr) noexcept {
		T resReal  = (this->real * expr.real) - (this->image * expr.image);
		T resImage = (this->image * expr.real) + (this->real * expr.image);
		real = resReal;
		image = resImage;
		return *this;
	}

template<typename T>
	Complex<T> Complex<T>::operator*(const Complex<T>& expr) noexcept {
		return Complex<T>(*this) *= expr;
	}

template<typename T>
	Complex<T>& Complex<T>::operator/=(const Complex<T>& expr) noexcept {
    T resReal  =  (((this->real  * expr.real) + (this->image * expr.image)) /
             (       (expr.real  * expr.real) + (expr.image * expr.image)));
		T resImage =  (((this->image * expr.real) - (this->real * expr.image)) /
                    ((expr.real  * expr.real) + (expr.image * expr.image)));
		real = resReal;
		image = resImage;
		return *this;
	}

template<typename T>
	Complex<T> Complex<T>::operator/(const Complex<T>& expr) noexcept {
		return Complex<T>(*this) /= expr;
	}

template<typename T>
	bool Complex<T>::operator==(const Complex<T>& expr) noexcept {
		return ((this->real == expr.real) &&
				(this->image == expr.image));
	}

template<typename T>
	bool Complex<T>::operator!=(const Complex<T>& expr) noexcept {
		return !(*this == expr);
	}

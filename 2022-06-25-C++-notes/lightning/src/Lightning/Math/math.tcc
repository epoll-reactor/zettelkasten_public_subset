#include "math.hpp"

#include <cmath>

using lightning::Math;

template <typename T>
	auto Math::fabs(const T expr) noexcept -> T {
		return expr < 0 ? -expr : expr;
	}

template <typename T>
	auto Math::max(const T expr1,
		const T expr2) noexcept -> T {
		return expr2 ^ ((expr2 ^ expr1) & -(expr2 < expr1));
	}

template <typename T>
	auto Math::min(const T expr1,
		const T expr2) noexcept -> T {
		return expr1 ^ ((expr2 ^ expr1) & -(expr2 < expr1));
	}

template <typename T>
	auto Math::roundDown(const T expr) noexcept -> T {
		return (int) expr;
	}

template <typename T>
	auto Math::roundUp(const T expr) noexcept -> T {
		if (expr == (int) expr) {
			return expr;
		} else {
			return (int) expr + 1;
		}
	}

template <typename T>
	auto Math::root(const T base,
		const T expr) noexcept -> T {
		double x = base * 0.5;
		double dx = (base / power(x, expr - 1) - x) / expr;
		while (dx >= 0.000001 || dx <= -0.000001) {
			x += dx;
			dx = (base / power(x, expr - 1) - x) / expr;
		}
		return x;
	}

template <typename T>
	auto Math::squareRoot(const T expr) noexcept -> T {
		return Math::root < float > (expr, 2);
	}

template <typename T>
	auto Math::power(const T expr,
		const short extent) noexcept -> T {
		short step = 0;
		T result;
		for (result = 1; step < (T) extent; step++) {
			result *= expr;
		}
		return result;
	}

template <typename T>
	auto Math::sin(const T expr) noexcept -> T {
		double sin = expr;
		double pow = expr;
		double acc = 1;
		double fact = 1;
		int i = 1;
		while (fabs(acc) > 0.000001 && i < 100) {
			fact *= (2 * i) * (2 * i + 1);
			pow *= -Math::power(expr, 2);
			acc = pow / fact;
			sin += acc;
			i++;
		}
		return sin;
	}

template <typename T>
	auto Math::cos(const T expr) noexcept -> T {
		return Math::sin(expr + 3.14159265 / 2);
	}

template <typename T>
	auto Math::tg(const T expr) noexcept -> T {
		return sin(expr) / cos(expr);
	}

template <typename T>
	auto Math::ctg(const T expr) noexcept -> T {
		return cos(expr) / sin(expr);
	}

template <typename T>
	auto Math::log(T expr) noexcept -> T {
		int radix = 0;
		if (expr == 0.0) {
			return 0.0;
		}
		if (expr == 1.0) {
			return 0;
		} else if (expr < 0.1) {
			while (expr < 0.1) {
				radix--;
				expr *= 10;
			}
		} else {
			while (expr > 10) {
				radix++;
				expr /= 10;
			}
		}
		float sqrt_10 = 3.1622776f;
		float r = (expr - sqrt_10) / (expr + sqrt_10);
		float r2 = r * r;
		float mantissa = (((0.21139497f * r2 + 0.15361371f) * r2 + 0.29115068f) * r2 + 0.86855434f) * r + 0.5f;
		return (float)(radix + mantissa);
	}

template <typename T>
	auto Math::ln(const T expr) noexcept -> T {
		double log_e = 0.43429448190f;
		return log(expr) / log_e;
	}

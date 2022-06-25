#ifndef MATH_HPP
#define MATH_HPP

#include "src/Lightning/namespace/lightning.hpp"


/*!
 * @brief Math class -
 * implementation of the most commonly used mathematical algorithms
 */
class lightning::Math {
public:
	template<typename T>
	static auto power(const T expr, const short extent) noexcept -> T;

	template<typename T>
	static auto fabs(const T expr) noexcept -> T;

	template<typename T>
	static auto max(const T expr1, const T expr2) noexcept -> T;

	template<typename T>
	static auto min(const T expr1, const T expr2) noexcept -> T;

	template<typename T>
	static auto roundDown(const T expr) noexcept -> T;

	template<typename T>
	static auto roundUp(const T expr) noexcept -> T;

	template<typename T>
	static auto root(const T base, const T expr) noexcept -> T;

	template<typename T>
	static auto squareRoot(const T expr) noexcept -> T;

	template<typename T>
	static auto sin(const T expr) noexcept -> T;

	template<typename T>
	static auto cos(const T expr) noexcept -> T;

	template<typename T>
	static auto tg(const T expr) noexcept -> T;

	template<typename T>
	static auto ctg(const T expr) noexcept -> T;

	template<typename T>
	static auto log(T expr) noexcept -> T;

	template<typename T>
	static auto ln(T expr) noexcept -> T;

};

#include "math.tcc"

#endif // MATH_HPP

#ifndef __LINE_H
#define __LINE_H

typedef unsigned short s16;

#include <ostream>
#include <istream>
#include <string.h> ///< strcat, strcmp, ...
#include "src/Lightning/namespace/lightning.hpp"
#include "boost/array.hpp"

/*!
 * @brief Line represents class to work
 * with text strings.
 */
class lightning::Line {
public:
	explicit Line(){};

	/*!
	 * @brief Constructor copies @param to
	 * private field "str".
	 */
	Line
	(const char*);

	Line
	(const Line&&);

	Line
	(const Line&);

	~Line();

	/*!
	 * @brief overloading default output stream operator.
	 * @return std::basic_ostream<char>&
	 */
	friend std::ostream&
	operator<<(std::ostream&, const Line&) noexcept;

	/*!
	 * @brief overloading default input stream operator.
	 * @return std::basic_istream<char>&
	 */
	friend std::istream&
	operator>>(std::istream&, Line&) noexcept;

	/*!
	 * @return result of concatenation.
	 * this->str with @param
	 */
	char*
	operator+(const Line&) noexcept;

	/*!
	 * @return result of concatenation.
	 * this->str with @param
	 */
	char*
	operator+(const char*) noexcept;

	bool
	operator==(const Line&) noexcept;

	bool
	operator==(const char*) noexcept;

	bool
	operator!=(const Line&) noexcept;

	bool
	operator!=(const char*) noexcept;

	/*!
	 * @brief concatenate this->str with Line.str
	 * and save result to this->str.
	 */
	void
	operator+=(const Line&) noexcept;

	/*!
	 * @brief concatenate this->str with char array
	 * and save result to this->str.
	 */
	void
	operator+=(const char*) noexcept;

	void operator=(Line&);

	void operator=(const char*);

	char*
	to_upper() noexcept;

	char*
	to_lower() noexcept;

	char*
	reverse() noexcept;

	void
	move(const Line&);

	/*!
	 * @brief to_char_array method needed
	 * for the Regex class,
	 * which accepts char* as a @param
	 * in Regex::match_exists.
	 * @return this->str
	 */
	char*
	to_char_array() noexcept;

//	/*!
//	 * @brief replace method cannot get
//	 * FIRST @param == SECOND @param
//	 * to avoid waste of resources by replacing
//	 * text with itself.
//	 */
//	char*
//	replace(const char* __restrict__,
//			const char* __restrict__) noexcept;

//	char*
//	cut(const char*) noexcept;

	s16
	length() noexcept;

	/*!
	 * @return empty string
	 */
	const char*
	clear() noexcept;

private:
	char* str;
};
#endif //__LINE_H

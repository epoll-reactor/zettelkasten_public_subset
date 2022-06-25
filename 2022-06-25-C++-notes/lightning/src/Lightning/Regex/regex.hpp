#define __REGEX_H
#ifdef  __REGEX_H

#include <ctype.h> ///< isdigit, isspace, isalpha
#include "src/Lightning/namespace/lightning.hpp"

/*!
 * @brief Regex represents the class
 * which allows to find match in text
 */
class lightning::Regex {
public:
	static bool match_exists(const char* regex, char* string);

private:
	static inline int strlen(char* string);

	static inline bool match_group(int ch, int group);

	static inline bool match_quantity(const char* regex, char* string, int quant, int ch, int* len);
};
#endif//__REGEX_H

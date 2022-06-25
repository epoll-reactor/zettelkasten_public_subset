#include "src/Lightning/Regex/regex.hpp"

int lightning::Regex::strlen(char *string)
{
	int length = 0;
	while (*string++ != '\0') {
		++length;
	}
	return length;
}

bool lightning::Regex::match_exists(const char* regex, char* string)
{
	unsigned int ch = regex[0];
	if (regex[0] == '\0') {
		return true;
	}
	if	(regex[0] == '\\' &&
		 regex[1] != '\0')
	{
		ch = regex[1];
		if (ch != '\\' &&
		    ch != '^'  &&
		    ch != '$'  &&
		    ch != '+'  &&
		    ch != '*'  &&
		    ch != '?')
		{
		    ch |= 0x100;
		}
		regex++;
	}
	int q = regex[1];
	if (q == '*' ||
	    q == '+' ||
	    q == '-' ||
		q == '?')
	{
		int len = strlen(string);
		return match_quantity(regex+2, string, q, ch, &len);
	}
	else if (match_group(*string, ch)) {
		++string;
		return match_exists(++regex, string);
	}
	return 0;
}

bool lightning::Regex::match_group(int ch, int group)
{
	if ((group & 0xFF) == '.') {
		group ^= 0x100;
	}
	if (group < 0x100) {
		return ch == group;
	}
	switch (group & 0xFF) {
		case '.': return  true;
		case 'd': return  isdigit(ch);
		case 'D': return !isdigit(ch);
		case 's': return  isspace(ch);
		case 'S': return !isspace(ch);
		case 'w': return (isalpha(ch) ||
		                  isdigit(ch));
		case 'W': return!(isalpha(ch) ||
		                  isdigit(ch));
	}
	return 0;
}
#include <stdio.h>

bool lightning::Regex::match_quantity(const char* regex, char* string, int quant, int ch, int* len)
{
	if (quant == '?') {
		if (match_group(*string, ch)) {
			++string;
			++len;
		}
		return match_exists(regex, string);
	}

    if (quant == '$' &&
        string[*len] != 0) {
//        return match_exists(regex, string);
//        ???
    }

    if (quant == '+' ||
		quant == '*')
	{
		char *p = string;
		for ( ; *p != '\0' && match_group(*p, ch); ++p) {
			++len;
		}

		if (quant == '+' &&
			p     == string) {
		return 0;
		}
			do {
				if (Regex::match_exists(regex, p)) {
					return 1;
				}
			} while (p-- > string);
	}
	return 0;
}

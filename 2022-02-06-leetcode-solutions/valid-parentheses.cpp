/* Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
 * An input string is valid if:
 *
 * Open brackets must be closed by the same type of brackets.
 * Open brackets must be closed in the correct order.
 *
 * Example:
 * Input: s = "()"
 * Output: true
 * 
 * Input: s = "(]"
 * Output: false
 */

#include <iostream>
#include <cassert>
#include <stack>

bool is_valid(std::string input)
{
	std::stack<char> parens;
	for (char c : input)
	{
		switch (c)
		{
		case '(':
		case '{':
		case '[':
			parens.push(c);
			break;
		case ')': {
			if (parens.empty() || parens.top() != '(') return false;
			parens.pop();
			break;
		}
		case '}': {
			if (parens.empty() || parens.top() != '{') return false;
			parens.pop();
			break;
		}
		case ']': {
			if (parens.empty() || parens.top() != '[') return false;
			parens.pop();
			break;
		}
		}
	}

	return parens.empty();
}

int main()
{
	assert(is_valid("()"));
	assert(is_valid("[]"));
	assert(is_valid("{}"));
	assert(is_valid("{}()[]"));
	assert(!is_valid("{}()]"));
	assert(!is_valid("(]"));
	assert(!is_valid("([)]"));
}

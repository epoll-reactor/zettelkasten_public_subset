#include <unordered_map>
#include <iostream>


int roman_to_int(std::string &&input)
{
	int output{0};
	int ones_reached{0};
	int decimals_reached{0};
	int hundreds_reached{0};

	static const std::unordered_map<char, int> priorities
	{
		{ 'I', 1 },
		{ 'V', 2 },
		{ 'X', 3 },
		{ 'L', 4 },
		{ 'C', 5 },
		{ 'D', 6 },
		{ 'M', 7 },
	};

	for (char symbol : input)
	{
		switch (symbol)
		{
			case 'I': { ++output; ++ones_reached; continue; }
			case 'V': { output +=    5;           break; }
			case 'X': { output +=   10;           break; }
			case 'L': { output +=   50;           break; }
			case 'C': { output +=  100;           break; }
			case 'D': { output +=  500;           break; }
			case 'M': { output += 1000;           break; }
			default:  {                           break; }
		}

		if (ones_reached > 0 && priorities.at(symbol) > priorities.at('I'))
		{
			output -= ones_reached * 2; ones_reached = 0;
		}

		if (decimals_reached > 0 && priorities.at(symbol) > priorities.at('X'))
		{
			output -= (10 * decimals_reached) * 2; decimals_reached = 0;
		}

		if (hundreds_reached > 0 && priorities.at(symbol) > priorities.at('C'))
		{
			output -= (100 * hundreds_reached) * 2; hundreds_reached = 0;
		}

		switch (symbol)
		{
			case 'X': { ++decimals_reached; break; }
			case 'C': { ++hundreds_reached; break; }
			default:  { break; }
		}
	}
	
	return output;
}

int main()
{
	std::cout << roman_to_int("X") << std::endl;
	std::cout << roman_to_int("IX") << std::endl;
	std::cout << roman_to_int("LVIII") << std::endl;
	
	std::cout << roman_to_int("M") << std::endl;
	std::cout << roman_to_int("CM") << std::endl;
	std::cout << roman_to_int("XC") << std::endl;
	std::cout << roman_to_int("IV") << std::endl;
	std::cout << roman_to_int("MCMXCIV") << std::endl;
}

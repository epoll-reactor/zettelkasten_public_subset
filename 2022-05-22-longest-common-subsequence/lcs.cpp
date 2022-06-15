#include <iostream>
#include <vector>

// Najgorszy przypadek - Θ(string.length() * substring.length()).
// https://www.programiz.com/dsa/longest-common-subsequence#:~:text=The%20longest%20common%20subsequence%20(LCS,positions%20within%20the%20original%20sequences.
int compute_longest_common_subsequence(std::string string, std::string substring) {
	int string_len = string.size();
	int substr_len = substring.size();
	int dp[string_len + 1][substr_len + 1];

	// Zera w pierwszej kolumnie (https://en.wikipedia.org/wiki/Longest_common_subsequence_problem#Worked_example).
	for(int i = 0; i <= string_len; i++)
		dp[i][0] = 0;

	// Zera w pierwszym wierszu (https://en.wikipedia.org/wiki/Longest_common_subsequence_problem#Worked_example).
	for(int j = 0; j <= string_len; j++)
		dp[0][j] = 0;

	for(int i = 1; i <= string_len; i++)
		for(int j = 1; j <= substr_len; j++)
			if(string[i - 1] == substring[j - 1])
				// Jezeli mamy taki sam symbol w obu ciagach, to zwiekszamy diagonalny
				// element odnisnie dp[i][j] o 1.
				//   A B       A B
				// A 0 0  -> A 0 0
				// B 0 0     B 0 1
				dp[i][j] = dp[i - 1][j - 1] + 1;
			else
				// inaczej bierzemy do nastepujacej komorki najwieksza wartosc z poprzednio
				// rozpatrzywanego wiersza i kolumny.
				dp[i][j] = std::max(dp[i][j - 1], dp[i - 1][j]);

	// W koncu zwracamy prawy dolny element tabeli, bo to jest nasz wynik.
	return dp[string_len][substr_len];
}

int main() {
	std::cout << compute_longest_common_subsequence("marcinjestsuper", "mister") << std::endl;
}

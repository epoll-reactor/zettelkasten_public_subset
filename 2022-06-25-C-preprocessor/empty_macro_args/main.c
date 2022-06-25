#include <stdio.h>

#define MACRO(_1, _2) _1 _2

int main() {
	printf(MACRO("2 ", "arguments\n"));
	printf(MACRO("1 argument\n", ));
}

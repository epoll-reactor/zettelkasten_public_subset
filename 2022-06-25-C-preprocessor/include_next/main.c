// #include <stdio.h> // /usr/include
// #include "stdio.h" // local
#include_next "stdio.h" // local

int main() {
	return non_stdio_func();
}

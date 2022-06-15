#include <stdio.h> // printf()

// В принципе логика довольно простая. Я нашёл здесь:
// https://www.tutorialspoint.com/tribonacci-numbers-in-cplusplus
void wymien(unsigned long long *a,
            unsigned long long *b,
            unsigned long long *c) {
	unsigned long next = *a + *b + *c;
	*a = *b;
	*b = *c;
	*c = next;
}

unsigned long long trib(int n) {
	int i;
	unsigned long long a = 0, b = 0, c = 1;
	for(i = 0; i < n; i++)
		wymien(&a, &b, &c);
	return a;
}

int main() {
	printf("T50 == %llu\n", trib(50));
	return 0;
}

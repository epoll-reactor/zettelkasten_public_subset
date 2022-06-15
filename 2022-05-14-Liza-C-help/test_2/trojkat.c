#include <stdio.h> // printf()

// Typedef нужно, чтобы не писать struct trojkat_prostokatny при создании
// переменных.
typedef struct {
	int a;
	int b;
	int c;
} trojkat_prostokatny;

double pole(trojkat_prostokatny t) {
	return (t.a * t.b) / 2;
}

int main() {
	// Это может не работать. Тогда можно сделать так:
	// trojkat_prostokatny t1;
	// t1.a = ...;
	// t1.b = ...;
	// t1.c = ...;
	trojkat_prostokatny t1 = {
		.a = 3,
		.b = 4,
		.c = 5
	};
	printf("pole(3, 4, 5)  = %.02f\n", pole(t1));
	trojkat_prostokatny t2 = {
		.a = 6,
		.b = 8,
		.c = 10
	};
	printf("pole(6, 8, 10) = %.02f\n", pole(t2));
}

#include <stdio.h> // printf()

// Не понимаю, что должна делать эта функция, сорян.
int redukuj(int A[], int n, int (*f)(int, int), int start) {
	if (start > n)
		return A[start];

	printf("%d, %d\n", A[start], A[start + 1]);
	return f(f(start, A[start]), A[start + 1]);
}

int dodaj (int x, int y) { return x + y; }
int wymnoz(int x, int y) { return x * y; }

int main() {
	int A[] = { 1, 2, 3, 4 };
	int n = 4;
	// Я не имею понятия, зачем препод посчитал нужным написать касты
	// (эта штука: (int (*)(int, int)).), если в redukuj и так
	// передаётся нужный тип. -_-
	printf("%d\n", redukuj(A, n, dodaj, 0));
	printf("%d\n", redukuj(A, n, wymnoz, 1));
}

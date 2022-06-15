#include <stdio.h> // printf()

struct element {
	int v;
	struct element *nastepny;
};

int liczba_elementow(struct element *eptr) {
	// Структуры данных передают привет.
	int length = 0;

	while (eptr) {
		++length;
		eptr = eptr->nastepny;
	}

	return length;
}

int main() {
	struct element e1 = { .v = 1, NULL};
	struct element e2 = { .v = 2, NULL};
	struct element e3 = { .v = 3, NULL};
	struct element e4 = { .v = 4, NULL};
	struct element e5 = { .v = 5, NULL};

	struct element *eptr = &e1;
	e1.nastepny = &e2;
	e2.nastepny = &e3;
	e3.nastepny = &e4;
	e4.nastepny = &e5;

	printf("\nLiczba elementow: %d\n\n", liczba_elementow(eptr));  
}

#include <stdio.h> // printf(), puts()
#include <stdlib.h> // atof()

int main(int argc, char *argv[])
{
	// Если передан один аргумент (имя программы).
	// Аргументы выглядят так: {имя программы, арг1, арг2, ...}
	if (argc < 2) {
		puts("Uzycie:\n\tsrednia ARG1 ARG2 ... ARGn");
		// Заканчиваем функцию.
		return 1;
	}

	// Здесь храним сумму всех чисел.
	float average = 0.0f;
	int i;

	for (i = 0; i < argc; ++i) {
		// atof() переводит строку в float.
		average += atof(argv[i]);
	}

	// сумма всех чисел / кол-во аргументов (без учёта имени программы).
	printf("%.06f\n", average / (argc - 1));

	return 0;
}

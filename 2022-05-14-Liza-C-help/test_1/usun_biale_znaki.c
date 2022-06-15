#include <stdio.h>  // printf()
#include <string.h> // strlen()
#include <ctype.h>  // isspace()

#define MAXLINE 1000

int pobierz_wiersz(char s[], int lim) {
	int c, i;

	for(i = 0;
		i < lim - 1 && (c = getchar()) != EOF && c != '\n';
		i++) {
		
		s[i] = c;
	}

	if(c == '\n'){
		s[i] = c;
		i++;
	}

	s[i] = '\0';

	return i;
}

int same_biale(char s[]) {
	// Указатель, которым мы бегем по строке
	char *ptr = s;
	// Количество пробелов
	size_t spaces = 0;

	// Пока сам указатель куда-то указывает, и пока
	// символ под ним не равен '\0' (нуль-терминатор)
	while (ptr && *ptr != '\0') {
		// Если мы встречаем пробельный символ, увеличиваем счётчик
		// (isspace смотрит на предмет \n, \r, \v, \t и т.д., в общем,
		// то что в задании)
		if (isspace(*ptr)) {
			++spaces;
		}
		// Сдвигаем указатель вправо на следующий символ в строке
		++ptr;
	}

	// Возвращаем 1, если кол-во пробельных символов равно длине строки
	return spaces == strlen(s);
}

int main() {
	char s[MAXLINE];
	while(pobierz_wiersz(s, MAXLINE) > 0)
		if(!same_biale(s))
			printf("%s", s);
		else
			printf("Same biale\n");
			
	return 0;
}

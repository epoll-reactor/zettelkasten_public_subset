#include <stdio.h>

int main()
{
	int array[10] = {
		[0 ... 5] = 1,
		[6 ... 9] = 2
	};
	int i;
	for (i = 0; i < 10; ++i) {
		printf("%d ", array[i]);
	}
}

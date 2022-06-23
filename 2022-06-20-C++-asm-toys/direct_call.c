int printf(const char *fmt, ...);

void _1(int argument) {
	printf("Argument: %d\n", argument);
}

int main() {
	// Вызывает функцию с вручную переданным параметром через регистр.
	asm(
	"mov   $123, %%edi\n"
	"call  _1"
	: // "=r" ()          output
	: // : "i"(123) //    input constant
	  // , "r"(11111)     input register
	: //                  clobbered register
	);
}

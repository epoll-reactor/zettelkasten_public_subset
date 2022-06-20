extern "C" int printf(const char *fmt, ...);

extern "C" void _1(int argument) {
	printf("Argument: %d\n", argument);
}

// r    specifies that the operand must be a general-purpose register
// m    specifies that the operand must be a memory address
// i    specifies that the operand must be an integer constant
// g    specifies that the operand must be a general-purpose register, or a memory address, or an integer constant (effectively the same as "rmi")
// +reg means that the output is actually a read-write value.
// =reg means that the output is write-only.

int main() {
	// Вызывает функцию с вручную переданным параметром через регистр.
	asm(
	"mov %0, %%edi"
	: // "=r" ()          output
	:   "i"(123) //       input constant
	  // , "r"(11111)     input register
	: "%edi" //           clobbered register
	);
	asm("call _1");
}

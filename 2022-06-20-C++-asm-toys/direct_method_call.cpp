// НЕ РАБОТАЕТ.
extern "C" int printf(const char *fmt, ...);

struct object {
	void method(int argument) {
		printf("Argument: %d\n", argument);
	}
};

int main() {
#if 1
	[[maybe_unused]] object memory;

	asm(
	"lea    %[pMemory], %%rax\n"
	"mov    $0,         %%esi\n"
	"mov    %%rax,      %%rdi\n"
	"call   _ZN6object6methodEi"
	: //                        output
	: [pMemory] "m" (memory) // input
	: //                        clobbered registers
	);

	// asm(
	// "lea    -1(%%rbp), %%rax\n"
	// "mov    $0,       %%esi\n"
	// "mov    %%rax,    %%rdi\n"
	// : //                       output
	// : //                       input
	// : //                       clobbered registers
	// );
#else
	object memory;
	memory.method(0);
#endif
}

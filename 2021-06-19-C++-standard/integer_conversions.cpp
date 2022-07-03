int main() {
	// char 'a' преобразуется в int со значением 97.
	// Разные типы: int и long.
	// Оба значные.
	// long имеет больший ранг, чем int,
	// тогда, int(97) преобразуется к long(97).
	// Результат = long(97) + long(1) = long(98).
	static_assert(((char)'a' + 1L) == 98L);
	// signed int 10 преобразуется к unsigned int 10
	// поскольку операция вычитания выполняется у беззначных
	// чисел, операция выглядит как (2 - 10) modulo (2^n),
	// где n - число бит unsigned int.
	// Тогда, -8 modulo (2^32) = unsigned int(4294967288).
	static_assert((2U - 10) == 4294967288U);
	// Ранг signed long long больше, чем у unsigned long.
	// Если ULONG_MAX > LLONG_MAX, тогда signed long long
	// не может представлять все значения unsigned long.
	// Тогда оба операнда преобразуются к unsigned long long.
	// unsigned long(0) преобразуется к unsigned long long(0).
	// signed long long(1) преобразуется к unsigned long long(1).
	// Операция выглядит как (0 - 1) modulo (2^64)
	// = unsigned long long(18446744073709551615) (ULONG_MAX).
	static_assert((0UL - 1LL) == 18446744073709551615ULL);
}

#include <iostream>
#include <limits>

// IEEE754
int main() {
	static_assert(20000000.f == 20000001.f);
//	static_assert(20000001.f == 20000002.f);
//	static_assert(20000002.f == 20000003.f);
//	Example:
//	              Sign    Exponent        Mantissa
//	20000003.f == [0]     [1001011 1]     [0011000 10010110 10000010]
//	20000004.f == [0]     [1001011 1]     [0011000 10010110 10000010]
	static_assert(20000003.f == 20000004.f);
	static_assert(20000004.f == 20000005.f);
//	static_assert(20000005.f == 20000006.f);
//	static_assert(20000006.f == 20000007.f);
	static_assert(20000007.f == 20000008.f);
	static_assert(20000008.f == 20000009.f);
	static_assert(20000000010.f == 20000000020.f);
	static_assert(20000000010.f == 20000000030.f);
	static_assert(20000000010.f == 20000000040.f);
	static_assert(20000000010.f == 20000000050.f);
	static_assert(20000000010.f == 20000000060.f);
	static_assert(20000000010.f == 20000000070.f);
	static_assert(20000000010.f == 20000000080.f);
	static_assert(20000000010.f == 20000000090.f);
	static_assert(20000000010.f == 20000000100.f);
	static_assert(20000000010.f == 20000001000.f);

	static_assert((1.f + 20000000) == (1.f + 20000001));
//	static_assert((1.f + 20000001) == (1.f + 20000002));
	static_assert((1.f + 20000002) == (1.f + 20000003));
	static_assert((1.f + 20000003) == (1.f + 20000004));
	static_assert((1.f + 20000004) == (1.f + 20000005));
//	static_assert((1.f + 20000005) == (1.f + 20000006));
	static_assert((1.f + 20000006) == (1.f + 20000007));
	static_assert((1.f + 20000007) == (1.f + 20000008));
	static_assert((1.f + 20000008) == (1.f + 20000009));
}

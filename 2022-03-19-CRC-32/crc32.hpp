#include <cstdint>
#include <cstddef>
#include <cassert>

/// Calculate given Element by its "formula" 8 times
/// while Repeat != 0. Every time we pass the computed Element
/// from previous iteration.
template <uint32_t Element, size_t Repeat = 8>
struct crc32_lookup_generator : crc32_lookup_generator<
  ((Element & 1) ? 0xedb88320 : 0) ^ (Element >> 1), Repeat - 1
> {};
/// So when Repeat == 0 we will fall here and recursion will be stopped. :)
template <uint32_t Element>
struct crc32_lookup_generator<Element, 0> {
  enum { value = Element };
};

#define A(x) B(x) B((x) + 128)
#define B(x) C(x) C((x) +  64)
#define C(x) D(x) D((x) +  32)
#define D(x) E(x) E((x) +  16)
#define E(x) F(x) F((x) +   8)
#define F(x) G(x) G((x) +   4)
#define G(x) H(x) H((x) +   2)
#define H(x) I(x) I((x) +   1)
#define I(x) crc32_lookup_generator<x>::value,
/// A(0) expands along the chain into 256 numbers,
/// each of them is CRC-32 constant from range [0, 256].
static constexpr uint32_t crc32_lookup_table[] = {A(0)};

#undef A
#undef B
#undef C
#undef D
#undef E
#undef F
#undef G
#undef H
#undef I

uint32_t crc32_bytes(const uint8_t* buffer, size_t size) noexcept {
  uint32_t crc32 = 0xFFFFFFFF;
  while (size-- > 0) {
    crc32 = (crc32 >> 8) ^ crc32_lookup_table[(crc32 ^ *(buffer++)) & 0xFF];
  }
  return crc32 ^ 0xFFFFFFFF;
}

uint32_t crc32_string(const char* buffer) noexcept {
  assert(buffer && "Null buffer received.");
  uint32_t crc32 = 0xFFFFFFFF;
  do {
    crc32 = (crc32 >> 8) ^ crc32_lookup_table[(crc32 ^ *(buffer++)) & 0xFF];
  } while (*buffer != '\0');
  return crc32 ^ 0xFFFFFFFF;
}

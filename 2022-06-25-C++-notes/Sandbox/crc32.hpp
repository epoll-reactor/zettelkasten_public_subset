template <unsigned _char, unsigned short _size = 8> struct table : table<((_char & 1) ? 0xEDB88320 : 0) ^ (_char >> 1), _size - 1> { };
template <unsigned _char> struct table<_char, 0> { enum { value = _char}; };

#define A(x) B(x) B(x + 128)
#define B(x) C(x) C(x +  64)
#define C(x) D(x) D(x +  32)
#define D(x) E(x) E(x +  16)
#define E(x) F(x) F(x +   8)
#define F(x) G(x) G(x +   4)
#define G(x) H(x) H(x +   2)
#define H(x) I(x) I(x +   1)
#define I(x) table<x>::value,

constexpr unsigned crc32_lookup_table[256] = { A(0) };
template <unsigned _string, char... chars> struct crc32_implementation { };
template <unsigned _string, char _head, char... _tail> struct crc32_implementation<_string, _head, _tail...> {
    static constexpr unsigned value =
        crc32_implementation<crc32_lookup_table[static_cast<unsigned char>(_string) ^ static_cast<unsigned char>(_head)] ^ (_string >> 8), _tail...>::value;
};
template <unsigned _string> struct crc32_implementation<_string> {
    static constexpr unsigned value = _string ^ ~0x0;
};
constexpr unsigned crc32rec(unsigned int crc, const char* src) {
    return *src == 0
        ? crc ^ ~0x0
        : crc32rec(crc32_lookup_table[static_cast<unsigned char>(crc) ^ static_cast<unsigned char>(*src)] ^ (crc >> 8), src + 1);
}
constexpr unsigned constexprcrc32gen(const char* src) {
    return crc32rec(~0x0, src);
}

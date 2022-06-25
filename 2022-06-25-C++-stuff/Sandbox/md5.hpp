// Feb 19, 2021
#include <string>
#include <cstring>
#include <memory>
#include <cstdio>


namespace md5_tables {

static constexpr uint32_t constants_table[] = {
  0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
  0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
  0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
  0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
  0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
  0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
  0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
  0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
  0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static constexpr uint32_t rotate_table[] = {
  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

}// namespace md5_tables

static std::string md5_to_string(uint32_t part1, uint32_t part2, uint32_t part3, uint32_t part4) {
  static constexpr std::string_view digits = "0123456789abcdef";
  std::string hash;
  hash.reserve(32);
  for (uint32_t part : {part1, part2, part3, part4}) {
    for (size_t i = 0; i < 4; ++i) {
      uint8_t byte = (part & (0xFF << (i * 8))) >> (i * 8);
      hash += digits[byte >>  4];
      hash += digits[byte  & 15];
    }
  }
  return hash;
}

static uint32_t rotate_left(uint32_t value, uint32_t count) noexcept {
  return (value << count) | (value >> (32 - count));
}

static size_t md5_align_to_chunk_size(size_t length) noexcept {
  /// length == 100, so 100 / 64 == 1,
  /// 1 + 1 = 2, 2 * 64 = 128,
  /// 128 - 8 = 120
  return ((((length + 8) / 64) + 1) * 64) - 8;
}

static std::unique_ptr<uint8_t[]> md5_create_padded_buffer(const uint8_t* buffer, size_t size) {
  std::unique_ptr<uint8_t[]> bytes(new uint8_t[size + 64]);
  memcpy(&bytes[0], buffer, size);
  memset(&bytes[0] + size, 0, 64);
  bytes[size] = 0b10000000;
  return bytes;
}

static void md5_process_chunk(const uint8_t* buffer, size_t offset, uint32_t* a0, uint32_t* b0, uint32_t* c0, uint32_t* d0) noexcept {
  const uint32_t* chunk = reinterpret_cast<const uint32_t*>(&buffer[0] + offset);
  uint32_t a = *a0;
  uint32_t b = *b0;
  uint32_t c = *c0;
  uint32_t d = *d0;

  for (uint32_t i = 0; i < 64; ++i) {
    uint32_t f, g;
    if (i < 16) {
      f = (b & c) | (~b & d);
      g = i;
    } else if (i < 32) {
      f = (d & b) | (~d & c);
      g = (5 * i + 1) % 16;
    } else if (i < 48) {
      f = b ^ c ^ d;
      g = (3 * i + 5) % 16;
    } else {
      f = c ^ (b | ~d);
      g = (7 * i) % 16;
    }

    const uint32_t temp = d;
    f += a + md5_tables::constants_table[i] + chunk[g];
    d = c;
    c = b;
    b = b + rotate_left(f, md5_tables::rotate_table[i]);
    a = temp;
  }
  *a0 += a;
  *b0 += b;
  *c0 += c;
  *d0 += d;
}

static void md5_impl(const uint8_t* buffer, size_t buffer_size, uint32_t* a0, uint32_t* b0, uint32_t* c0, uint32_t* d0) {
  std::unique_ptr<uint8_t[]> padded_buffer = md5_create_padded_buffer(buffer, buffer_size);
  const size_t new_size = md5_align_to_chunk_size(buffer_size);
  const uint32_t bits = 8 * buffer_size;
  /// Write buffer size to the end of buffer
  padded_buffer[new_size    ] = (bits & 0x000000FF) >>  0;
  padded_buffer[new_size + 1] = (bits & 0x0000FF00) >>  8;
  padded_buffer[new_size + 2] = (bits & 0x00FF0000) >> 16;
  padded_buffer[new_size + 3] = (bits & 0xFF000000) >> 24;

  for (size_t offset = 0; offset < new_size; offset += 64) {
    md5_process_chunk(padded_buffer.get(), offset, a0, b0, c0, d0);
  }
}

std::string md5_bytes(const uint8_t* buffer, size_t buffer_size) {
  uint32_t a0 = 0x67452301;
  uint32_t b0 = 0xefcdab89;
  uint32_t c0 = 0x98badcfe;
  uint32_t d0 = 0x10325476;

  md5_impl(buffer, buffer_size, &a0, &b0, &c0, &d0);
  return md5_to_string(a0, b0, c0, d0);
}

std::string md5_string(std::string_view data) {
  return md5_bytes(reinterpret_cast<const uint8_t*>(data.data()), data.length());
}

void run_md5_tests() {
  auto test = [](std::string hash, std::string expected) {
    if (hash != expected) {
      std::string message;
      message.reserve(128);
      message += "ERROR: expected " + expected;
      message += ", got " + hash;
      throw std::runtime_error(message.c_str());
    }
  };
  test(md5_string(""), "d41d8cd98f00b204e9800998ecf8427e");
  test(md5_string("1"), "c4ca4238a0b923820dcc509a6f75849b");
  test(md5_string("???"), "0d1b08c34858921bc7c662b228acb7ba");
  test(md5_string(":)"), "50585be4e3159a71c874c590d2ba12ec");
  test(md5_string("\xFF"), "00594fd4f42ba43fc1ca0427a0576295");
  test(md5_string("tetetettetetettetetettetetettetetettetetettetetettetetettetetet"), "dd64676148158e3a655e4fa25d5df01e");
  test(md5_string("_________________________________________"), "e70df1451f00e32c7b41ca1b389fe81a");
}

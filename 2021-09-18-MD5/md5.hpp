#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>


typedef struct {
  uint64_t data_length;
  uint32_t state[4];
  uint8_t  data[64];
} md5_ctx_t;

__attribute__((always_inline, const))
static inline uint32_t rotate_left(uint32_t value, uint32_t count) {
  __asm__("rol %1, %0" : "+r" (value) : "cI" (count));
  return value;
}

__attribute__((always_inline, const))
static inline uint32_t md5_ch(uint32_t x, uint32_t y, uint32_t z) {
  return (x & y) ^ (~x & z);
}

void md5_init(md5_ctx_t* ctx) {
  ctx->data_length = 0;
  uint32_t* data = &ctx->state[0];
  *(data++) = 0x67452301;
  *(data++) = 0xefcdab89;
  *(data++) = 0x98badcfe;
  *(data  ) = 0x10325476;
}

inline void md5_transform(md5_ctx_t* ctx, uint32_t* buffer) {
  static const uint32_t shifts_table[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
  };
  static const uint32_t md5_constants[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
  };

  uint32_t i, e, j, tmp, vars[4];

  for (i = 0; i < 4; ++i) {
    vars[i] = ctx->state[i];
  }

  for(i = 0; i < 64; ++i) {
    switch(i / 16) {
      case 0:
        e = md5_ch(vars[1], vars[2], vars[3]);
        j = i;
        break;
      case 1:
        e = (vars[1] & vars[3]) | (vars[2] & ~vars[3]);
        j = ((i * 5) + 1) % 16;
        break;
      case 2:
        e = vars[1] ^ vars[2] ^ vars[3];
        j = ((i * 3) + 5) % 16;
        break;
      default:
        e = vars[2] ^ (vars[1] | ~vars[3]);
        j = (i * 7) % 16;
        break;
    }

    tmp = vars[3];
    vars[3] = vars[2];
    vars[2] = vars[1];
    vars[1] = vars[1] + rotate_left(vars[0] + e + md5_constants[i] + buffer[j], shifts_table[i]);
    vars[0] = tmp;
  }

  for (i = 0; i < 4; ++i) {
    ctx->state[i] += vars[i];
  }
}

inline void md5_update(md5_ctx_t* ctx, const uint8_t* buffer, size_t size) {
  uint32_t i, offset;
  offset = ctx->data_length % 64;
  ctx->data_length += size;

  for(i = 0; i < size; ++i) {
    ctx->data[offset++] = *(buffer + i);
    if(offset % 64 == 0) {
      md5_transform(ctx, (uint32_t*)ctx->data);
      offset = 0;
    }
  }
}

inline void md5_to_string(md5_ctx_t* ctx, char* output) {
  char digest[16];
  uint32_t i;
  ((uint32_t*)ctx->data)[14] = (uint32_t) (ctx->data_length * 8);
  ((uint32_t*)ctx->data)[15] = (uint32_t)((ctx->data_length * 8) >> 32);
  md5_transform(ctx, (uint32_t*)ctx->data);
  memcpy(&digest[0], &ctx->state[0], sizeof(uint8_t) * 16);

  static const char alphabet[] = "0123456789abcdef";
  for(i = 0; i < 16; ++i) {
    uint8_t c = digest[i];
    output[i * 2    ] = alphabet[c >> 4];
    output[i * 2 + 1] = alphabet[c & 15];
  }
}

inline void md5_finalize(md5_ctx_t* ctx, char* output) {
  uint32_t offset = ctx->data_length % 64;
  uint32_t padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;
  static const uint8_t padding[] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  md5_update(ctx, padding, padding_length);
  ctx->data_length -= padding_length;
  md5_to_string(ctx, output);
}

#ifdef __cplusplus
} // extern "C"
#endif

inline std::string md5_file(md5_ctx_t* ctx, std::string_view filename) {
  FILE* file = fopen(filename.data(), "rb");
  if (!file) {
    return "";
  }
  char buffer[16384];
  std::string output;
  output.resize(32);

  size_t read;
  while ((read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
    md5_update(ctx, reinterpret_cast<const uint8_t*>(buffer), read);
  }
  fclose(file);

  md5_finalize(ctx, output.data());

  return output;
}

inline std::string md5_file(std::string_view filename) {
  md5_ctx_t ctx;
  md5_init(&ctx);
  return md5_file(&ctx, filename);
}

inline std::string md5_bytes(const uint8_t* buffer, size_t buffer_size) {
  std::string output;
  output.resize(32);

  md5_ctx_t ctx;
  md5_init(&ctx);
  md5_update(&ctx, buffer, buffer_size);
  md5_finalize(&ctx, output.data());

  return output;
}

inline std::string md5_string(std::string_view buffer) {
  return md5_bytes(reinterpret_cast<const uint8_t*>(buffer.data()), buffer.length());
}

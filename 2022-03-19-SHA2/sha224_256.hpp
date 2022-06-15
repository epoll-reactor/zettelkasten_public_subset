#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>


enum { SHA224, SHA256 };

typedef struct {
  uint8_t  data[64];
  uint32_t data_length;
  uint64_t bit_length;
  uint32_t state[8];
  uint8_t  type;
} sha224_256_ctx_t;

inline void sha256_init(sha224_256_ctx_t* ctx) {
  ctx->type = SHA256;
  ctx->data_length = 0;
  ctx->bit_length = 0;
  uint32_t* data = &ctx->state[0];
  *(data++) = 0x6a09e667;
  *(data++) = 0xbb67ae85;
  *(data++) = 0x3c6ef372;
  *(data++) = 0xa54ff53a;
  *(data++) = 0x510e527f;
  *(data++) = 0x9b05688c;
  *(data++) = 0x1f83d9ab;
  *(data  ) = 0x5be0cd19;
}

inline void sha224_init(sha224_256_ctx_t* ctx) {
  ctx->type = SHA224;
  ctx->data_length = 0;
  ctx->bit_length = 0;
  uint32_t* data = &ctx->state[0];
  *(data++) = 0xc1059ed8;
  *(data++) = 0x367cd507;
  *(data++) = 0x3070dd17;
  *(data++) = 0xf70e5939;
  *(data++) = 0xffc00b31;
  *(data++) = 0x68581511;
  *(data++) = 0x64f98fa7;
  *(data  ) = 0xbefa4fa4;
}

__attribute__((always_inline, const))
static inline uint32_t rotate_right_32_bit(uint32_t value, uint32_t count) {
  __asm__("rorl %1, %0" : "+r" (value) : "c" (count));
  return value;
}

__attribute__((always_inline, const))
static inline uint32_t shift_right_32_bit(uint32_t value, uint32_t count) {
  __asm__("shr %1, %0" : "+r" (value) : "c" (count));
  return value;
}

inline void sha224_256_transform(sha224_256_ctx_t* ctx, const uint8_t* buffer) {
  static const uint32_t constants[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
  };

  uint32_t i, sum0, sum1, sigma0, sigma1, padded_chunk[64], vars[8];

  for (i = 0; i < 8; ++i) {
    vars[i] = ctx->state[i];
  }

  for (i = 0; i < 16; ++i) {
    padded_chunk[i]
      = ((uint32_t)buffer[0]) << 24
      | ((uint32_t)buffer[1]) << 16
      | ((uint32_t)buffer[2]) <<  8
      | ((uint32_t)buffer[3])     ;
    buffer += 4;
  }

  for (i = 16; i < 64; ++i) {
    sigma0 = padded_chunk[i - 15];
    sigma0 = (
      rotate_right_32_bit(sigma0,  7) ^
      rotate_right_32_bit(sigma0, 18) ^
       shift_right_32_bit(sigma0,  3)
    );
    sigma1 = padded_chunk[i - 2];
    sigma1 = (
      rotate_right_32_bit(sigma1, 17) ^
      rotate_right_32_bit(sigma1, 19) ^
       shift_right_32_bit(sigma1, 10)
    );
    padded_chunk[i] = padded_chunk[i - 16] + sigma0 + padded_chunk[i - 7] + sigma1;
  }

  for (i = 0; i < 64; ++i) {
    sum0 = vars[7] + (
      rotate_right_32_bit(vars[4],  6) ^
      rotate_right_32_bit(vars[4], 11) ^
      rotate_right_32_bit(vars[4], 25)
    ) + ((vars[4] & vars[5]) ^ (~vars[4] & vars[6])) + constants[i] + padded_chunk[i];
    sum1 = (
      rotate_right_32_bit(vars[0],  2) ^
      rotate_right_32_bit(vars[0], 13) ^
      rotate_right_32_bit(vars[0], 22)
    ) + ((vars[0] & vars[1]) ^ (vars[0] & vars[2]) ^ (vars[1] & vars[2]));
    vars[7] = vars[6];
    vars[6] = vars[5];
    vars[5] = vars[4];
    vars[4] = vars[3] + sum0;
    vars[3] = vars[2];
    vars[2] = vars[1];
    vars[1] = vars[0];
    vars[0] = sum0 + sum1;
  }

  for (i = 0; i < 8; ++i) {
    ctx->state[i] += vars[i];
  }
}

inline void sha224_256_update(sha224_256_ctx_t* ctx, const uint8_t* buffer, size_t size) {
  uint32_t i;
  for (i = 0; i < size; ++i) {
    ctx->data[ctx->data_length] = buffer[i];
    ctx->data_length++;
    if (ctx->data_length == 64) {
      sha224_256_transform(ctx, ctx->data);
      ctx->bit_length += 512;
      ctx->data_length = 0;
    }
  }
}

inline void sha224_256_to_string(sha224_256_ctx_t* ctx, char* output) {
  uint8_t hash[65];
  uint8_t* ptr = hash;
  uint32_t i;
  hash[64] = 0;
  size_t states_to_process = (ctx->type == SHA256) ? 8 : 7;
  for (i = 0; i < states_to_process; ++i) {
    *(ptr++) = (uint8_t)(ctx->state[i] >> 24);
    *(ptr++) = (uint8_t)(ctx->state[i] >> 16);
    *(ptr++) = (uint8_t)(ctx->state[i] >>  8);
    *(ptr++) = (uint8_t)(ctx->state[i]      );
  }
  static const char alphabet[] = "0123456789abcdef";
  uint32_t hash_bytes;
  if (ctx->type == SHA256) {
    output[64] = 0;
    hash_bytes = 32;
  } else {
    output[56] = 0;
    hash_bytes = 28;
  }
  for (i = 0; i < hash_bytes; ++i) {
    uint8_t c = hash[i];
    output[i * 2    ] = alphabet[c >> 4];
    output[i * 2 + 1] = alphabet[c & 15];
  }
}

inline void sha224_256_final(sha224_256_ctx_t* ctx, char* output) {
  uint32_t i = ctx->data_length;
  ctx->data[i++] = 0x80;
  if (ctx->data_length < 56) {
    memset(&ctx->data[i], 0x00, 56 - i);
  } else {
    memset(&ctx->data[i], 0x00, 64 - i);
    sha224_256_transform(ctx, ctx->data);
    memset(ctx->data, 0x00, 56);
  }

  ctx->bit_length += ctx->data_length * 8;

  uint8_t* data = &ctx->data[55];
  *(++data) = ctx->bit_length >> 56;
  *(++data) = ctx->bit_length >> 48;
  *(++data) = ctx->bit_length >> 40;
  *(++data) = ctx->bit_length >> 32;
  *(++data) = ctx->bit_length >> 24;
  *(++data) = ctx->bit_length >> 16;
  *(++data) = ctx->bit_length >>  8;
  *(++data) = ctx->bit_length      ;
  sha224_256_transform(ctx, ctx->data);
  sha224_256_to_string(ctx, output);
}

#ifdef __cplusplus
} // extern "C"
#endif

#include <string>

inline std::string sha224_256_file(sha224_256_ctx_t* ctx, std::string_view filename) {
  FILE* file = fopen(filename.data(), "rb");
  if (!file) {
    return "";
  }
  char buffer[16384];
  size_t read;
  while ((read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
    sha224_256_update(ctx, reinterpret_cast<const uint8_t*>(buffer), read);
  }
  fclose(file);
  std::string result;
  if (ctx->type == SHA256) {
    result.resize(64);
  } else {
    result.resize(56);
  }
  sha224_256_final(ctx, result.data());
  return result;
}

inline std::string sha256_file(std::string_view filename) {
  sha224_256_ctx_t ctx;
  sha256_init(&ctx);
  return sha224_256_file(&ctx, filename);
}

inline std::string sha224_file(std::string_view filename) {
  sha224_256_ctx_t ctx;
  sha224_init(&ctx);
  return sha224_256_file(&ctx, filename);
}

inline std::string sha224_256_bytes(sha224_256_ctx_t* ctx, const uint8_t* buffer, size_t buffer_size) {
  std::string result;
  if (ctx->type == SHA256) {
    result.resize(64);
  } else {
    result.resize(56);
  }
  sha224_256_update(ctx, buffer, buffer_size);
  sha224_256_final(ctx, result.data());
  return result;
}

inline std::string sha256_bytes(const uint8_t* buffer, size_t buffer_size) {
  sha224_256_ctx_t ctx;
  sha256_init(&ctx);
  return sha224_256_bytes(&ctx, buffer, buffer_size);
}

inline std::string sha224_bytes(const uint8_t* buffer, size_t buffer_size) {
  sha224_256_ctx_t ctx;
  sha224_init(&ctx);
  return sha224_256_bytes(&ctx, buffer, buffer_size);
}

inline std::string sha256_string(std::string_view buffer) {
  return sha256_bytes(reinterpret_cast<const uint8_t*>(buffer.data()), buffer.size());
}

inline std::string sha224_string(std::string_view buffer) {
  return sha224_bytes(reinterpret_cast<const uint8_t*>(buffer.data()), buffer.size());
}

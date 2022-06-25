// Feb 23, 2021
#include <stdio.h>
#include <string.h>
#include <stdint.h>


typedef struct {
  uint8_t  data[128];
  uint64_t state[8];
  uint32_t data_length;
  uint64_t bit_length;
} sha512_ctx_t;

void sha512_init(sha512_ctx_t* ctx) {
  ctx->data_length = 0;
  ctx->bit_length = 0;
  uint64_t* data = &ctx->state[0];
  *(data++) = 0x6a09e667f3bcc908;
  *(data++) = 0xbb67ae8584caa73b;
  *(data++) = 0x3c6ef372fe94f82b;
  *(data++) = 0xa54ff53a5f1d36f1;
  *(data++) = 0x510e527fade682d1;
  *(data++) = 0x9b05688c2b3e6c1f;
  *(data++) = 0x1f83d9abfb41bd6b;
  *(data  ) = 0x5be0cd19137e2179;
}

static inline uint64_t rotate_right(uint64_t value, uint64_t count) {
  __asm__("ror %1, %0" : "+r" (value) : "cI" (count));
  return value;
}

static inline uint64_t shift_right(uint64_t value, uint8_t count) {
  __asm__("shr %1, %0" : "+r" (value) : "cI" (count));
  return value;
}

void sha512_transform(sha512_ctx_t* ctx, const uint8_t* data) {
  static const uint64_t constants[] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
  };
  uint64_t i, s0, s1, t1, t2, padded_chunk[16], vars[8];

  for (i = 0; i < 8; ++i) {
    vars[i] = ctx->state[i];
  }

  for (i = 0; i < 16; i++) {
    padded_chunk[i]
      = ((uint64_t)data[0]) << 56
      | ((uint64_t)data[1]) << 48
      | ((uint64_t)data[2]) << 40
      | ((uint64_t)data[3]) << 32
      | ((uint64_t)data[4]) << 24
      | ((uint64_t)data[5]) << 16
      | ((uint64_t)data[6]) <<  8
      | ((uint64_t)data[7])     ;
    data += 8;

    t1 = vars[7] + (
      rotate_right(vars[4], 14) ^
      rotate_right(vars[4], 18) ^
      rotate_right(vars[4], 41)
    ) + ((vars[4] & vars[5]) ^ (~vars[4] & vars[6])) + constants[i] + padded_chunk[i];

    t2 = (
      rotate_right(vars[0], 28) ^
      rotate_right(vars[0], 34) ^
      rotate_right(vars[0], 39)
    ) + ((vars[0] & vars[1]) ^ (vars[0] & vars[2]) ^ (vars[1] & vars[2]));

    vars[7] = vars[6];
    vars[6] = vars[5];
    vars[5] = vars[4];
    vars[4] = vars[3] + t1;
    vars[3] = vars[2];
    vars[2] = vars[1];
    vars[1] = vars[0];
    vars[0] = t1 + t2;
  }

  for (i = 16; i < 80; i++) {
    s0 = padded_chunk[(i +  1) & 0x0F];
    s0 = rotate_right(s0, 1) ^ rotate_right(s0,  8) ^  shift_right(s0, 7);
    s1 = padded_chunk[(i + 14) & 0x0F];
    s1 = rotate_right(s1, 19) ^ rotate_right(s1, 61) ^  shift_right(s1, 6);

    t1 = padded_chunk[i & 0xf] += s0 + s1 + padded_chunk[(i + 9) & 0xf];
    t1 += vars[7] + (
      rotate_right(vars[4], 14) ^
      rotate_right(vars[4], 18) ^
      rotate_right(vars[4], 41)
    ) + ((vars[4] & vars[5]) ^ (~vars[4] & vars[6])) + constants[i];
    t2 = (
      rotate_right(vars[0], 28) ^
      rotate_right(vars[0], 34) ^
      rotate_right(vars[0], 39)
    ) + ((vars[0] & vars[1]) ^ (vars[0] & vars[2]) ^ (vars[1] & vars[2]));

    vars[7] = vars[6];
    vars[6] = vars[5];
    vars[5] = vars[4];
    vars[4] = vars[3] + t1;
    vars[3] = vars[2];
    vars[2] = vars[1];
    vars[1] = vars[0];
    vars[0] = t1 + t2;
  }

  for (i = 0; i < 8; ++i) {
    ctx->state[i] += vars[i];
  }
}

void sha512_update(sha512_ctx_t* ctx, const uint8_t* data, size_t size) {
  uint32_t i;
  for (i = 0; i < size; ++i) {
    ctx->data[ctx->data_length] = data[i];
    ctx->data_length++;
    if (ctx->data_length == 128) {
      sha512_transform(ctx, ctx->data);
      ctx->bit_length += 1024;
      ctx->data_length = 0;
    }
  }
}

void sha512_to_string(sha512_ctx_t* ctx, char* output) {
  char hash[65]{0};
  char* ptr = hash;
  for (size_t i = 0; i < 8; ++i) {
    *(ptr++) = (uint8_t)(ctx->state[i] >> 56);
    *(ptr++) = (uint8_t)(ctx->state[i] >> 48);
    *(ptr++) = (uint8_t)(ctx->state[i] >> 40);
    *(ptr++) = (uint8_t)(ctx->state[i] >> 32);
    *(ptr++) = (uint8_t)(ctx->state[i] >> 24);
    *(ptr++) = (uint8_t)(ctx->state[i] >> 16);
    *(ptr++) = (uint8_t)(ctx->state[i] >>  8);
    *(ptr++) = (uint8_t)(ctx->state[i]      );
  }

  static const char alphabet[] = "0123456789abcdef";
  output[128] = 0;
  for (size_t i = 0; i < 64; ++i) {
    uint8_t c = hash[i];
    output[i * 2    ] = alphabet[c >> 4];
    output[i * 2 + 1] = alphabet[c & 15];
  }
}

void sha512_final(sha512_ctx_t* ctx, char* output) __THROW {
  uint32_t i = ctx->data_length;
  if (ctx->data_length < 120) {
    ctx->data[i++] = 0x80;
    while (i < 120) {
      ctx->data[i++] = 0x00;
    }
  } else {
    ctx->data[i++] = 0x80;
    while (i < 128) {
      ctx->data[i++] = 0x00;
    }
    sha512_transform(ctx, ctx->data);
    memset(ctx->data, 0, 120);
  }

  ctx->bit_length += ctx->data_length * 8;

  uint8_t* data = &ctx->data[119];
  *(++data) = ctx->bit_length >> 56;
  *(++data) = ctx->bit_length >> 48;
  *(++data) = ctx->bit_length >> 40;
  *(++data) = ctx->bit_length >> 32;
  *(++data) = ctx->bit_length >> 24;
  *(++data) = ctx->bit_length >> 16;
  *(++data) = ctx->bit_length >>  8;
  *(++data) = ctx->bit_length      ;
  sha512_transform(ctx, ctx->data);
  sha512_to_string(ctx, output);
}

#include <string>

std::string sha512_buffer(const uint8_t* buffer, size_t size) {
  sha512_ctx_t ctx;
  sha512_init(&ctx);
  std::string result;
  result.resize(128);
  sha512_update(&ctx, buffer, size);
  sha512_final(&ctx, result.data());
  return result;
}

std::string sha512_string(std::string_view data) {
  return sha512_buffer(reinterpret_cast<const uint8_t*>(data.data()), data.size());
}

std::string sha512_file(std::string_view filename) {
  sha512_ctx_t ctx;
  sha512_init(&ctx);
  FILE* file = fopen(filename.data(), "rb");
  if (!file) {
    return "";
  }
  char buffer[4096];
  size_t read;
  while ((read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
    sha512_update(&ctx, reinterpret_cast<const uint8_t*>(buffer), read);
  }
  fclose(file);
  std::string result;
  result.resize(128);
  sha512_final(&ctx, result.data());
  return result;
}

#include <iostream>

void run_sha512_tests() {
  bool success = true;
  success &= (sha512_string("") == "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e");
  success &= (sha512_string("говно") == "e17cefa98dfab4645707eb59bd9c600a13587402c7630407976bcc3daa448e166ea1650c7d46a12f63076f375d9d5f397aed91a0ee80294d6303d33679ceab72");
  success &= (sha512_file("/boot/vmlinuz-linux") == "a85acdf43648ad113864884781606a2263fbaa6cd48c88dfe77bdf24ba9a10bc2ba390da4fe78a82aedb57713b03baefc67aaa4dc363cdbea6cfa366bd9f397c");
  std::cout << "Tests " << (success ? "passed" : "failed") << std::endl;
}

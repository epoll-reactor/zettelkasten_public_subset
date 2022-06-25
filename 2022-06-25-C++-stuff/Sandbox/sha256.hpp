// Feb 23, 2021
#ifndef SHA256_HPP
#define SHA256_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>

//#define SHA256_DEBUG
#ifdef SHA256_DEBUG
# include <stdio.h>
#endif // SHA256_DEBUG


enum { SHA224, SHA256 };

typedef struct {
  uint8_t  data[64];
  uint32_t data_length;
  uint64_t bit_length;
  uint32_t state[8];
  uint8_t  type;
} sha224_256_ctx_t;

static inline uint32_t rotate_right(uint32_t value, uint32_t count) {
  __asm__("ror %1, %0" : "+r" (value) : "cI" (count));
  return value;
}

static inline uint32_t shift_right(uint32_t value, uint32_t count) {
  __asm__("shr %1, %0" : "+r" (value) : "cI" ((uint8_t)count));
  return value;
}

static inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
  return (x & y) ^ (~x & z);
}

static inline uint32_t majority(uint32_t x, uint32_t y, uint32_t z) {
  return (x & y) ^ (x & z) ^ (y & z);
}

static inline void sha224_256_transform(sha224_256_ctx_t* ctx, const uint8_t* buffer) __THROW {
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

  uint32_t i, j, sigma0, sigma1, padded_chunk[64], vars[8];

  for (i = 0; i < 8; ++i) {
    vars[i] = ctx->state[i];
  }

  /* i=(0, 15) */
  for (i = 0, j = 0; i < 16; ++i, j += 4) {
    padded_chunk[i]
      = ((uint32_t)(buffer[j    ]) << 24)
      | ((uint32_t)(buffer[j + 1]) << 16)
      | ((uint32_t)(buffer[j + 2]) <<  8)
      | ((uint32_t)(buffer[j + 3]) <<  0);
  }

  /* i=(16, 63) */
  for (; i < 64; ++i) {
    sigma0 = rotate_right(padded_chunk[i - 15],  7) ^
             rotate_right(padded_chunk[i - 15], 18) ^
              shift_right(padded_chunk[i - 15],  3);
    sigma1 = rotate_right(padded_chunk[i -  2], 17) ^
             rotate_right(padded_chunk[i -  2], 19) ^
              shift_right(padded_chunk[i -  2], 10);
    padded_chunk[i] = padded_chunk[i - 16] + sigma0 + padded_chunk[i - 7] + sigma1;
  }

  for (i = 0; i < 64; ++i) {
    sigma0 = vars[7] + (
      rotate_right(vars[4],  6) ^
      rotate_right(vars[4], 11) ^
      rotate_right(vars[4], 25)
    ) + ch(vars[4], vars[5], vars[6]) + constants[i] + padded_chunk[i];
    sigma1 = (
      rotate_right(vars[0],  2) ^
      rotate_right(vars[0], 13) ^
      rotate_right(vars[0], 22)
    ) + majority(vars[0], vars[1], vars[2]);
    vars[7] = vars[6];
    vars[6] = vars[5];
    vars[5] = vars[4];
    vars[4] = vars[3] + sigma0;
    vars[3] = vars[2];
    vars[2] = vars[1];
    vars[1] = vars[0];
    vars[0] = sigma0 + sigma1;
  }
  for (i = 0; i < 8; ++i) {
    ctx->state[i] += vars[i];
  }
}

void sha256_init(sha224_256_ctx_t* ctx) __THROW {
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

void sha224_init(sha224_256_ctx_t* ctx) __THROW {
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

void sha224_256_update(sha224_256_ctx_t* ctx, const uint8_t* data, size_t length) __THROW {
  uint32_t i;
  for (i = 0; i < length; ++i) {
    ctx->data[ctx->data_length] = data[i];
    ctx->data_length++;
    if (ctx->data_length == 64) {
      sha224_256_transform(ctx, ctx->data);
      ctx->bit_length += 512;
      ctx->data_length = 0;
    }
  }
}

void sha224_256_to_string(sha224_256_ctx_t* ctx, char* out) __THROW {
  uint8_t hash[65];
  hash[64] = 0;
  uint32_t i;
  for (i = 0; i < 4; ++i) {
    hash[i     ] = (uint8_t)(ctx->state[0] >> (24 - i * 8));
    hash[i +  4] = (uint8_t)(ctx->state[1] >> (24 - i * 8));
    hash[i +  8] = (uint8_t)(ctx->state[2] >> (24 - i * 8));
    hash[i + 12] = (uint8_t)(ctx->state[3] >> (24 - i * 8));
    hash[i + 16] = (uint8_t)(ctx->state[4] >> (24 - i * 8));
    hash[i + 20] = (uint8_t)(ctx->state[5] >> (24 - i * 8));
    hash[i + 24] = (uint8_t)(ctx->state[6] >> (24 - i * 8));
    if (ctx->type == SHA256) {
      hash[i + 28] = (uint8_t)(ctx->state[7] >> (24 - i * 8));
    }
  }
  static const char alphabet[] = "0123456789abcdef";
  uint32_t hash_bytes;
  if (ctx->type == SHA224) {
    out[56] = 0;
    hash_bytes = 28;
  } else {
    out[64] = 0;
    hash_bytes = 32;
  }
  for (i = 0; i < hash_bytes; ++i) {
    uint8_t c = hash[i];
    out[i * 2    ] = alphabet[c >> 4];
    out[i * 2 + 1] = alphabet[c & 15];
  }
}

void sha224_256_final(sha224_256_ctx_t* ctx, char* out) __THROW {
  uint32_t i = ctx->data_length;
  if (ctx->data_length < 56) {
    ctx->data[i++] = 0x80;
    while (i < 56) {
      ctx->data[i++] = 0x00;
    }
  } else {
    ctx->data[i++] = 0x80;
    while (i < 64) {
      ctx->data[i++] = 0x00;
    }
    sha224_256_transform(ctx, ctx->data);
    memset(ctx->data, 0, 56);
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
  sha224_256_to_string(ctx, out);
}

#ifdef __cplusplus
} // extern "C"
#endif

#include <string>

std::string sha224_256_string(sha224_256_ctx_t* ctx, std::string_view data) {
  std::string result;
  if (ctx->type == SHA256) {
    result.resize(64);
  } else {
    result.resize(56);
  }
  sha224_256_update(ctx, reinterpret_cast<const uint8_t*>(data.data()), data.size());
  sha224_256_final(ctx, result.data());
  return result;
}

std::string sha224_256_file(sha224_256_ctx_t* ctx, std::string_view filename) {
  FILE* file = fopen(filename.data(), "rb");
  if (!file) {
    return "";
  }
  char buffer[4096];
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

std::string sha224_256_buffer(sha224_256_ctx_t* ctx, const uint8_t* buffer, size_t buffer_size) {
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

std::string sha256_string(std::string_view data) {
  sha224_256_ctx_t ctx;
  sha256_init(&ctx);
  return sha224_256_string(&ctx, data);
}

std::string sha224_string(std::string_view data) {
  sha224_256_ctx_t ctx;
  sha224_init(&ctx);
  return sha224_256_string(&ctx, data);
}

std::string sha256_file(std::string_view filename) {
  sha224_256_ctx_t ctx;
  sha256_init(&ctx);
  return sha224_256_file(&ctx, filename);
}

std::string sha224_file(std::string_view filename) {
  sha224_256_ctx_t ctx;
  sha224_init(&ctx);
  return sha224_256_file(&ctx, filename);
}

std::string sha256_bytes(const uint8_t* buffer, size_t buffer_size) {
  sha224_256_ctx_t ctx;
  sha256_init(&ctx);
  return sha224_256_buffer(&ctx, buffer, buffer_size);
}

std::string sha224_bytes(const uint8_t* buffer, size_t buffer_size) {
  sha224_256_ctx_t ctx;
  sha224_init(&ctx);
  return sha224_256_buffer(&ctx, buffer, buffer_size);
}


void run_sha256_test(const char* string, const char* expected_hash, sha224_256_ctx_t* ctx) {
  char out[65];
  sha256_init(ctx);
  sha224_256_update(ctx, (const uint8_t*) (string), strlen((const char*) (string)));
  sha224_256_final(ctx, out);
  uint8_t failed = strcmp(out, expected_hash) != 0;
  if (failed) {
    printf("SHA-256 failed:\n\t%s got,\n\t%s\nexpected\n", out, expected_hash);
    exit(-1);
  }
}

void run_sha224_test(const char* string, const char* expected_hash, sha224_256_ctx_t* ctx) {
  char out[65];
  sha224_init(ctx);
  sha224_256_update(ctx, (const uint8_t*) (string), strlen((const char*) (string)));
  sha224_256_final(ctx, out);
  uint8_t failed = strcmp(out, expected_hash) != 0;
  if (failed) {
    printf("SHA-224 failed:\n\t%s got,\n\t%s\nexpected\n", out, expected_hash);
    exit(-1);
  }
}

void run_sha224_256_file_test(const char* filename, const char* expected_hash, sha224_256_ctx_t* ctx) {
  FILE* file = fopen(filename, "rb");
  if (!file) {
    perror("fopen");
    return;
  }
  uint8_t buffer[8192];
  size_t read;
  while ((read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
    sha224_256_update(ctx, buffer, read);
  }
  fclose(file);
  char out[65];
  sha224_256_final(ctx, out);
  uint8_t failed = strcmp(out, expected_hash) != 0;
  if (failed) {
    printf("SHA-224/256 failed:\n\t%s got,\n\t%s\nexpected\n", out, expected_hash);
    exit(-1);
  }
}

void run_sha256_file_test(const char* filename, const char* expected_hash, sha224_256_ctx_t* ctx) {
  sha256_init(ctx);
  run_sha224_256_file_test(filename, expected_hash, ctx);
}

void run_sha224_file_test(const char* filename, const char* expected_hash, sha224_256_ctx_t* ctx) {
  sha224_init(ctx);
  run_sha224_256_file_test(filename, expected_hash, ctx);
}

#include "function_benchmarking.hpp"

void run_sha256_tests() {
  sha224_256_ctx_t ctx;
  run_sha256_test("", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855", &ctx);
  run_sha256_test("abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad", &ctx);
  run_sha256_test("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1", &ctx);
  run_sha256_test(":)", "54d626e08c1c802b305dad30b7e54a82f102390cc92c7d4db112048935236e9c", &ctx);
  run_sha256_test("\xC9\x8C\x8E\x55", "7abc22c0ae5af26ce93dbb94433a0e0b2e119d014f8e7f65bd56c61ccccd9504", &ctx);

  std::string big(2'000'000, '\xFF');
  run_sha256_test(big.data(), "80ba255c480cbaf980212d7b2fe103518f93ebbbaaa7360e33be6f47fd134395", &ctx);

  fun_benchmark("sha256 /boot/vmlinuz", 1, [&] { run_sha256_file_test("/boot/vmlinuz-linux", "f03c7a5aeeba3de5eb1357d103d4d340e722cfc9867b94ddb7772fe3bd592f53", &ctx); });
//  fun_benchmark("sha256 2.9 GiB", 1, [&] { run_sha256_file_test("/home/machen/downloads/ubuntu-unity-21.04.iso", "90eb79d6b46fd346820b0d3c9b1a035213c4715c8af116a092e232bce440a1d9", &ctx); });

  run_sha224_test("123", "78d8045d684abd2eece923758f3cd781489df3a48e1278982466017f", &ctx);
  run_sha224_test("", "d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f", &ctx);
  run_sha224_test("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525", &ctx);

  fun_benchmark("sha224 /boot/vmlinuz", 1, [&] { run_sha224_file_test("/boot/vmlinuz-linux", "ceedac7c0100ebfab10499a4a9cb0bc67bd49987350c18ce52e8b7af", &ctx); });
//  fun_benchmark("sha224 2.9 GiB", 1, [&] { run_sha224_file_test("/home/machen/downloads/ubuntu-unity-21.04.iso", "86562b51c457b41554fbefd9bed851f3a8633fe3d8595120cc04d431", &ctx); });

  std::cout << "SHA-224 :) - " << sha224_string(":)") << std::endl;
  std::cout << "SHA-256 :) - " << sha256_string(":)") << std::endl;

  std::cout << "SHA-224 /boot/vmlinuz - " << sha224_file("/boot/vmlinuz-linux") << std::endl;
  std::cout << "SHA-256 /boot/vmlinuz - " << sha256_file("/boot/vmlinuz-linux") << std::endl;
}

#endif // SHA256_HPP

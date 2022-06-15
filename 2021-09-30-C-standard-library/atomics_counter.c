#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

typedef struct {
  atomic_int *atomic;
  int        *regular;
} args_t;

int run(void *arg) {
  args_t *ctx = (args_t *)(arg);
  for (int i = 0; i < 1000; ++i) {
    (*ctx->atomic)++;
    (*ctx->regular)++;
  }
  return 0;
}

int main() {
  atomic_int atomic;
  int regular = 0;
  atomic_store_explicit(&atomic, 0, memory_order_consume);
  args_t args = {
  	.atomic = &atomic,
  	.regular = &regular
  };
  for (int i = 0; i < 1024 * 2; i += 8) {
    thrd_t threads[8];
    for (int j = 0; j < 8; ++j) {
      thrd_create(&threads[j], run, &args);
    }
  	for (int j = 0; j < 8; ++j) {
  	  thrd_join(threads[j], NULL);
  	}
  }
  printf("    atomic counter: %u\n", atomic);
  printf("non-atomic counter: %u\n", regular);
}

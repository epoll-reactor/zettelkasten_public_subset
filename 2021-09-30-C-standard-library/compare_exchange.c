#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

typedef struct {
  atomic_int i, j;
} args_t;

void *run_thread(void *arg) {
  args_t *args = (args_t *)(arg);
  
  while(!atomic_compare_exchange_weak_explicit(&args->i, &args->j, args->j,
    memory_order_seq_cst,
    memory_order_relaxed))
    ;
  
  while (args->i != args->j) {
    asm volatile("lock\n\tcmpxchg %1,%2"
 	  :"=a" (args->j)
   	  :"r"(args->j), "m"(args->i), "0"(args->j)
   	  :"memory");
  }
}

int main() {
  int iterations;
  atomic_int i, j;
  iterations = 0;
  atomic_store_explicit(&i, 10, memory_order_relaxed);
  atomic_store_explicit(&j, 20, memory_order_relaxed);
  printf("i = %d, j = %d\n", i, j);
  while (iterations++ != 1000) {
    pthread_t threads[8];
    for (int i = 0; i < 8; ++i) {
      args_t arg = {
      	.i = i,
      	.j = j
      };
      pthread_create(&threads[i], NULL, run_thread, &arg);
    }
    for (int i = 0; i < 8; ++i) {
    	pthread_join(threads[i], NULL);
    }
    /*
    while (i != j) {
   	  asm volatile("lock\n\tcmpxchg %1,%2"
   	    :"=a" (j)
   	    :"r"(j), "m"(i), "0"(j)
   	    :"memory");
    }
    */
  }
  printf("i = %d, j = %d\n", i, j);
}

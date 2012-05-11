#include <stdlib.h>
#include "semaphore.h"

typedef struct {
int nextcount;
int mutex;
int next ;
} lock;

typedef struct {
   int condsem;
   int semcount;
   lock *lock;
} cond;

Lock* lock_init();
void lock_acquire(Lock *l);
void lock_release(Lock *l);
Cond* cond_init();
void cond_wait(Cond *cnd);
void cond_signal(Cond *cnd);

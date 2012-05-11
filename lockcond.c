#include "lockcond.h"

typedef struct lock {
  int nextcount;
  int mutexSem;
  int nextSem;
} Lock;

typedef struct cond {
   int condSem;
   int semCount;
   Lock* lockVar;
} Cond;

Lock* lock_init()
{
  Lock* l      = (Lock*) malloc(sizeof(Lock));
  l->mutexSem  = seminit(0, 1);
  l->nextSem   = seminit(0, 0);
  l->nextcount = 0;

  return l;
}

void lock_acquire(Lock *l) {
  semdown(l->mutexSem);
  return;
}

void lock_release(Lock *l) {
  if(l->nextCount > 0) {
    semup(l->next);
  }
  else {
    semup(l->mutex);
  }

  return;
}

Cond* cond_init() {
  Cond* con     = (Cond*) malloc(sizeof(Cond));
  con->condSem  = seminit(0, 0);
  con->semCount = 0;
  con->lockVar  = lock_init();

  return con;
}

void cond_wait(Cond *cnd) {
  cnd->semCount += 1;
  lock_release(cnd->lockVar);
  semdown(cnd->condSem);
  cnd->semCount -= 1;

  return;
}

void cond_signal(Cond *cnd) {
   if(cnd->semCount > 0) {
      cnd->lockVar->nextcount += 1;
      semup(cnd->condSem);
      lock_acquire(cnd->lockVar);
      cnd->lockVar->nextcount -= 1;
   }
   return;
}

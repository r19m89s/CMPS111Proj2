#ifndef _PM_SEMAPHORE_H_
#define _PM_SEMAPHORE_H_1

/* Global variables */
typedef struct pLIST{
    struct mproc *pCUR;
    struct pLIST *next;
}pLIST;
typedef struct semaphore{
    int value;
    pLIST *ProcessList;
    int plLEN;
}semaphore;
semaphore *semLIST[100];

#endif /* !_PM_SEMAPHORE_H_ */



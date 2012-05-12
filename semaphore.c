/*
    This file handles system calls for semaphore operations
*/

#include "pm.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <minix/com.h>
#include <errno.h>
#include "param.h"
#include "mproc.h"

#define semUpperLimit  1000000
#define semLowerLimit -1000000


/* Global variables */

typedef struct pLIST{
    struct mproc*  pCUR;
    struct pLIST *next;
}pLIST;

typedef struct semaphore{
    int    value;
    pLIST *ProcessList;
    int   plLEN;
    int init;
} semaphore;

semaphore *semLIST[100];

PUBLIC int do_seminit( ){
    int sem = m_in.m1_i1;
    int value = m_in.m2_i2;
    int ind = 0;
    semaphore *semi;
    int blah = 0;
    int retVAL = 0;
    int size = sizeof(semLIST)/sizeof(semaphore);
    semi = semLIST[blah];
    int isFULL = 1;
    while ((&semi != NULL)&&(semLIST[sem]->init==1)){
        blah++;
        ind = blah;
        semi = semLIST[blah];
    }
    if ((isFULL == 1)&&(ind == 1000)){ printf("EAGAIN\n"); return EAGAIN;}
    if (sem < 0 || sem >= 100){
        printf("EINVAL\n");
        return EINVAL;
    }
    else if (sem != 0){
        if ((&semLIST[sem] != NULL)&&(semLIST[sem]->init == 1)){
            printf("EEXIST\n");
            return EEXIST;
        }
        semLIST[sem] = (semaphore*)malloc(sizeof(semaphore));
        semLIST[sem]->value = value;
        semLIST[sem]->plLEN = 0;
        semLIST[sem]->init = 1;
        retVAL = sem;
    }else {
        semLIST[ind] = (semaphore*)malloc(sizeof(semaphore));
        semLIST[ind]->value = value;
        semLIST[ind]->init = 1;
        semLIST[ind]->plLEN = 0;
        printf ("ind: %d\n", ind);
        retVAL = ind;
    }
    return retVAL;
}


PUBLIC int do_semvalue(){
    int sem = m_in.m1_i1;
    if(&semLIST[sem] == NULL) return 0x8000000;
    int val = semLIST[sem]->value;
    int min =(-10 ^ 6);
    int max = (10 ^ 6);
    if ((val <= min)||(val >= max)){
        return 0x8000000;
    }else {
        if (val < 0){
            return -val;
        }else {
            return 10001;
        }
    }
}


PUBLIC int do_semfree(){
    int sem;
    sem = m_in.m1_i1;
    if (sem < -1000 || sem > 1000){
        return 0;
     }else if (&semLIST[sem] == NULL){
         return 0;
     }else {
        if (semLIST[sem]->value > 0) return EBUSY;
        else {
            semLIST[sem]->init = 0;
            return 1;
        }
    }
}


/*
This call does UP on the semaphore whose identifier is passed. This call never blocks.
 If there’s at least one process waiting on this semaphore, semup() causes one waiting
  process to be awakened. The call returns 1 if successful, 0 otherwise.
Note: while a semaphore can’t be initialized outside the range −1000≤value≤1000, it may 
be incremented (or decremented) to a value outside this range, up to ±106. If the semup()
 call would result in a value above 106, return 0 and set the error to EOVERFLOW.
*/

PUBLIC int do_semup() {
    int sem;
    sem = m_in.m1_i1;
    if (sem < 0 || sem >= 100) {
        printf("EINVAL\n");
        return EINVAL;
    }
    semaphore* thisSem = semLIST[sem];
    if(thisSem->value == semUpperLimit)
    { //check if the semaphore is already at the upper end of the allowed limit
        printf("EOVERFLOW\n");
        errno = EOVERFLOW;
        return 0;
    }
    else {
        thisSem->value += 1;
    }
    if(thisSem->value <= 0){
        /*
            get the process at the beginning of the waiting list
                and wake it up.
        */
        thisSem->plLEN -= 1;
        if(thisSem->ProcessList != NULL){
            pLIST *tempNode = thisSem->ProcessList;

            thisSem->ProcessList = thisSem->ProcessList->next;

            // give signal to tempNode->pCUR to wake up;
            tempNode->pCUR->mp_flags |= UNPAUSED;

            if(pm_isokendpt(who_e, &who_p) != OK)
                  panic("PM got message from invalid endpoint: %d", who_e);


            setreply(who_p, OK);

            free(tempNode->pCUR);
            free(tempNode);
        }
    }

    return 1;   //return 1 on success
}


PUBLIC int do_semdown() {
    int sem;
    sem = m_in.m1_i1;
    if (sem < 0 || sem >= 100) {
        printf("EINVAL\n");
        return EINVAL;
    }
    semaphore* thisSem = semLIST[sem];
    printf("thisSem->value: %d\n", thisSem->value);
    if(thisSem->value == semLowerLimit)
    { //check if the semaphore is already at the lower end of the allowed limit
        printf("EOVERFLOW\n");
        errno = EOVERFLOW;
        return 0;
    }
    else {
        thisSem->value -= 1;
    }
    if(thisSem->value < 0)
    {
        /* add the current process to the list of waiting processes
            and put the process to sleep
        */
        thisSem->plLEN += 1;
        
        if(thisSem->plLEN > 1000)
        {
            fprintf(stderr, "Error: More than 1000 processes waiting on a semaphore.\n");
            exit(-1);
        }

        pLIST* newPLnode = (pLIST*) malloc(sizeof(pLIST));
        newPLnode->pCUR  = (struct mproc*)malloc(sizeof(struct mproc));
        newPLnode->pCUR  = mp;
        newPLnode->next  = NULL;

        if(thisSem->ProcessList == NULL)
        {
            thisSem->ProcessList = newPLnode;
        }
        else
        {
            pLIST* existingLastNode = thisSem->ProcessList;

            while(existingLastNode->next != NULL)
            {
                existingLastNode = existingLastNode->next;
            }

            existingLastNode->next = newPLnode;
        }

        mp->mp_flags |= PAUSED;
        return SUSPEND;
    }

    return 1;   //return 1 on success
}

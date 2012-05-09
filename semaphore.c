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
} semaphore;


semaphore *semLIST[100];

PUBLIC int do_seminit( ){
    int sem = m_in.m1_i1;
    int value = m_in.m2_i2;
    printf("sem: %d, value: %d\n", sem, value);
    int ind;
    semaphore *semi;
    int blah = 0;
    int retVAL = 0;
    int size = sizeof(semLIST)/sizeof(semaphore);
    if (size == 100){
       semi = semLIST[blah];
       int isFULL = 1;
       while (semi != NULL){
         if (semi->value == 0){
            isFULL = 0;
            ind = blah;
        }
        blah++;
        semi = semLIST[blah];
       }
       if (isFULL == 1){ printf("EAGAIN\n"); return EAGAIN;}
    }else if (sem < 0 || sem >= 100){
        printf("EINVAL\n");
        return EINVAL;
    }else if ((semi != NULL)&&(semi->value != 0)){
        printf("EEXIST\n");
        return EEXIST;
    }
    else if (sem != 0){
        semLIST[sem]->value = value;
        semLIST[sem]->plLEN = 0;
        retVAL = sem;
    }else {
        semLIST[ind]->value = value;
        semLIST[ind]->plLEN = 0;
        retVAL = ind;
    }
    return retVAL;
}


PUBLIC int do_semvalue(){
    int sem = m_in.m1_i1;
    if(&semLIST[sem] == NULL) return 0x8000000;
    int val = semLIST[sem]->plLEN;
    int min =(-10 ^ 6);
    int max = (10 ^ 6);
    if ((val <= min)||(val >= max)){
        return 0x8000000;
    }else {
        if (val < 0){
            return -val;
        }else {
            return 0;
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
            free ((semaphore*)semLIST[sem]->ProcessList);
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
        //figure out how to set an error
        return 0;
    }
    else {
        thisSem->value += 1;
    }

    if(thisSem->value <= 0)
    {
        /*
            get the process at the beginning of the waiting list
                and wake it up.
        */
        thisSem->plLEN -= 1;

        if(thisSem->ProcessList != NULL)
        {
            pLIST *tempNode = thisSem->ProcessList;

            thisSem->ProcessList = thisSem->ProcessList->next;

            // give signal to tempNode->pCUR to wake up;
            tempNode->pCUR->mp_flags |= UNPAUSED;

//            setreply(tempNode->pCUR->p)

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
        /* add the process id of the current process to the list of waiting processes
            and put the process to sleep
        */
        thisSem->plLEN += 1;

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

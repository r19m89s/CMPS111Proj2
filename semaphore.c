#include "pm.h"
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <minix/com.h>
#include "param.h"
#include "mproc.h"
#include "semaphore.h"
PUBLIC int do_seminit( ){
    int sem = m_in.m1_i1;
    int value = m_in.m2_i2;
    printf("sem: %d, value: %d\n", sem, value);
    int ind;
    semaphore *semi;
    int blah = 0;
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
        semLIST[sem] = (semaphore*)malloc(sizeof(semaphore));
        semLIST[sem]->value = value;
        semLIST[sem]->ProcessList = (pLIST*)malloc(sizeof(pLIST));
        semLIST[sem]->ProcessList->pCUR = (struct mproc*)malloc(sizeof(struct mproc));
        semLIST[sem]->ProcessList->next = (pLIST*)malloc(sizeof(pLIST));
        semLIST[sem]->plLEN = 0;
        printf ("return sem\n");
        return sem;
    }else {
        semLIST[ind] = (semaphore*)malloc(sizeof(semaphore));
        semLIST[ind]->value = value;
        semLIST[ind]->ProcessList = (pLIST*)malloc(sizeof(pLIST));
        semLIST[ind]->ProcessList->pCUR = (struct mproc*)malloc(sizeof(struct mproc));
        semLIST[ind]->ProcessList->next = (pLIST*)malloc(sizeof(pLIST));
        semLIST[ind]->plLEN = 0;
        printf("return ind\n");
        return ind;
    }
}
PUBLIC int do_semvalue(){
    int sem = m_in.m1_i1;
    printf ("do_semvalue sem: %d\n", sem);
    int min =(-10 ^ 6);
    int max = (10 ^ 6);
    if ((min <= sem)||(sem >= max)||(&semLIST[sem] == NULL)){
        return 0x8000000;
    }else {
        return -semLIST[sem]->value;
    }
}
PUBLIC int do_semup(){
    printf("I am do_semup\n");
    return(OK);
}
PUBLIC int do_semdown(){
    printf("I am do_semdown\n");
    return(OK);
}
PUBLIC int do_semfree(){
    int sem;
    printf("I am do_semfree\n");
    return(OK);
}




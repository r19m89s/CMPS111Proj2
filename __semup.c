#include <lib.h>
#include <unistd.h>

PUBLIC int semup(sem)
int sem;
{
        message m;
        m.m1_i1 = sem;
        return(_syscall(PM_PROC_NR,SEMUP,&m));
}

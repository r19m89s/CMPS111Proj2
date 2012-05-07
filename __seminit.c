#include <lib.h>
#include <unistd.h>

PUBLIC int seminit(sem,value)
int sem;
int value;
{
        message m;
        m.m1_i1 = sem;
        m.m2_i2 = value;
        return(_syscall(PM_PROC_NR,SEMINIT,&m));
}

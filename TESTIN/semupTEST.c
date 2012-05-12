#include <stdio.h>
int main(){
    int retVAL = semvalue(5);
    printf ("retVAL: %d\n", retVAL);
    int retUP = semup(5);
    printf("retUP: %d\n", retUP);
    int retVAL2 = semvalue(5);
    printf("retVAL2: %d\n", retVAL2);
}

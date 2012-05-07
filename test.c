#include <stdio.h>
int main(){
    int retINIT = seminit(3,5);
    int ret = semvalue(3);
    printf ("retINIT: %d, ret: %d\n", retINIT, ret);
}

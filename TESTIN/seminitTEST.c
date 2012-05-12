#include <stdio.h>
int main(){
    int retINIT;
    int v = 0;
    for (; v < 100; v++){
        retINIT = seminit(v,0);
    }
    retINIT = seminit(5,0);
}

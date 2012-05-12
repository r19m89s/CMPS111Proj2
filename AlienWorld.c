#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define gen1 1
#define gen2 2
#define gen3 3

int repTIME = 0;
int nursVAL = 2;

void gender1(){
    int val2 = semvalue(2);
    int val3 = semvalue(3);
    int down;
    int up;
    if((val2 == 1)&&(val3 == 1)){
        down = semdown(1);
    }else {
        int nursVAL = semvalue(4);
        while (nursVAL <= 0);
        up = semup(2);
        up = semup(3);
        while (repTIME == 0);
        usleep(repTIME);
        int time = rand()%400000+1000;
        usleep(time);
    }
}
void gender2(){
    int val1 = semvalue(1);
    int val3 = semvalue(3);
    int down, up;
    if ((val1 == 1001)&&(val3 == 1001)){
         down = semdown(2);
    }else {
        int nursVAL = semvalue(4);
        while (nursVAL == 1001);
        up = semup(2);
        up = semup(3);
        while (repTIME == 0);
        usleep(repTIME);
        int time = rand()%4000000+1000;
        usleep(time);
    }
}
void gender3(){
    int val1 = semvalue(1);
    int val2 = semvalue(2);
    int up, down;
    if ((val1 == 1001)||semvalue(1)==1001){
        down = semdown(3);
    }else {
        up = semup(1);
        up = semup(2);
        up = semup(4);
        repTIME = rand()%200000+2000;
        usleep(repTIME);
        int time = rand()%40000+1000;
        usleep(time);
    }
}
int genNUMB(int gen){
    if (gen == gen1) gender1();
    else if (gen  == gen2) gender2();
    else if (gen == gen3) gender3();
}
int main(){
    srand(time(NULL));
    int gen1sem = seminit(1,1);
    int gen2sem = seminit(2,1);
    int gen3sem = seminit(3,1);
    int nursery = seminit(4,2);
    int randARR[7] = {1,2,2,2,3,2,1};
    int v = 0;
    for (; v < 7; v++){
        genNUMB(randARR[v]);
    }
}


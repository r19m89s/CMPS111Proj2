#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int nreaders = 0;
int mutexSem;
int writingSem;
int fd;           //file descriptor for a file to read/write

void readerFunc();
void writerFunc();

int main() {
  fd = open("rwTestFile.txt", O_RDWR, 0644);

  mutexSem = seminit(0, 1);
  writingSem = seminit(0, 1);

  readerFunc();
  writerFunc();
  return 0;
}

void readerFunc() {
    char readBuffer[100];
    semdown(mutexSem);
    /*
          in semdown, this reader will go to sleep if there already 
          are readers in the mutex queue
    */
    nreaders++; //announcing the existance of this new Reader

    if (nreaders == 1) {          // Check if this is the frist Reader
        if(semvalue(writingSem) <= 0) {  //Check if a Writer exists
            //manually put the reader to sleep for random 20000–200000 microseconds.
          long randNum = (random() + 20000) % 200000;
          usleep(randNum);
        }
        semdown(writingSem);
    }
    semup(mutexSem);
    read(fd, readBuffer, 1);

    semdown(mutexSem);
    nreaders--;       //this current reader is leaving

    if (nreaders == 0) {  //check if this reader is the last one
      semup(writingSem);       //this will let the W resume
    }
    semup(mutexSem);
}

void writerFunc() {
  semdown(writingSem);
  //do the writing
  write(fd, "W", 1);

  semup(writingSem);
}

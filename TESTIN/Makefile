# CMPS 111
# Makefile for testing directory TESTIN
all: seminitTEST seminitTEST2 seminitTEST3 semupTEST

seminitTEST: seminitTEST.o
	gcc -o seminitTEST seminitTEST.o

seminitTEST.o: seminitTEST.c
	gcc -c seminitTEST.c

seminitTEST2: seminitTEST2.o
	gcc -o seminitTEST2 seminitTEST2.o

seminitTEST2.o: seminitTEST2.c
	gcc -c seminitTEST2.c

seminitTEST3: seminitTEST3.o
	gcc -o seminitTEST3 seminitTEST3.o

seminitTEST3.o: seminitTEST3.c
	gcc -c seminitTEST3.c

semupTEST: semupTEST.o
	gcc -o semupTEST semupTEST.o

semupTEST.o: semupTEST.c
	gcc -c semupTEST.c

clean:
	rm -f seminitTEST.o seminitTEST2.o seminitTEST3.o semupTEST.o

cleanAll:
	rm -f seminitTEST.o seminitTEST seminitTEST2.o seminitTEST2 seminitTEST3.o seminitTEST3 semupTEST.o semupTEST

HEADERS = chain.h statemachine.h

CFLAGS = -g 

default: all

chain.o: chain.c $(HEADERS)
	gcc $(CFLAGS) -c chain.c -o chain.o 

statemachine.o: statemachine.c $(HEADERS)
	gcc $(CFLAGS) -c statemachine.c -o statemachine.o 

ingest.o: ingest.c $(HEADERS)
	gcc $(CFLAGS) -c ingest.c -o ingest.o

main.o: main.c $(HEADERS)
	gcc $(CFLAGS) -c main.c -o main.o 

rcmpmain: main.c
	gcc $(CFLAGS) main.o -o run_chain

all: chain.o main.o statemachine.o ingest.o
	gcc $(CFLAGS) main.o chain.o statemachine.o ingest.o -o run_chain


clean:
	-rm -f *.o
	-rm run_chain
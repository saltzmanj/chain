IDIR =../include
CC=gcc
CFLAGS=-g -I$(IDIR)

ODIR=obj
LDIR =../src

LIBS=-lm

default: run_chain

_DEPS = chain.h ingest.h statemachine.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = chain.o ingest.o statemachine.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run_chain: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
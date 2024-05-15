IDIR =../include
CC=gcc
CFLAGS=-g -I$(IDIR)

ODIR=obj
LDIR =../src

LIBS=-lm

_DEPS = chain.h ingest.h statemachine.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ_TRAIN = chain.o ingest.o statemachine.o train.o
OBJ_TRAIN = $(patsubst %,$(ODIR)/%,$(_OBJ_TRAIN))

_OBJ_RUN = chain.o statemachine.o run.o
OBJ_RUN = $(patsubst %,$(ODIR)/%,$(_OBJ_RUN))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

train: $(OBJ_TRAIN)
	$(CC) -o ../trainmodel $^ $(CFLAGS) $(LIBS)

run: $(OBJ_RUN)
	$(CC) -o ../runmodel $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
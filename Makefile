CC=gcc

BDIR = ./bin/
IDIR = ./head/
SDIR = ./src/

CFLAGS=-I $(IDIR) -lm
OBJ = $(BDIR)rsa.o $(BDIR)prime.o $(BDIR)keys.o $(BDIR)protec.o $(BDIR)signs.o
DEB=-ggdb -Wall

TARGETS = main tests test

all: main

$(TARGETS): $(OBJ)
	$(CC) -o $(BDIR)$@ $(SDIR)$@.c $^ $(DEB) $(CFLAGS)

$(BDIR)%.o: $(SDIR)%.c $(IDIR)%.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f bin/*
	rm -f vgcore*

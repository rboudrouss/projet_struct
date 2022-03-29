CC=gcc

BDIR = ./bin/
IDIR = ./src/
SDIR = ./src/

CFLAGS=-I $(IDIR) -lm
OBJ = $(BDIR)rsa.o $(BDIR)prime.o
HEA = $(IDIR)rsa.h $(IDIR)prime.h
DEB=-ggdb -Wall

all: $(OBJ)
	make main

main : $(OBJ)
	$(CC) -o $(BDIR)$@ $(SDIR)$@.c $^ $(DEB) $(CFLAGS)

$(BDIR)%.o: $(SDIR)%.c $(HEA)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f bin/*
	rm -f vgcore*

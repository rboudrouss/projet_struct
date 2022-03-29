CC=gcc

BDIR = ./bin/
IDIR = ./src/
SDIR = ./src/

CFLAGS=-I $(IDIR) -lm
OBJ = $(BDIR)rsa.o $(BDIR)prime.o
# HEA = $(IDIR)rsa.h $(IDIR)prime.h
DEB=-ggdb -Wall

all: main

#% : $(OBJ)
#	$(CC) -o $(BDIR)$@ $(SDIR)$@.c $^ $(DEB) $(CFLAGS)

main : $(OBJ)
	$(CC) -o $(BDIR)$@ $(SDIR)$@.c $^ $(DEB) $(CFLAGS)

tests: $(OBJ)
	$(CC) -o $(BDIR)$@ $(SDIR)$@.c $^ $(DEB) $(CFLAGS)

uwu:
	$(CC) -o $(BDIR)$@ $(SDIR)$@.c $^ $(DEB) $(CFLAGS)

$(BDIR)%.o: $(SDIR)%.c $(IDIR)%.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f bin/*
	rm -f vgcore*

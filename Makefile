CC=gcc

BDIR = ./bin/
IDIR = ./head/
SDIR = ./src/

CFLAGS=-I $(IDIR)
DLC = $(IDIR)biblioLC.h $(IDIR)entreeSortieLC.h
OLC = $(BDIR)biblioLC.o $(BDIR)entreeSortieLC.o
DH = $(IDIR)biblioH.h $(IDIR)entreeSortieH.h
OH =  $(BDIR)biblioH.o $(BDIR)entreeSortieH.o
DEB=-ggdb -Wall

all: $(OLC) $(OH)
	make mainLC
	make testLC
	make mainH
	make testH
	make timer

timer: $(OH) $(OLC)
	make timerH
	make timerLC

%H: $(OH)
	$(CC) -o $(BDIR)$@ $(SDIR)$@.c $^ $(DEB) $(CFLAGS)

%LC: $(OLC)
	$(CC) -o $(BDIR)$@ $(SDIR)$@.c $^ $(DEB) $(CFLAGS)


$(BDIR)%.o: $(SDIR)%.c $(DLC) $(DH)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f bin/*
	rm -f vgcore*

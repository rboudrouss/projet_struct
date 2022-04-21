CC=gcc

BDIR = ./bin/
IDIR = ./head/
SDIR = ./src/
RDIR = ./report/

CFLAGS=-I $(IDIR) -lm -lssl -lcrypto
OBJ = $(BDIR)rsa.o $(BDIR)prime.o $(BDIR)keys.o $(BDIR)protec.o $(BDIR)signs.o $(BDIR)cells.o $(BDIR)block.o $(BDIR)btree.o $(BDIR)generate.o
DEB=-ggdb -Wall

TARGETS = main tests test

all: $(TARGETS)

$(TARGETS): $(OBJ)
	$(CC) -o $(BDIR)$@ $(SDIR)$@.c $^ $(DEB) $(CFLAGS)

$(BDIR)%.o: $(SDIR)%.c $(IDIR)%.h
	$(CC) -c -o $@ $< $(CFLAGS) $(DEB)

rapport:
	pandoc $(RDIR)*.md \
	-H $(RDIR)theme.tex \
    -V geometry:a4paper \
    -V mainfont="DejaVu Serif" \
    -V monofont="DejaVu Sans Mono" \
    --pdf-engine=xelatex \
	-o rapport.pdf

clean:
	rm -f bin/*
	rm -f vgcore*

DEBUG ?= 1

CC=gcc

BDIR = ./bin/
IDIR = ./head/
SDIR = ./src/
RDIR = ./report/
TDIR = ./tests/

CFLAGS=-I $(IDIR) -lm -lssl -lcrypto
DEB=-ggdb -Wall
ifeq ($(DEBUG), 1)
    CFLAGS += $(DEB)
else
    CFLAGS += -O3
endif

HEADS := $(shell find $(IDIR) -name *.h -not -name "*files.h")
OBJS := $(HEADS:$(IDIR)%.h=$(BDIR)%.o)

OBJS_TESTS := $(shell find $(TDIR) -name "*.c" -not -name "tests.c")

TARGETS = main test

all: folders $(TARGETS) tests

folders:
	mkdir -p bin blockchain/temp tests/blockchain/temp

$(TARGETS): $(OBJS)
	$(CC) -o $(BDIR)$@ $(SDIR)$@.c $^ $(DEB) $(CFLAGS)

$(BDIR)%.o: $(SDIR)%.c $(IDIR)%.h
	$(CC) -c -o $@ $< $(CFLAGS) $(DEB)

tests: $(OBJS) $(OBJS_TESTS)
	$(CC) -o $(BDIR)$@ $(TDIR)$@.c $^ $(DEB) $(CFLAGS)

%_tests.o: $(TDIR)%_tests.c
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
	rm -f vgcore* bin/*
	find blockchain -maxdepth 2 -type f -delete
	find tests/blockchain -maxdepth 2 -type f -delete
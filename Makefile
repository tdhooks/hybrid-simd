SHELL = /bin/bash

.SUFFIXES:
.SUFFIXES: .c .h .o

SRCDIR = src/
INCDIR = inc/
OBJDIR = obj/
BINDIR = bin/

_BIN = hybrid_simd
BIN = $(addprefix $(BINDIR),$(_BIN))

DEP = $(wildcard $(INCDIR)*.h)

_SRC = $(wildcard $(SRCDIR)*.c)
OBJ = $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(_SRC))

CC = mpicc
CFLAGS = -fopenmp -O2 -Wall -Wextra -Werror=implicit-function-declaration -pedantic -pipe -I$(INCDIR)
OFLAGS = -fopenmp

.PHONY: all debug
all: $(BIN)

debug: CFLAGS += -g
debug: all

$(BIN): $(OBJ) $(BINDIR)
	$(CC) -o $@ $(OBJ) $(OFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.c $(DEP) $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BINDIR):
	mkdir -p $@

$(OBJDIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BINDIR) $(OBJDIR)
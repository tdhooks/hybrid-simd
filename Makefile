SHELL = /bin/bash

.SUFFIXES:
.SUFFIXES: .c .h .o


SRCDIR = ./src/
INCDIR = ./inc/
OBJDIR = ./obj/
BINDIR = ./bin/


_BIN = hybrid_simd
BIN = $(addprefix $(BINDIR), $(_BIN))

SRC = $(wildcard $(SRCDIR)*.c)

_OBJ = $(patsubst $(SRCDIR)%.c, %.o, $(SRC))
OBJ = $(addprefix $(OBJDIR), $(_OBJ))

CC = mpicc
CFLAGS = -fopenmp -O2 -Wall -Wextra -Werror=implicit-function-declaration -pedantic -g -pipe -I$(INCDIR)
OFLAGS = -fopenmp


.PHONY: all
all: $(BIN)


$(BIN): $(OBJ) $(BINDIR)
	$(CC) $(OFLAGS) $< -o $@

$(BINDIR):
	mkdir -p $@

$(OBJ): $(SRC) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@


.PHONY: clean
clean:
	rm -rf $(BINDIR) $(OBJDIR)
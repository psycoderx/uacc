# Unique ANSI C Compiler
# Makefile

# ---------------------------------------------------------- #
# VARIABLES                                                  #
# ---------------------------------------------------------- #

CC = gcc

CC_WARNS = -std=c89 -pedantic -pedantic-errors -Wall -Wextra

LD = gcc

UACC_EXE = uacc

C_FILES = uacc.c

H_FILES = uacc.h

O_FILES = $(C_FILES:.c=.o)

# ---------------------------------------------------------- #
# TARGETS                                                    #
# ---------------------------------------------------------- #

.PHONY: all exec clean rm_o_files

all: exec

exec: $(UACC_EXE)

clean: rm_o_files

rm_o_files:
	rm -f $(O_FILES)

$(UACC_EXE): $(O_FILES)
	$(LD) -o $@ $<

%.o: %.c $(H_FILES)
	$(CC) $(CC_WARNS) -o $@ -c $<


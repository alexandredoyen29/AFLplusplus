CC=gcc
CC_OPTIONS=-g -I../../include/ -Wall -DDEBUG
LD_OPTIONS=
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

.PHONY: all clean

%.o: %.c
	${CC} -c ${CC_OPTIONS} $^ -o $@

csl_mutator: $(OBJ)
	${CC} ${LD_OPTIONS} $^ -o $@

all: $(csl_mutator)

clean:
	rm -f *.o
	rm -f csl_mutator
CC=gcc
CC_OPTIONS_BASE=-g -I../../include/ -Wall -Wno-unknown-pragmas
LD_OPTIONS_BASE=-shared -fPIC
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

ifeq ($(DEBUG), 1)
	CC_OPTIONS=${CC_OPTIONS_BASE} -DDEBUG
	LD_OPTIONS=
else
	CC_OPTIONS=${CC_OPTIONS_BASE}
	LD_OPTIONS=${LD_OPTIONS_BASE}
	TARGET_EXT=.so
endif

.PHONY: all clean

%.o: %.c
	${CC} -c ${CC_OPTIONS} ${LD_OPTIONS} $^ -o $@

csl_mutator: $(OBJ)
	${CC} ${LD_OPTIONS} $^ -o $@${TARGET_EXT}

all: $(csl_mutator)

clean:
	rm -f *.o
	rm -f csl_mutator
	rm -f csl_mutator.so
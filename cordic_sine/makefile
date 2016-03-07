CC=gcc
CFLAGS=-Wall -Dlinux -g
HEAD=basic_op.h oper_32b.h typedef.h testing.h cordic_sine.h
OBJS=basicop2.o testing.o oper_32b.o cordic_sine.o

all: cordic 

cordic: top_level.c ${OBJS} ${HEAD} 
	${CC} -o cordic top_level.c ${OBJS} ${CFLAGS} -lm

${OBJS}: ${HEAD}

clean:
	rm -rf *.o cordic 

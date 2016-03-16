CC=gcc
INC=-I../core_lib
CFLAGS=-Wall -Dlinux -g $(INC)
HEAD=../core_lib/basic_op.h ../core_lib/oper_32b.h ../core_lib/typedef.h ../core_lib/testing.h float_freq_selective_filter.h fixed_freq_selective_filter.h
OBJS=../core_lib/basicop2.o ../core_lib/testing.o ../core_lib/oper_32b.o float_freq_selective_filter.o fixed_freq_selective_filter.o
all: fsf 

fsf: top_level.c ${OBJS} ${HEAD} 
	${CC} -o fsf top_level.c ${OBJS} ${CFLAGS} -lm

${OBJS}: ${HEAD} 

clean:
	rm -rf *.o fsf 

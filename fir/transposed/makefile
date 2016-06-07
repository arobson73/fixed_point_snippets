CC=gcc
INC=-I../../../workspace_v6_1/core_lib/
INC_PATH=../../../workspace_v6_1/core_lib/
CFLAGS=-Wall -Dlinux -g $(INC)
HEAD=${INC_PATH}basic_op.h ${INC_PATH}oper_32b.h ${INC_PATH}typedef.h ${INC_PATH}testing.h 
OBJS=${INC_PATH}basicop2.o ${INC_PATH}testing.o ${INC_PATH}oper_32b.o 

all: fir_tran 

fir_tran: top_level.c ${OBJS} ${HEAD} 
	${CC} -o fir_tran top_level.c ${OBJS} ${CFLAGS} -lm

${OBJS}: ${HEAD} 

clean:
	rm -rf *.o fir_tran 

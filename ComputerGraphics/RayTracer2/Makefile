CC = c++
CFLAGS = -g -O2 -Wall -lm

OBJ_FILE = ray2.o
EXE_FILE = ray2

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE}

ray2.o: ray2.cpp
	${CC} ${CFLAGS} -c ray2.cpp

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}

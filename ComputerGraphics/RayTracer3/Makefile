CC = c++
CFLAGS = -g -O2 -Wall -lm

OBJ_FILE = ray3.o
EXE_FILE = ray3

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE}

ray2.o: ray3.cpp
	${CC} ${CFLAGS} -c ray3.cpp

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}

CC = c++
CFLAGS = -g -O2 -Wall -lm

OBJ_FILE = ray1.o
EXE_FILE = ray1

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE}

ray1.o: ray1.cpp
	${CC} ${CFLAGS} -c ray1.cpp

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}

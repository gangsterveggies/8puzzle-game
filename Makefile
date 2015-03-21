EXEC_NAME=PUZZLE
CC=g++

# CFLAGS= -Wall -Wno-write-strings -O0 -g
#CFLAGS= -w -Wno-write-strings -O3 -g
CFLAGS= -w -Wno-write-strings -O3 -g

SRC =                   	\
	main.cpp 		\
	board.cpp		\
	search.cpp 		\
	dfs.cpp 		\
	bfs.cpp 		\
	iddfs.cpp		\
	greedy.cpp		\
	astar.cpp

OBJ =  ${SRC:.cpp=.o}

#------------------------------------------------------------

all: ${EXEC_NAME}

${EXEC_NAME}: ${OBJ}
	${CC} ${CFLAGS} ${CLIBS} -o ${EXEC_NAME} ${OBJ}

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $+

clean:
	rm ${EXEC_NAME} *.o *~ *# -rf

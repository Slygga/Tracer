CC = gcc
LIBRARIES = curses
INCLUDE_DIRS = includes
SRC_DIRS = src

LIBS = $(patsubst %, -l%, $(LIBRARIES))
INCLUDE = $(wildcard $(INCLUDE_DIRS)/*.h)
INCLUDES = $(patsubst %, -I %, $(INCLUDE_DIRS))
SRC := $(wildcard $(SRC_DIRS)/*.c)

tracer: ${SRC} ${INCLUDE}
	${CC} -o $@ ${SRC} ${LIBS} ${INCLUDES}

clean:
	rm tracer

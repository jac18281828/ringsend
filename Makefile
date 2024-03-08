
INCLUDE = include/application.h \
	include/network.h \
	include/ringsender.h

SRC = src/application.c \
	src/network.c \
	src/ringsender.c \
	src/main.c

all: ${INCLUDE} ${SRC}
	gcc -o ringsend -O2 ${SRC} -I include -lpthread
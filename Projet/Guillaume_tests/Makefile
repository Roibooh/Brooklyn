CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -pthread

all: main

main: main.c graph.o binary_heap.o tsp.o dijkstra.o


.PHONY: clean

clean:
		${RM} *.o main 

CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -pthread

all: main

main: main.c graph.o tsp.o dijkstra.o graph.o binary_heap.o graph_print.o

.PHONY: clean

clean:
		${RM} *.o main 

CC = gcc
CFLAGS = -Wall -Wextra

all: main

main: main.c graph.o dijkstra.o graph.o binary_heap.o
dijkstra.o: dijkstra.c dijkstra.h binary_heap.o
graph.o:
binary_heap.o:

.PHONY: clean

clean:
		${RM} *.o main 

CC = gcc
CFLAGS = -Wall -Wextra

all: main

main: graph.o dijkstra.o
dijkstra.o: dijkstra.c dijkstra.h
graph.o: graph.c graph.h

.PHONY: clean

clean:
		${RM} graph.o dijkstra.o main 

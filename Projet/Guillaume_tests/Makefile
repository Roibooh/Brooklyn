CC = gcc
CFLAGS = -Wall -Wextra -g

all: main

main: graph.o dijkstra.o graph.o priority_queue.o
dijkstra.o: dijkstra.c dijkstra.h priority_queue.o
graph.o: graph.c graph.h
priority_queue.o: priority_queue.c priority_queue.h

.PHONY: clean

clean:
		${RM} *.o main 

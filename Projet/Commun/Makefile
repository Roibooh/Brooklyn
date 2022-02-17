CC = gcc
CFLAGS = -Wall -Wextra -g

all: main

main: graph.o dijkstra.o graph.o priority_queue.o graph_print.o
dijkstra.o: dijkstra.c dijkstra.h priority_queue.o
graph.o:
priority_queue.o:
graph_print.o:

.PHONY: clean

clean:
		${RM} *.o main 

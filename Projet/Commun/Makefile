CC = gcc
CFLAGS = -Wall -Wextra

all: main

main: main.c graph.o dijkstra.o graph.o priority_queue.o graph_print.o
dijkstra.o: dijkstra.c dijkstra.h priority_queue.o
graph.o:
priority_queue.o:
graph_print.o:

.PHONY: clean

clean:
		${RM} *.o main 

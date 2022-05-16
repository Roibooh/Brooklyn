CC = gcc
CFLAGS += -Wall -Wextra -pedantic -std=c99 -ggdb -O0
LIBS += -lm -pthread


all: main

main: main.c graph.o tsp.o ant.o dijkstra.o graph.o binary_heap.o graph_print.o  
	$(CC) $(CFLAGS) main.c tsp.o graph_print.o graph.o dijkstra.o ant.o binary_heap.o  -o main $(LIBS) 


.PHONY: clean

clean:
		${RM} *.o main 

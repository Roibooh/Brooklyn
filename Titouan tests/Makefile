CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = graph_print.c graph.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
LIBS += -lm -lmath

main: main.o graph_print.o graph.o generate_graph.o link_bikes.o generate_metro.o generate_tram.o generate_bus.o auxilary.o generate_walk.o generate_bike.o

generate_graph: generate_graph.o $(LIBS)
graph_print: graph_print.o graph.o
graph: graph.o

.PHONY: clean

clean:
		${RM} *.o main 
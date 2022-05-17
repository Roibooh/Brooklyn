CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O3  -g -p
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image`
LIBS += -lm -pthread
all: graph_GUI

graph_GUI: graph_GUI.c pixel_operations.o tsp.o graph.o graph_print.o
	$(CC) $(CFLAGS) graph_GUI.c pixel_operations.o tsp.o graph.o graph_print.o $(LIBS) $(LDLIBS)

#graph_GUI.o: graph_GUI.c pixel_operations.h tsp.h graph.h graph_print.h
#tsp.o:

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale
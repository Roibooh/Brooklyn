#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

#include "graph.h"
#include "tsp.h"
#include "graph_print.h"

void Hline(SDL_Surface* surface, int x, int y, int w);
void Vline(SDL_Surface* surface, int x, int y, int h);
void Hline2(SDL_Surface* surface, int x, int y, int w, Uint8 z, Uint8 a, Uint8 c);
void Vline2(SDL_Surface* surface, int x, int y, int h, Uint8 z, Uint8 a, Uint8 c);
void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void wait_for_keypressed();
void LoadGraphFile (size_t width, size_t height, size_t depart, size_t arrivee);
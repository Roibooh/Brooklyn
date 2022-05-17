#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

#include "graph.h"
#include "tsp.h"
#include "graph_print.h"

struct point 
{
	size_t number;
	int x;
	int y;
	struct point * next;
};

void Hline(SDL_Surface* surface, int x, int y, int w)
{
    SDL_Rect area;
    area.x = x;
    area.y = y;
    area.w = w;
    area.h = 3;

    SDL_FillRect(surface, &area, SDL_MapRGB(surface->format, 150, 150, 150));
}

void Vline(SDL_Surface* surface, int x, int y, int h)
{
    SDL_Rect area;
    area.x = x;
    area.y = y;
    area.w = 3;
    area.h = h;

    SDL_FillRect(surface, &area, SDL_MapRGB(surface->format, 150, 150, 150));
}

void Hline2(SDL_Surface* surface, int x, int y, int w, Uint8 z, Uint8 a, Uint8 c)
{
    SDL_Rect area;
    area.x = x;
    area.y = y;
    area.w = w;
    area.h = 3;

    SDL_FillRect(surface, &area, SDL_MapRGB(surface->format, z, a, c));
}

void Vline2(SDL_Surface* surface, int x, int y, int h, Uint8 z, Uint8 a, Uint8 c)
{
    SDL_Rect area;
    area.x = x;
    area.y = y;
    area.w = 3;
    area.h = h;

    SDL_FillRect(surface, &area, SDL_MapRGB(surface->format, z, a, c));
}		
			
void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

int main()
{

    SDL_Surface* image_surface ;
    SDL_Surface* screen_surface ;
    SDL_Surface* circle;

    init_sdl();

    //image_surface = load_image("my_image.jpg");
    
    size_t width =20;
    size_t height = 15;
    
    size_t Rw = width*1.15;
    size_t Rh = height*1.3;
    
    image_surface = SDL_CreateRGBSurface(0, width*Rw, height*Rh, 32, 0, 0, 0, 0); 
    screen_surface = display_image(image_surface);
    
    wait_for_keypressed();
    
    circle = load_image("circle.png");
    
    /*int x = SDL_BlitSurface(circle, NULL, image_surface, NULL);
    
    if (x != 0)
    {
    	SDL_FreeSurface(image_surface);
    	SDL_FreeSurface(screen_surface);
    	return 0;
    }*/
    
    SDL_Rect imgrect;
    
    imgrect.x = 0;
    imgrect.y = 0;
    imgrect.w = Rw;
    imgrect.h = Rh;
    
    int circle_width = circle->w;
    int circle_height = circle->h;
    int tmp_circlew = circle_width;
    int tmp_circleh = circle_height;
    
    size_t cpt_node = 0;
    
    struct point* senti = malloc(sizeof(struct node));
    
    struct point* tmp_senti = senti;
    
    for (size_t j = 0; j < height; j++)
    {
    
    	for (size_t i = 0; i < width; i++)
    	{
    		struct point* pt = malloc(sizeof(struct node));
    		pt->number = cpt_node;
    		cpt_node += 1;
    		pt->x = tmp_circlew/2;
    		pt->y = tmp_circleh/2;
    		tmp_senti->next = pt;
    		tmp_senti = pt;
    		SDL_BlitSurface(circle, NULL, image_surface, &imgrect);
    		imgrect.x += Rw;
    		Vline(image_surface, tmp_circlew/2, circle_height/2, Rh*(height-1));
    		tmp_circlew += 2*Rw;	
    	}
    	Hline(image_surface, circle_width/2, tmp_circleh/2, imgrect.x - Rw);
    	tmp_circleh += 2*Rh;
    	imgrect.x = 0;
    	imgrect.y += Rh;
    	tmp_circlew = circle_width;
    }
    
    struct graph* g = load_graph("../graphs/graph300/");
    int len_dest = 2;

    size_t destinations[len_dest];
    destinations[0] = 2;
    destinations[1] = 10;
    struct node* final = tsp_main(g, destinations, len_dest, 1);
    struct node* path = complete_path(final, 20);
    
    
    struct point* senti_tmp = senti;
    
    path->vertex = 2;
    
    while (path->next != NULL)
    {
    	size_t vertex = path->vertex;
    	size_t next_v = path->next->vertex;
    	for (size_t i = 0; i <= vertex; i++)
    	{
    		senti_tmp = senti_tmp->next;
    	}
    	int x1 = senti_tmp->x;
    	int y1 = senti_tmp->y;
    	senti_tmp = senti;
    	for (size_t j = 0; j <= next_v; j++)
    	{
    		senti_tmp = senti_tmp->next;
    	}
    	int x2 = senti_tmp->x;
    	int y2 = senti_tmp->y;
    	
    	if (next_v - vertex == 1)
    	{
    		if (path->next->transport == 0)
    		{
    			Hline2(image_surface, x1, y1, Rw, 250, 0, 0);
    		}
    		else if (path->next->transport == 1)
    		{
    			Hline2(image_surface, x1, y1, Rw, 0, 250, 0);
    		}
    		else if (path->next->transport == 3)
    		{
    			Hline2(image_surface, x1, y1, Rw, 0, 0, 250);
    		}
    		else if (path->next->transport == 4)
    		{
    			Hline2(image_surface, x1, y1, Rw, 250, 250, 0);
    		}
    			
    	}
    	else if (y2 > y1)
    	{
    		if (path->next->transport == 0)
    		{
    			Vline2(image_surface, x1, y1, Rh, 250, 0, 0);
    		}
    		else if (path->next->transport == 1)
    		{
    			Vline2(image_surface, x1, y1, Rh, 0, 250, 0);
    		}
    		else if (path->next->transport == 3)
    		{
    			Vline2(image_surface, x1, y1, Rh, 0, 0, 250);
    		}
    		else if (path->next->transport == 4)
    		{
    			Vline2(image_surface, x1, y1, Rh, 250, 250, 0);
    		}
    	}
    	else if (vertex - next_v == 1)
    	{
    		if (path->next->transport == 0)
    		{
    			Hline2(image_surface, x2, y2, Rw, 250, 0, 0);
    		}
    		else if (path->next->transport == 1)
    		{
    			Hline2(image_surface, x2, y2, Rw, 0, 250, 0);
    		}
    		else if (path->next->transport == 3)
    		{
    			Hline2(image_surface, x2, y2, Rw, 0, 0, 250);
    		}
    		else if (path->next->transport == 4)
    		{
    			Hline2(image_surface, x2, y2, Rw, 250, 250, 0);
    		}
    	}
    	else if (y1 > y2)
    	{
    		if (path->next->transport == 0)
    		{
    			Vline2(image_surface, x2, y2, Rh, 250, 0, 0);
    		}
    		else if (path->next->transport == 1)
    		{
    			Vline2(image_surface, x2, y2, Rh, 0, 250, 0);
    		}
    		else if (path->next->transport == 3)
    		{
    			Vline2(image_surface, x2, y2, Rh, 0, 0, 250);
    		}
    		else if (path->next->transport == 4)
    		{
    			Vline2(image_surface, x2, y2, Rh, 250, 250, 0);
    		}
    	}
    	
    			
    			
    	
    	/*if (x1 > x2)
    	{
    		Hline2(image_surface, x2, y2, Rw, 250);
    	}
    	else if (x2 > x1)
    	{
    		Hline2(image_surface, x1, y1, Rw, 250);
    	}
    	if (y1 > y2)
    	{
    		Vline2(image_surface, x2, y2, Rh,250);
    	}
    	else
    	{
    		Vline2(image_surface, x1, y1, Rh, 250);
    	}*/
    	path = path->next;
    	senti_tmp = senti;
    }
    
    SDL_SaveBMP(image_surface,"image.bmp");	
    			
    
    
    
    	
    			
    
    /*int width = image_surface->w;
    int height = image_surface->h;
    
    for (int i = 0; i <= height; i++)
    {
    	for (int j = 0; j <= width; j++)
    	{
    		Uint32 pixel = get_pixel(image_surface, i, j);
    		
    		Uint8 r, g, b;
    		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b
    		
    		Uint8 a = 0.3*r + 0.59*g + 0.11*b;
    		Uint32 pixel1 = SDL_MapRGB(image_surface->format, a, a, a);
    		
    		put_pixel(image_surface, i, j, pixel1);
    	}
    }*/
    
    update_surface(screen_surface, image_surface);

    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    
    return 0;

}
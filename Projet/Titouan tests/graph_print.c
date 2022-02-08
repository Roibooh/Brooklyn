#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"


void print_graph(size_t height, size_t width)
{
    size_t y, x;
    char n[5];
    char node[10];
    for(x = 0; x < height; ++x)
    {
        for(y = 0; y < width; ++y)
        {
            if (x*width + y < 10)
            {
                strcpy(node, "( ");
                sprintf(n, "%lu", x*width + y);
                strcat(node, n);
                strcat(node, " )");
            }
            else if (x*width + y < 100)
            {
                strcpy(node, "(");
                sprintf(n, "%lu", x*width + y);
                strcat(node, n);
                strcat(node, " )");
            }
            else
            {
                strcpy(node, "(");
                sprintf(n, "%lu", x*width + y);
                strcat(node, n);
                strcat(node, ")");
            }

            if (y != width-1)
                printf("%s---", node);
            else
                printf("%s\n", node);
        }
        if (x != height-1)
        {
            for(y = 0; y < width; ++y)
            {
                if (y != width-1)
                    printf("  |     ");
                else
                    printf("  |\n");
            }
        }
    }
}


//------------------------------------------------------------------------------


int find_neighbor(int *path, int length, int node)
{
    int counter = 0;
    while (path[counter] != node && counter < length)
    {
        counter++;
    }
    if(path[counter] != node)
        return 0;
    if (counter == 0)
        return path[1];
    if (counter == length-1)
        return path[length-2];
    else
        if (path[counter-1] > path[counter+1])
            return path[counter-1];
        else
            return path[counter+1];
    //a refaire avec les liste chainés
    // et rajouter le cas où les deux voisins sont plus grands que node
}

void print_path(int *path, int path_length, size_t height, size_t width)
{
    size_t y, x;
    size_t i_neighbor;
    for(x = 0; x < height; ++x)
    {
        for(y = 0; y < width; ++y)
        {
            i_neighbor = find_neighbor(path, path_length, x*width + y);
            size_t neighbor = (size_t)i_neighbor;
            if (x*width + y < 10)
            {
                if (y != width-1)
                    if (neighbor == x*width + y + 1)
                        printf("( %lu )%s---%s", x*width + y, KRED, KNRM);
                    else
                        printf("( %lu )---", x*width + y);
                else
                    printf("( %lu )\n", x*width + y);
            }
            else if (x*width + y < 100)
            {
                if (y != width-1)
                    if (neighbor == x*width + y + 1)
                        printf("(%lu )%s---%s", x*width + y, KRED, KNRM);
                    else
                        printf("(%lu )---", x*width + y);
                else
                    printf("(%lu )\n", x*width + y);
            }
            else
            {
                if (y != width-1)
                    if (neighbor == x*width + y + 1)
                        printf("(%lu)%s---%s", x*width + y, KRED, KNRM);
                    else
                        printf("(%lu)---", x*width + y);
                else
                    printf("(%lu)\n", x*width + y);
            }
        }
        if (x != height-1)
        {
            for(y = 0; y < width; ++y)
            {
                i_neighbor = find_neighbor(path, path_length, x*width + y);
                size_t neighbor = (size_t)i_neighbor;
                if (neighbor == (x+1)*width + y)
                {
                    if (y != width-1)
                        printf("%s  |     %s", KRED, KNRM);
                    else
                        printf("%s  |\n%s", KRED, KNRM);
                }
                else
                {
                    if (y != width-1)
                        printf("  |     ");
                    else
                        printf("  |\n");
                }
            }
        }
    }
}


int main()
{
    //int test[3] = {8, 7, 2};
    int edges[10] = {0, 1, 2, 10, 18, 19, 20, 12, 13, 5};
    print_path(edges, 10, 4, 8);
}
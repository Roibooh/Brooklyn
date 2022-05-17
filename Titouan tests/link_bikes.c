#include "link_bikes.h"


int give_weight(int node_1, int node_2, int height, int width)
{
    int x_n1 = node_1%width;
    int y_n1 = node_1/height;
    
    int x_n2 = node_2%width;
    int y_n2 = node_2/height;

    if (y_n1 == y_n2)
        return abs(x_n1 - x_n2);
    if (x_n1 == x_n2)
        return abs(y_n1 - y_n2);
    return abs(x_n1 - x_n2) + abs(y_n1 - y_n2);
}

void link_bikes(char* data, char* res, int height, int width, int bike_number, int bike_weight)
{
    FILE *read_f = fopen(data, "r");
    if(read_f == NULL)
        err(1, "Unable to open file!");

    FILE *write_f = fopen(res, "w");
    if(write_f == NULL)
        err(1, "Unable to open file!");

    char chunk[8];
    int chunk_i;
    int arr[bike_number];
    int counter = 0;
    while(fgets(chunk, sizeof(chunk), read_f) != NULL && counter<bike_number)
    {
        chunk_i = atoi(chunk);
        arr[counter] = chunk_i;
        counter++;
    }
    int i, j, weight;
    char w[4], node_i[4], node_j[4];
    for(i = 0; i < bike_number-1; i++)
    {
        for(j = i+1; j < bike_number; j++)
        {
            weight = give_weight(arr[i], arr[j], height, width);
            weight *= bike_weight;
            sprintf(node_i, "%i", arr[i]);
            fputs(node_i, write_f);
            fputs(" ", write_f);
            sprintf(node_j, "%i", arr[j]);
            fputs(node_j, write_f);
            fputs(" ", write_f);
            sprintf(w, "%i", weight);
            fputs(w, write_f);
            if ((i != bike_number-2) || (j != bike_number-1))
                fputs("\n", write_f);
        }
    }
    int seconds = 1;
    wait(&seconds);
    fclose(write_f);
    fclose(read_f);
}
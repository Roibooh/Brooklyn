#include "auxilary.h"

#define METRO_WEIGHT 6


//Generating metros nodes and arcs
//Still need to look into bettering this function (upgraded the function in generate_metro_v2)
//size of graph: height * width

int *generate_metro_v1(int height, int width)
{
    srand(time(0));
    FILE *file = fopen("gen_graph/gen_metro.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }
    int counter = 0;
    int vertical_ways = rand()%(height/10);
    if (vertical_ways == 0)
        vertical_ways = rand()%(height/10);
    int horizontal_ways = rand()%(width/10);
    if (horizontal_ways == 0)
        horizontal_ways = rand()%(width/10);
    int arr_len = vertical_ways*horizontal_ways*height*width/50;
    int *nodes = calloc(sizeof(int), arr_len);
    for (int x = 0; x < arr_len; x++)
    {
        *(nodes+x) = -1;
    }
    //printf("amout of line = %i + %i\n", vertical_ways, horizontal_ways);

    int start_width = 0, start_height = 0;
    int mem_w = 0, mem_h = 0;
    int neg, i;
    for(i = 0; i < vertical_ways; i++)
    {
        neg = rand()%2;
        if (neg)
            start_height = ((i+1)*height/(vertical_ways+1))+rand()%3;
        else
            start_height = ((i+1)*height/(vertical_ways+1))-rand()%3;
        start_width = rand()%5;
        *(nodes+counter) = start_height*width + start_width;
        counter++;
        while(start_width+5 < width)
        {
            mem_h = next_node_height(height, start_height, 0, 3);
            mem_w = next_node_width(width, start_width, 7, 3);
            regroup(width, nodes, arr_len, &mem_h, &mem_w, 3);
            print_to_file(width, start_height*width+start_width, mem_h*width+mem_w, METRO_WEIGHT, file);
            start_height = mem_h;
            start_width = mem_w;
            *(nodes+counter) = start_height*width + start_width;
            counter++;
        }
    }
    for(i = 0; i < horizontal_ways; ++i)
    {
        neg = rand()%2;
        if (neg)
            start_width = ((i+1)*width/(horizontal_ways+1))+rand()%3;
        else
            start_width = ((i+1)*width/(horizontal_ways+1))-rand()%3;
        start_height = rand()%6;
        *(nodes+counter) = start_height*width + start_width;
        counter++;
        while(start_height+5 < height)
        {
            mem_h = next_node_height(height, start_height, 7, 3);
            mem_w = next_node_width(width, start_width, 0, 3);
            regroup(width, nodes, arr_len, &mem_h, &mem_w, 3);
            print_to_file(width, start_height*width+start_width, mem_h*width+mem_w, METRO_WEIGHT, file);
            start_height = mem_h;
            start_width = mem_w;
            *(nodes+counter) = start_height*width + start_width;
            counter++;
        }
    }
    int seconds = 1;
    wait(&seconds);
    fclose(file);
    return nodes;
}



void next_stop (int height, int width, int *new_h, int *new_w)
{
    int sign, add_rand, add_rand_w;
    add_rand = rand()%(7+rand()%2); // 6 or 7
        if(*new_h > 7){
            if(*new_h < height-7){
                sign = rand()%2; // sign
                if (sign == 0)
                    *new_h += add_rand;
                else
                    *new_h -= add_rand;
            }
            else{
                *new_h -= add_rand;
            }
        }
        else
            *new_h += add_rand;

        add_rand_w = 8-add_rand+rand()%2; //so that the total length of the line is 7 or 8 
        if(*new_w > 7){
            if(*new_w < width-7){
                sign = rand()%2; // sign
                if (sign == 0)
                    *new_w += add_rand_w;
                else
                    *new_w -= add_rand_w;
            }
            else{
                *new_w -= add_rand_w;
            }
        }
        else
            *new_w += add_rand_w;
        ////////////////////////////////////////////////add the regroup function here
}


//second version, improved graph generation
int* generate_metro_v2(int height, int width)
{
    srand(time(0));
    FILE *file = fopen("gen_graph/gen_metro.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }
    int q_ways = height%12 + width%12/2;
    printf("q = %d\n", q_ways);
    int* metros = malloc(sizeof(int)* q_ways*height*width/100);
    for (int x = 0; x < q_ways*height*width/100; x++)
    {
        *(metros+x) = -1;
    }
    int counter = 0;
    int count;

    int max_w, min_w;//, progress;
    int sign;//, add_rand, add_rand_w;
    int dif_h, dif_w, dif_h_sign, dif_h_sign_mem, dif_w_sign;
    int new_h, new_w, next_h, next_w, temp_w, temp_h;
    for (int i = 0; i < q_ways; i++)
    {
        //generate first metro stop not too close from existing ones
        new_h = rand()%height;
        new_w = rand()%width;
        count = 1000;
        while(spawn_too_close(metros, new_h, new_w, counter, width) && count != 0)
        {
            count--;
            new_h = rand()%height;
            new_w = rand()%width;
        }
        if (count == 0)
            continue; //no empty spot found so lets go to the next metro way
        *(metros+counter) = new_h*width+new_w;
        counter++;
        printf("h = %i  and w =%i\n", new_h, new_w);

        //find the next stop
        next_h = new_h;
        next_w = new_w;
        next_stop (height, width, &next_h, &next_w);
        *(metros+counter) = next_h*width+next_w;
        counter++;
        printf("h = %i  and w =%i\n", next_h, next_w);


        //pseudo recursive to find the next stops
        //a bit long
        dif_h = abs(new_h-next_h)/2;
        dif_h_sign = next_h>new_h; //direction on height - 1 means going down - 0 means going up
        dif_w = abs(new_w-next_w)/2;
        dif_w_sign = next_w>next_h; //direction on width - 1 goes right - 0 goes left
        while(max(dif_h, min(next_h,height-next_h)) + max(dif_w, min(next_w, width-next_w)) > 9 && rand()%(height*width/100)) //not directed at a corner //last condition should be changed
        {
            dif_h_sign_mem = dif_h_sign;
            max_w = max(dif_w, min(next_w, width-next_w)); //le maximum qu'on peux parcourir en largeur
            min_w = min(dif_w, min(next_w, width-next_w)); //la plus petite des plus grandes distance qu'on peux parcourir en largeur
            printf("max_w is %i and min_w is %i\n", max_w, min_w);
            //working out the new height
            if (dif_h + max_w > 8) // je peux aller en direction de l'ancien point 
            {
                if (dif_h_sign) //je descend
                {
                    if (height-1-next_h + max_w > 8) //whats left of height + max width possible
                    {
                        sign = rand()%2; //up or down
                        if (sign) //going down
                        {
                            dif_h = min(height-1-next_h, rand()%(min_w) + (7-min(7,min_w))); //min here just for precaution
                            temp_h = next_h+ dif_h*dif_h_sign;
                        }
                        else //going up
                        {
                            dif_h = min(rand()%dif_h + (7-min(min_w, 7)), dif_h);
                            dif_h_sign = -dif_h_sign;
                            temp_h = next_h+ dif_h*dif_h_sign;
                            printf("h case 2\n");
                        }
                    }
                    else
                    {
                        dif_h = min(dif_h, rand()%dif_h + (7-min(min_w, 7))); //min here for precaution
                        dif_h_sign = -dif_h_sign;
                        temp_h = next_h+ dif_h*dif_h_sign;
                        printf("h case 3\n");
                    }
                }
                else //je monte
                {
                    if (next_h + max_w > 8) //si je peux monter 
                    {
                        sign = rand()%2; //up or down
                        if (sign) //going up
                        {
                            dif_h = min(next_h, rand()%min_w + (7-min(min_w, 7))); //min here just for precaution
                            temp_h = next_h+ dif_h*dif_h_sign;
                            printf("h case 4\n");
                        }
                        else //going down
                        {
                            dif_h = min(dif_h, rand()%dif_h + (7-min(min_w, 7)));
                            dif_h_sign = -dif_h_sign;
                            temp_h = next_h+ dif_h*dif_h_sign;
                            printf("h case 5\n");
                        }
                    }
                    else //je descend vers l'ancien point
                    {
                        dif_h = min(dif_h, rand()%dif_h + (7-min(min_w, 7)));
                        dif_h_sign = -dif_h_sign;
                        temp_h = next_h+ dif_h*dif_h_sign;
                        printf("h case 6\n");
                    }
                }
            }
            else // je m'éloigne de l'ancien point
            {
                dif_h = rand()%min_w + (7-min(min_w, 7)); //traveling by this much
                if (dif_h_sign)
                {
                    dif_h = min(height-1-next_h, dif_h); // trying to settle error bellow
                    printf("h case 7\n");
                }
                else
                {
                    dif_h = min(next_h, dif_h); // trying to settle error bellow
                    printf("h case 8\n");
                }                  
                temp_h = next_h+dif_h*dif_h_sign;
            }

            //working out new width
            if (dif_h_sign_mem != dif_h_sign) //we changed direction: de retour vers l'ancien point en heauteur
            {
                dif_w = 7-dif_h;//or 8
                temp_w = next_w + dif_w*dif_w_sign; //je m'éloigne de l'ancien point en largeur
            }
            else
            {
                if (dif_w + dif_h >= 7) //peux retourner vers l'ancien point
                {
                    if(dif_w_sign) //going right
                    {
                        if (dif_h + width - next_w > 7) // can go right 
                        {
                            sign = rand()%2; //1 is right 0 is left
                            if (sign)
                            {
                                dif_w = 7 - dif_h +rand()%2;
                                temp_w = next_w + dif_w;                                                              
                            }
                            else
                            {
                                dif_w = 7 - dif_h;
                                temp_w = next_w - dif_w;
                                printf("w case 0\n");
                                dif_w_sign = -dif_w_sign;
                            }
                        }
                        else //can only go back to ancient point #franglais
                        {
                            dif_w = 7 - dif_h;
                            temp_w = next_w - dif_w;
                            dif_w_sign = -dif_w_sign;
                            printf("w case 1\n");
                        }
                    }
                    else
                    {
                        if (dif_h + next_w > 7) // can go left 
                        {
                            sign = rand()%2; //1 is right 0 is left
                            if (sign)
                            {
                                dif_w = 7 - dif_h;
                                temp_w = next_w + dif_w;
                                dif_w_sign = -dif_w_sign;
                            }
                            else
                            {
                                dif_w = 7 - dif_h;// +rand()%2;
                                temp_w = next_w - dif_w;
                                printf("w case 2\n");
                            }
                        }
                        else // can only go back to ancient point
                        {
                            dif_w = 7 - dif_h;
                            temp_w = next_w + dif_w;
                            printf("w case 3\n");
                            dif_w_sign = -dif_w_sign;
                        }
                    }
                }
                else //peux pas retourner vers l'ancien point
                {
                    if (dif_w_sign) //1 is right 0 is left
                    {
                        dif_w = 7 - dif_h +rand()%2;
                        temp_w = next_w + dif_w;
                    }
                    else
                    {
                        dif_w = 7 - dif_h +rand()%2;
                        temp_w = next_w - dif_w;
                        printf("w case 4\n");
                    }
                }
            }
            dif_h /= 2;
            dif_w /= 2;
            next_h = temp_h;
            next_w = temp_w;
            printf("h = %i  and w =%i\n", next_h, next_w);
            //posibly add regroup function here;
            *(metros+counter) = temp_h*width+temp_w;
            counter++;
        }
        printf("metro line %i is complete -----------------------\n", i);
    }
    return metros;
    fclose(file);
}
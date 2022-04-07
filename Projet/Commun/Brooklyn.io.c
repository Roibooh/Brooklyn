//gcc  -Wall -Wextra -o Brooklyn.io Brooklyn.io.c tsp.c graph.c binary_heap.c dijkstra.c   $(pkg-config gtk+-3.0 --cflags --libs) -rdynamic
#include <gtk/gtk.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "graph.h"
#include "tsp.h"
#include "graph_print.h"

static GtkWidget *depart;
static GtkWidget *arrivee;
static GtkWidget *lanceur;
static GtkWidget *result;


void calculate(__attribute__((unused)) GtkWidget *widget, 
               __attribute__((unused)) gpointer   data)
{
    size_t num1 = atol((char *)gtk_entry_get_text(GTK_ENTRY(depart)));
    size_t num2 = atol((char *)gtk_entry_get_text(GTK_ENTRY(arrivee)));
    
    
    
    
    struct graph* g = load_graph("../graphs/graph300/");
    int len_dest = 2;

    size_t destinations[len_dest];
    destinations[0] = num1;
    destinations[1] = num2;
    struct node* final = tsp_main(g, destinations, len_dest, 1);
    
    char buffer[1000];
    struct node* tmp = final;
    while (tmp->next != NULL)
    {
    	sprintf(buffer, "%lu->", tmp->vertex);
    	tmp = tmp->next;
    }
    gtk_label_set_text (GTK_LABEL(result), (const gchar*) buffer);
    
    //char buffer[32];
    //snprintf(buffer, sizeof(buffer), "%ld", num1 + num2);
    
    //gtk_label_set_text(GTK_LABEL(result), buffer);
    	
    	
    
    
    
}


void quitButtonClicked(__attribute__((unused)) GtkWidget *widget, 
                       __attribute__((unused)) gpointer   data)
{
    g_print("Quit Clicked\n");
    gtk_main_quit();
}


// Handle the user trying to close the window 
gboolean windowDelete(__attribute__((unused)) GtkWidget *widget, 
                      __attribute__((unused)) GdkEvent  *event,
                      __attribute__((unused)) gpointer   data)
{
    g_print("%s called.\n",__FUNCTION__);
    return TRUE;    // Returning TRUE stops the window being deleted.
                    // Returning FALSE allows deletion.   
}

int main ( int argc, char **argv) {
 
    GtkWidget *window;
    GtkBuilder *builder = NULL;
 
    gtk_init (&argc , &argv);
 
    builder = gtk_builder_new();
 
    if( gtk_builder_add_from_file (builder,"Brooklyn.io.glade" , NULL) == 0)
    {
        printf("gtk_builder_add_from_file FAILED\n");
        return(0);
    }
 
    window  = GTK_WIDGET (gtk_builder_get_object (builder,"window1"));
   
 
    gtk_builder_connect_signals(builder,NULL);
    
    depart = GTK_WIDGET(gtk_builder_get_object(builder, "depart"));
    arrivee = GTK_WIDGET(gtk_builder_get_object(builder, "arrivee"));
    lanceur = GTK_WIDGET(gtk_builder_get_object(builder, "lanceur"));
    result = GTK_WIDGET(gtk_builder_get_object(builder, "result"));
    
    g_signal_connect(lanceur, "clicked", G_CALLBACK(calculate), NULL);
    
 
    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
}
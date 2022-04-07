//gcc  -Wall -Wextra -o Brooklyn.io Brooklyn.io.c $(pkg-config gtk+-3.0 --cflags --libs) -rdynamic
#include <gtk/gtk.h>

static GtkWidget *depart;
static GtkWidget *arrivee;
static GtkWidget *lanceur;
static GtkWidget *result;


void calculate(__attribute__((unused)) GtkWidget *widget, 
               __attribute__((unused)) gpointer   data)
{
    int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(depart)));
    int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(arrivee)));
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", num1 + num2);
    
    gtk_label_set_text(GTK_LABEL(result), buffer);
    
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
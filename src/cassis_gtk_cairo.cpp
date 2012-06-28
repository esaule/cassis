#include <math.h>
#include <gtk/gtk.h>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "cairo.h"

#include <gtk/gtk.h>

#include "GameState.hpp"
#include "IA.hpp"

GdkImage *im;
GtkWidget *imWind;


#include "cassis_cairo.hpp"


  static gboolean
  key_press (GtkWidget *widget, GdkEventKey *event, gpointer data)
  {
    CassisDisplay* g = (CassisDisplay*)data;

    std::cout << event->hardware_keycode<<std::endl;

    gtk_widget_queue_draw (imWind);

    return TRUE;

  }

  static gboolean
  button_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
  {
    CassisDisplay* g = (CassisDisplay*)data;

    g->clickat(event->x, event->y);
    
    gtk_widget_grab_focus(widget);
    gtk_widget_queue_draw (imWind);

    return TRUE;
  }


  static gboolean
  expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
  {
    CassisDisplay* g = (CassisDisplay*)data;

    cairo_t *cr;
    /* get a cairo_t */  

    cr = gdk_cairo_create (imWind->window);
    
    /* set a clip region for the expose event */
    cairo_rectangle (cr,
 		     event->area.x, event->area.y,
 		     event->area.width, event->area.height);
    cairo_clip (cr);
    
    g->setSizeX (imWind->allocation.width);
    g->setSizeY (imWind->allocation.height);

    g->render(cr);
    cairo_destroy (cr);
    return TRUE;
  }




void
destroy (void)
{
  gtk_main_quit ();
}

int
main (int argc, char *argv[])
{
  gtk_init (&argc, &argv);
  GtkWidget *window;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_set_application_name("cairo gtk");

  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC (destroy), NULL);
  gtk_container_border_width (GTK_CONTAINER (window), 10);

  CassisDisplay g;

  imWind = gtk_label_new("You should not see this message!");

  g_signal_connect (G_OBJECT (imWind), "expose_event",
		    G_CALLBACK (expose), &g);


  g_signal_connect (G_OBJECT (window), "key-press-event",
		    G_CALLBACK (key_press), &g);
  
  g_signal_connect (G_OBJECT (window), "button-press-event",
		    G_CALLBACK (button_press), &g);
  
  
  
  GTK_WIDGET_SET_FLAGS(window, GTK_CAN_FOCUS);
  gtk_widget_add_events (window, GDK_KEY_PRESS_MASK);
  gtk_widget_add_events (window, GDK_BUTTON_PRESS_MASK);



  g.show();

  gtk_container_add(GTK_CONTAINER(window), imWind);
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}


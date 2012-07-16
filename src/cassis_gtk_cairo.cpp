//This file is part of Cassis.

//Copyright (C) 2012, Erik Saule

//Cassis is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Cassis is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Cassis.  If not, see <http://www.gnu.org/licenses/>.

#define GTK
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
#include "cairo_menu_selector.hpp"



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

  //  CassisDisplay g;
  CairoMenuSelector g;

  imWind = gtk_label_new("You should not see this message!");

  g_signal_connect (G_OBJECT (imWind), "expose_event",
		    G_CALLBACK (CairoGraphicController::expose), &g);


  g_signal_connect (G_OBJECT (window), "key-press-event",
		    G_CALLBACK (CairoGraphicController::key_press), &g);
  
  g_signal_connect (G_OBJECT (window), "button-press-event",
		    G_CALLBACK (CairoGraphicController::button_press), &g);
  
  
  
  GTK_WIDGET_SET_FLAGS(window, GTK_CAN_FOCUS);
  gtk_widget_add_events (window, GDK_KEY_PRESS_MASK);
  gtk_widget_add_events (window, GDK_BUTTON_PRESS_MASK);



  gtk_widget_show (imWind);

  gtk_container_add(GTK_CONTAINER(window), imWind);
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}


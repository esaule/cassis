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

class GraphDisplay
{
private:
  GdkImage *im;
  GtkWidget *imWind;

  //window size
  int sizeX, sizeY;

  //position of the frame
  double frameXl, frameXh, frameYl, frameYh;

  //position of the pdfs' labels
  double nameFontsize, nameXoffset, nameXspacing, nameYoffset, nameYspacing;

  //colors
  cairo_pattern_t * bgcolor;
  cairo_pattern_t * fgcolor;

  cairo_pattern_t * colors[4];

  

  int xoffset;
  int yoffset;

  double interTaskSpace;
  double timeDilat;

public:
  GtkWidget* getWidget()
  {
    return imWind;
  }

  GraphDisplay()
  {
    imWind = gtk_label_new("You should not see this message!");

    g_signal_connect (G_OBJECT (imWind), "expose_event",
		      G_CALLBACK (GraphDisplay::expose), this);

    timeDilat = 10000;

    xoffset = 0;
    yoffset = 0;

    interTaskSpace = 20;

    frameXl=.07;
    frameYl=.05;
    frameXh=.95;
    frameYh=.9;

    nameFontsize = 20;
    nameXoffset = .07;
    nameYoffset = 0.1;
    nameXspacing = 0;
    nameYspacing = .05; 
      
    bgcolor = cairo_pattern_create_rgb(1,1,1);
    fgcolor = cairo_pattern_create_rgb(0,0,0);

    colors[0]=cairo_pattern_create_rgb(0,0,0);
    colors[1]=cairo_pattern_create_rgb(1,0,0);
    colors[2]=cairo_pattern_create_rgb(0,1,0);
    colors[3]=cairo_pattern_create_rgb(0,0,1);
  }

  ~GraphDisplay()
  {
    cairo_pattern_destroy(bgcolor);
    cairo_pattern_destroy(fgcolor);

    cairo_pattern_destroy(colors[0]);   
    cairo_pattern_destroy(colors[1]);
    cairo_pattern_destroy(colors[2]);
    cairo_pattern_destroy(colors[3]);
  }

  void show()
  {
    gtk_widget_show (imWind);
    //    gtk_widget_show (drawing);
  }


  static gboolean
  key_press (GtkWidget *widget, GdkEventKey *event, gpointer data)
  {
    GraphDisplay* g = (GraphDisplay*)data;

    std::cout << event->hardware_keycode<<std::endl;

    gtk_widget_queue_draw (g->getWidget());

    return TRUE;

  }

  static gboolean
  button_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
  {
    GraphDisplay* g = (GraphDisplay*)data;

    std::cout<<"button press event"<<std::endl;

    gtk_widget_grab_focus(widget);

    return TRUE;

  }


  static gboolean
  expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
  {
    GraphDisplay* g = (GraphDisplay*)data;

    cairo_t *cr;
    /* get a cairo_t */  

    cr = gdk_cairo_create (g->imWind->window);
    
    /* set a clip region for the expose event */
    cairo_rectangle (cr,
 		     event->area.x, event->area.y,
 		     event->area.width, event->area.height);
    cairo_clip (cr);
    
    cairo_translate(cr, g->imWind->allocation.x, g->imWind->allocation.y);

    g->sizeX = g->imWind->allocation.width;
    g->sizeY = g->imWind->allocation.height;

    g->render(cr);
    cairo_destroy (cr);
    return TRUE;
  }

  void scaled_stroke(cairo_t *cr)
  {
    double xwise = 1;
    double ywise = 1;
    cairo_save(cr);
    cairo_device_to_user_distance (cr,&xwise,&ywise);
    
    cairo_scale(cr,xwise,ywise);
    cairo_stroke(cr);
    cairo_restore(cr);
  }


  void scaled_show_text (cairo_t *cr, const std::string &s)
  {
    double xwise = 1;
    double ywise = 1;
    cairo_save(cr);
    cairo_device_to_user_distance (cr,&xwise,&ywise);
   
    cairo_scale(cr,xwise,ywise);
    cairo_show_text (cr, s.c_str() );
    cairo_restore(cr);
  }

  // double time_to_x (timetype time)
  // {
  //   return ((double)time)/timeDilat;
  // }

  void render(cairo_t* cr)
  { 
    //paint background
    cairo_set_source(cr, bgcolor);
    cairo_paint(cr);

    cairo_set_source(cr, fgcolor);
    
    cairo_translate(cr, -xoffset, -yoffset);

    // for (std::vector<taskinfo>::const_iterator it = graphs.begin();
    // 	 it != graphs.end();
    // 	 it++)
    //   {
    // 	const taskinfo& t = *it;

    // 	//	std::cout<<t.id<<" "<<time_to_x(t.release)<<std::endl;
    // 	//waiting line
    // 	cairo_set_source(cr, colors[0]);
    // 	cairo_move_to (cr, time_to_x(t.release), t.id*interTaskSpace);
    // 	cairo_line_to (cr, time_to_x(t.start), t.id*interTaskSpace);
    // 	cairo_stroke(cr);

    // 	//processing line
    // 	cairo_set_source(cr, colors[2]);
    // 	cairo_move_to (cr, time_to_x(t.start), t.id*interTaskSpace);
    // 	cairo_line_to (cr, time_to_x(t.completion), t.id*interTaskSpace);
    // 	cairo_stroke(cr);

    // 	//write ID number
    // 	{
    // 	  cairo_set_source(cr, fgcolor);
    // 	  cairo_move_to(cr, time_to_x(t.release) - 20, t.id*interTaskSpace);
    // 	  std::stringstream ss;
    // 	  ss<<t.id;
    // 	  scaled_show_text(cr,ss.str());
    // 	}

    // 	//write m
    // 	{
    // 	  cairo_set_source(cr, fgcolor);
    // 	  cairo_move_to(cr, time_to_x(t.start) + 20, t.id*interTaskSpace);
    // 	  std::stringstream ss;
    // 	  ss<<"["<<t.release<<" , "<<t.completion<<"] / "<<t.start;
    // 	  ss<<" m = "<<t.m;
    // 	  timetype f = (t.completion-t.release);
    // 	  //	  ss<<" f = "<<f;
    // 	  //	  ss<<" p1 = "<<t.p1;
    // 	  ss<<" s = "<<((double)f)/t.p1;
    // 	  scaled_show_text(cr,ss.str());
    // 	}

    //   }
	   
    
  }

};


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

  GraphDisplay g;

  g_signal_connect (G_OBJECT (window), "key-press-event",
		    G_CALLBACK (GraphDisplay::key_press), &g);
  
  g_signal_connect (G_OBJECT (window), "button-press-event",
		    G_CALLBACK (GraphDisplay::button_press), &g);
  
  
  
  GTK_WIDGET_SET_FLAGS(window, GTK_CAN_FOCUS);
  gtk_widget_add_events (window, GDK_KEY_PRESS_MASK);
  gtk_widget_add_events (window, GDK_BUTTON_PRESS_MASK);



  g.show();

  gtk_container_add(GTK_CONTAINER(window), g.getWidget());
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}


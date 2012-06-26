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

class CassisDisplay
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

  cairo_pattern_t * colorp1;
  cairo_pattern_t * colorp2;

  Cassis::Engine::GameState gs;

  int gamecenterx;
  int gamecentery;
  int gameradius;
  int vertexradius;

  int xoffset;
  int yoffset;

  double interTaskSpace;
  double timeDilat;

  int selected;

public:
  GtkWidget* getWidget()
  {
    return imWind;
  }

  CassisDisplay()
  {
    selected = -1;
    gamecenterx = gamecentery = 200;
    gameradius = 100;
    vertexradius = 20;

    imWind = gtk_label_new("You should not see this message!");

    g_signal_connect (G_OBJECT (imWind), "expose_event",
		      G_CALLBACK (CassisDisplay::expose), this);

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

    colorp1 = cairo_pattern_create_rgb(1,0,0);
    colorp2 = cairo_pattern_create_rgb(0,0,1);

  }

  ~CassisDisplay()
  {
    cairo_pattern_destroy(bgcolor);
    cairo_pattern_destroy(fgcolor);

    cairo_pattern_destroy(colorp1);   
    cairo_pattern_destroy(colorp2);
  }

  void show()
  {
    gtk_widget_show (imWind);
    //    gtk_widget_show (drawing);
  }


  static gboolean
  key_press (GtkWidget *widget, GdkEventKey *event, gpointer data)
  {
    CassisDisplay* g = (CassisDisplay*)data;

    std::cout << event->hardware_keycode<<std::endl;

    gtk_widget_queue_draw (g->getWidget());

    return TRUE;

  }

  static gboolean
  button_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
  {
    CassisDisplay* g = (CassisDisplay*)data;

    g->clickat(event->x, event->y);
    
    gtk_widget_grab_focus(widget);
    gtk_widget_queue_draw (g->getWidget());

    return TRUE;
  }

  void clickat(int x, int y)
  {
    int m = matchVertex(x, y);

    std::cout<<"button press event at "<<x<<" "<<y
	     <<" "<<m<<std::endl;

    if (m == -1)
      return;
    if (selected == -1)
      {
	selected = m; 
	return;
      }
    try{
      gs.play(selected, m, gs.whoseTurn());
    }catch (Cassis::Engine::InvalidParameter)
      {}
    selected = -1;
  }


  static gboolean
  expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
  {
    CassisDisplay* g = (CassisDisplay*)data;

    cairo_t *cr;
    /* get a cairo_t */  

    cr = gdk_cairo_create (g->imWind->window);
    
    /* set a clip region for the expose event */
    cairo_rectangle (cr,
 		     event->area.x, event->area.y,
 		     event->area.width, event->area.height);
    cairo_clip (cr);
    
    //    cairo_translate(cr, g->imWind->allocation.x, g->imWind->allocation.y);

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

  void displayStatus(cairo_t* cr)
  {
    std::stringstream ss;
    ss<<"Player ";
    if (gs.whoseTurn() == Cassis::Engine::PLAYER1)
      {
	cairo_set_source(cr, colorp1);
	ss<<'1';
      }
    else
      {
	cairo_set_source(cr, colorp2);
	ss<<'2';
      }

    ss<<"'s turn";
    
    cairo_move_to(cr, 30, 30);
    scaled_show_text(cr, ss.str());
  }

  Cassis::Engine::Vertex matchVertex(int x, int y)
  {
    int xcand, ycand;
    Cassis::Engine::Vertex ret = -1;

    std::cout<<"vertexradius is "<<vertexradius<<'\n';

    for (Cassis::Engine::Vertex v = 0; v < gs.nbVertex(); ++v)
      {
	centerOfVertex(v, xcand, ycand);
	double distance_squared = (x-xcand)*(x-xcand) + (y-ycand)*(y-ycand);

	std::cout<<"distance to "<<v<<" is "<<distance_squared<<'\n';
	
	if (distance_squared <= vertexradius * vertexradius)
	  ret = v;
      }

    std::cout<<std::flush;

    return ret;
  }

  void centerOfVertex(Cassis::Engine::Vertex v, int& x, int& y)
  {
    double angle_in_degree = v*360.0/gs.nbVertex();
    double angle_in_radian = angle_in_degree * (M_PI / 180.);

    x = gamecenterx + gameradius * cos(angle_in_radian);
    y = gamecentery + gameradius * sin(angle_in_radian);
  }

  void printVertices(cairo_t* cr)
  {
    cairo_set_source(cr, fgcolor);

    for (Cassis::Engine::Vertex v = 0; v < gs.nbVertex(); ++v)
      {
	int x;
	int y;
	centerOfVertex (v, x, y);
	cairo_arc (cr, x,y , vertexradius, 0., 2 * M_PI);	
	cairo_stroke (cr);
      }
  }

  void printEdges(cairo_t* cr)
  {
    for (Cassis::Engine::Vertex u = 0; u < gs.nbVertex(); ++u)
      for (Cassis::Engine::Vertex v = u+1; v < gs.nbVertex(); ++v)
      {
	Cassis::Engine::Color c = gs.edge(u,v);
	if (c == Cassis::Engine::UNCOLORED) continue;
	if (c == Cassis::Engine::PLAYER1) cairo_set_source(cr, colorp1);
	if (c == Cassis::Engine::PLAYER2) cairo_set_source(cr, colorp2);
	
	  
	int x_u;
	int y_u;
	centerOfVertex (u, x_u, y_u);
	
	int x_v;
	int y_v;
	centerOfVertex (v, x_v, y_v);

	cairo_move_to(cr, x_u, y_u);
	cairo_line_to(cr, x_v, y_v);
	
	cairo_stroke(cr);	
      }
  }


  void render(cairo_t* cr)
  { 
    //paint background
    cairo_set_source(cr, bgcolor);
    cairo_paint(cr);

    cairo_set_source(cr, fgcolor);

    printEdges(cr);
    printVertices(cr);
    
    displayStatus(cr);
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

  CassisDisplay g;

  g_signal_connect (G_OBJECT (window), "key-press-event",
		    G_CALLBACK (CassisDisplay::key_press), &g);
  
  g_signal_connect (G_OBJECT (window), "button-press-event",
		    G_CALLBACK (CassisDisplay::button_press), &g);
  
  
  
  GTK_WIDGET_SET_FLAGS(window, GTK_CAN_FOCUS);
  gtk_widget_add_events (window, GDK_KEY_PRESS_MASK);
  gtk_widget_add_events (window, GDK_BUTTON_PRESS_MASK);



  g.show();

  gtk_container_add(GTK_CONTAINER(window), g.getWidget());
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}


#ifndef CAIRO_GRAPHIC_CONTROLLER_H
#define CAIRO_GRAPHIC_CONTROLLER_H

class CairoGraphicController
{
protected:
  //window size
  int sizeX, sizeY;


  ///maps back to user distance before stoking.
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

#ifndef NOSTRING
  ///maps to user distance before displaying text
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
#endif

  ///maps to user distance before displaying text
  void scaled_show_text (cairo_t *cr, char* s)
  {
    double xwise = 1;
    double ywise = 1;
    cairo_save(cr);
    cairo_device_to_user_distance (cr, &xwise, &ywise);
   
    cairo_scale(cr,xwise,ywise);
    cairo_show_text (cr, s );
    cairo_restore(cr);
  }


public:

  void setSizeX(int sx){sizeX=sx;}
  void setSizeY(int sy){sizeY=sy;}

  int getSizeX() const {return sizeX;}
  int getSizeY() const {return sizeY;}
  
  virtual void clickat(int x, int y) = 0;
  

  ///cairo is scaled to (sizeX,sizeY) before render is called and area is cliped.
  virtual void render(cairo_t* cr) = 0;
  virtual ~CairoGraphicController(){};

  ///these functions are used to integrate in GTK
#ifdef GTK
  static gboolean
  key_press (GtkWidget *widget, GdkEventKey *event, gpointer data)
  {
    CairoGraphicController* g = (CairoGraphicController*)data;

    std::cout << event->hardware_keycode<<std::endl;

    gtk_widget_queue_draw (imWind);

    return TRUE;

  }

  static gboolean
  button_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
  {
    CairoGraphicController* g = (CairoGraphicController*)data;

    g->clickat(event->x, event->y);
    
    gtk_widget_grab_focus(widget);
    gtk_widget_queue_draw (imWind);

    return TRUE;
  }


  static gboolean
  expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
  {
    CairoGraphicController* g = (CairoGraphicController*)data;

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
#endif

};

#endif

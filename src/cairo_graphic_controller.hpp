#ifndef CAIRO_GRAPHIC_CONTROLLER_H
#define CAIRO_GRAPHIC_CONTROLLER_H

class CairoGraphicController
{
protected:
  //window size
  int sizeX, sizeY;

  ///blit a src to cr on the rectangle (x,y,x+width,y+height)
  void blit (cairo_t* cr, cairo_surface_t* src,
	     double x, double y, 
	     double width, double height)
  {
      cairo_save(cr);

      cairo_rectangle(cr,x,y,width,height);
      cairo_clip(cr);

      cairo_translate (cr,x,y);

      cairo_rectangle (cr,0,0,width,height);

      cairo_scale(cr,
		  width/cairo_image_surface_get_width(src),
		  height/cairo_image_surface_get_height(src));

      cairo_set_source_surface(cr, src, 0, 0);
      cairo_fill(cr);

      cairo_restore(cr);
  }


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

  virtual void setSizeX(int sx){sizeX=sx;}
  virtual void setSizeY(int sy){sizeY=sy;}

  virtual int getSizeX() const {return sizeX;}
  virtual int getSizeY() const {return sizeY;}
  
  virtual void clickat(int x, int y) = 0;
  

  ///cairo is scaled to (sizeX,sizeY) before render is called and area is cliped.
  virtual void render(cairo_t* cr) = 0;
  virtual ~CairoGraphicController(){};
  CairoGraphicController(){sizeX=sizeY=0;}

  virtual bool quit() const = 0;

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

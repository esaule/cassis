#ifndef CAIRO_SELECTOR_H
#define CAIRO_SELECTOR_H_

#include "cassis_cairo.hpp"

class CairoMenuSelector: public CairoGraphicController
{
  typedef const char * tagtype;

  cairo_pattern_t * bgcolor;
  cairo_pattern_t * fgcolor;
  cairo_pattern_t * optionbgcolor;


  CairoGraphicController* current;

  int nboption;
  float interoption_height; //fraction of sizeY
  float optionheight; //fraction of sizeY
  float optionwidth; //fraction of sizeX
  float optionOffsetX;
  float optionOffsetY;
  tagtype* tags;

  bool checkcurrent()
  {
    if (! current) return false;
    //assert (current);
    if (current->quit())
      {
	delete current;
	current = NULL;
      }

    return (current != NULL);
  }
public:

  CairoMenuSelector()
    :current(NULL)
  {
    nboption = 3;
    interoption_height = .05;
    optionheight = .1;
    optionwidth = .4;

    optionOffsetY = .1;
    optionOffsetX = .1;

    bgcolor = cairo_pattern_create_rgb(1,1,1);
    fgcolor = cairo_pattern_create_rgb(0,0,0);
    optionbgcolor = cairo_pattern_create_rgb(0,1,0);

    tags = new tagtype[nboption];
    tags[0] = "Dumb";
    tags[1] = "Easy";
    tags[2] = "Medium";
  }

  virtual void setSizeX(int sx)
  {
    CairoGraphicController::setSizeX(sx);
    if (checkcurrent()) {current->setSizeX(sx);}
  }

  virtual void setSizeY(int sy)
  {
    CairoGraphicController::setSizeY(sy);
    if (checkcurrent()) {current->setSizeY(sy);}
  }

  void clickon(int opt)
  {
    current = new CassisDisplay(opt);
  }

  virtual void clickat(int x, int y)
  {
    if (checkcurrent()) {current -> clickat (x,y); return;}

    for (int i=0; i<nboption; ++i)
      {
	int xlow = getSizeX()*optionOffsetX;
	int xhigh = getSizeX()*(optionwidth+optionOffsetX);
	int ylow = getSizeY()*(i*(optionheight+interoption_height)+optionOffsetY);
	int yhigh = getSizeY()*((i+1)*optionheight+i*interoption_height+optionOffsetY);

	if (x >= xlow && x <= xhigh
	    && y >= ylow && y <= yhigh) 
	  clickon(i);
      }    
  }

  virtual void render(cairo_t* cr)
  {
    if (checkcurrent()) {current -> render (cr); return;}


    cairo_set_source(cr, bgcolor);
    cairo_paint(cr);

    cairo_set_source(cr, fgcolor);

    cairo_save(cr);

    for (int i=0; i<nboption; ++i)
      {
	cairo_rectangle (cr,
			 getSizeX()*optionOffsetX, getSizeY()*(optionOffsetY+i*(optionheight+interoption_height)),
			 getSizeX()*optionwidth, getSizeY()*optionheight);

	//inside
	cairo_set_source (cr, optionbgcolor);
	cairo_fill_preserve(cr);

	//contour
	cairo_set_source (cr, fgcolor);
	cairo_stroke(cr);

	//print tag
	cairo_move_to(cr,
		      getSizeX()*(optionOffsetX+optionwidth/4.),
		      getSizeY()*(optionOffsetY+i*(optionheight+interoption_height)+optionheight/2.));
	scaled_show_text(cr, tags[i]);
      }

    cairo_restore(cr);
  }

  virtual ~CairoMenuSelector()
  {
    if (current) delete current;
    current = NULL;
    
    if (tags) delete[] tags;
    tags = NULL;

    cairo_pattern_destroy(bgcolor);
    cairo_pattern_destroy(fgcolor);
    cairo_pattern_destroy(optionbgcolor);
  }

  virtual bool quit() const {return false;}
};

#endif

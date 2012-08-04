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

#ifndef CAIRO_SELECTOR_H
#define CAIRO_SELECTOR_H_

#include "cassis_cairo.hpp"
#include "cairo_text_display.hpp"
#include "exception.hpp"

#define COPYRIGHTTEXT "(c) Erik Saule, 2012. " VERSION_NUMBER ". Released under the GPLv3 license."

class CairoMenuSelector: public CairoGraphicController
{
  typedef const char * tagtype;

  typedef enum
    {
      NONE,
      GAME,
      HELP
    } savetypes;

  cairo_pattern_t * bgcolor;
  cairo_pattern_t * fgcolor;
  cairo_pattern_t * optionbgcolor;

  const char* helptext;

  const char* copyrighttext;

  CairoGraphicController* current;

  int nboption;
  float interoption_height; //fraction of sizeY
  float optionheight; //fraction of sizeY
  float optionwidth; //fraction of sizeX
  float optionOffsetX;
  float optionOffsetY;
  float optionFontSize;
  float optionFontSizeRel;
  float titleFontSizeRel;
  float titleFontSize;
  tagtype* tags;

  float copyrightOffsetX;
  float copyrightOffsetY;
  float copyrightFontSizeRel;
  float copyrightFontSize;

  savetypes currentobj;
  
  bool checkcurrent()
  {
    if (! current) return false;
    //assert (current);
    if (current->quit())
      {
	delete current;
	current = NULL;
	currentobj = NONE;
      }

    return (current != NULL);
  }
public:
  CairoMenuSelector()
    :current(NULL)
  {
    nboption = 5;
    interoption_height = .05;
    optionheight = .1;
    optionwidth = .7;

    currentobj = NONE;

    optionOffsetY = .30;
    optionOffsetX = .1;
    optionFontSizeRel = .05;
    
    optionFontSize = optionFontSizeRel*getSizeY();

    titleFontSizeRel = .20;
    titleFontSize = titleFontSizeRel*getSizeY();

    copyrightOffsetX = .05;
    copyrightOffsetY = .9;
    copyrightFontSizeRel = .015;
    copyrightFontSize = copyrightFontSizeRel*getSizeY();


    bgcolor = cairo_pattern_create_rgb(1,1,1);
    fgcolor = cairo_pattern_create_rgb(0,0,0);
    optionbgcolor = cairo_pattern_create_rgb(0,1,0);

    tags = new tagtype[nboption];
    tags[0] = "Dumb";
    tags[1] = "Easy";
    tags[2] = "Medium";
    tags[3] = "Impossible";
    tags[4] = "Help";

    helptext = "Each player at her turn adds an edge between two points. The first player making a triangle LOSES. Do not connect 3 points together with 3 edges and you will win!";
    copyrighttext = COPYRIGHTTEXT;
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
    optionFontSize = optionFontSizeRel*getSizeY();
    titleFontSize = titleFontSizeRel*getSizeY();
    copyrightFontSize = copyrightFontSizeRel*getSizeY();
  }

  void clickon(int opt)
  {
    if (opt < 4)
      {
	currentobj = GAME;
	current = new CassisDisplay(opt);
      }

    if (opt == 4)
      {
	currentobj = HELP;
	current = new CairoTextDisplay(helptext);
      }

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


    cairo_surface_t* bgsprite = SpriteCollection::sc.getSprite("wood3.png");
    //cairo_surface_t* bgsprite = NULL;
    if (bgsprite != NULL)
      {
	blit(cr,bgsprite,0,0, getSizeX(), getSizeY());
      }

    cairo_set_source(cr, fgcolor);

    {
      cairo_move_to(cr,
		    getSizeX()*(optionOffsetX),
		    getSizeY()*(optionOffsetY+titleFontSizeRel)/2.);
      show_text(cr, "Cassis", titleFontSize);
    }
    
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
		      getSizeY()*(optionOffsetY+i*(optionheight+interoption_height)
				  +(optionheight+optionFontSizeRel)/2.));
	show_text(cr, tags[i], optionFontSize);
      }

    cairo_restore(cr);

    cairo_move_to (cr, getSizeX()*copyrightOffsetX, getSizeY()*copyrightOffsetY);
    show_text(cr, copyrighttext, copyrightFontSize);
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

  virtual void deserialize(const char* c)
  {
    currentobj = *((savetypes*) c);
    c += sizeof(currentobj);

    switch (currentobj)
      {
      case NONE:
	break;
      case HELP:
	current = new CairoTextDisplay(helptext);
	current->deserialize(c);
	break;
      case GAME:
	current = new CassisDisplay(0);
	current->deserialize(c);
	break;
      default:
	throw DeserializeException();
      }
  }

  virtual void serialize(char* c) const
  {
    *((savetypes*) c) = currentobj;
    c += sizeof(currentobj);
    
    if (current != NULL)
      current->serialize(c);
  }
  
  virtual size_t serializesize() const
  {
    size_t size = sizeof(currentobj);
    if (current != NULL)
      size += current->serializesize();

    return size;
  }

};

#endif

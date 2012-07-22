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

#ifndef CAIRO_TEXT_DISPLAY_H
#define CAIRO_TEXT_DISPLAY_H

#include "cairo_graphic_controller.hpp"
#include <string.h>

class CairoTextDisplay : public CairoGraphicController
{
  bool quitting;
  float textFontSize;
  float textFontSizeRel;
  float textOffsetX;
  float textOffsetY;
  char* text;
  int sizetext;

  float width_to_height_ratio;

  bool need_to_cut;

  cairo_pattern_t * bgcolor;
  cairo_pattern_t * fgcolor;

public:
  CairoTextDisplay(const char* te)
  {
    sizetext = strlen(te);
    text = new char[sizetext+1];
    memcpy(text, te, sizeof(char)*(sizetext+1));
    quitting = false;

    textFontSizeRel = .07;
    textFontSize = textFontSizeRel*getSizeY();
    width_to_height_ratio = .7;


    textOffsetX = .05;
    textOffsetY = .05;

    need_to_cut = true;
    
    bgcolor = cairo_pattern_create_rgb(1,1,1);
    fgcolor = cairo_pattern_create_rgb(0,0,0);
  }

  virtual void clickat(int x, int y)
  {
    quitting = true;
  }

  virtual bool quit() const
  {
    return quitting;
  }


  virtual void setSizeY(int sy)
  {
    CairoGraphicController::setSizeY(sy);
    textFontSize = textFontSizeRel*getSizeY();
    
    need_to_cut = true;
  }

  virtual void setSizeX(int sx)
  {
    CairoGraphicController::setSizeX(sx);

    need_to_cut = true;
  }


  void resetCut()
  {
    //change all '\0' to ' '
    for (int i=0; i<sizetext; ++i)
      if (text[i] == '\0')
	text[i] = ' ';
  }

  void reCutAt(int maxsize)
  {
    resetCut();
    

    char* t = text;
    t += maxsize;

    while (t < text+sizetext)
      {
	while (*t != ' ' && *t != '\0')
	  --t;
	*t = '\0';
	t += maxsize;
      }
  }


  virtual void render(cairo_t* cr)
  {
    if (need_to_cut)
      {
	reCutAt((1-2.*textOffsetX)/(textFontSizeRel*width_to_height_ratio));
	need_to_cut = false;
      }

    char* t = text;
    
    cairo_surface_t* bgsprite = SpriteCollection::sc.getSprite("wood3.png");
    //cairo_surface_t* bgsprite = NULL;
    if (bgsprite != NULL)
      {
	blit(cr,bgsprite,0,0, getSizeX(), getSizeY());
      }
    else
      {
	cairo_set_source(cr, bgcolor);
	cairo_paint(cr);
      }
    
    cairo_set_source(cr, fgcolor);
    
    int i=1;
    while (t < text+sizetext)
      {
	cairo_move_to(cr,
		      getSizeX()*(textOffsetX),
		      getSizeY()*(textOffsetY+i*textFontSizeRel));

	show_text(cr, t, textFontSize);

	t += strlen(t) + 1;
	++i;
      }
    
//     cairo_rectangle (cr,
// 		     getSizeX()*(textOffsetX), getSizeY()*(textOffsetY+textFontSizeRel), textFontSize, textFontSize
// 		     );
//     cairo_stroke(cr);

//     cairo_rectangle (cr,
// 		     getSizeX()*(textOffsetX), getSizeY()*(textOffsetY+textFontSizeRel), 
// 		     getSizeX()*(1-2.*textOffsetX)/(textFontSizeRel*width_to_height_ratio)
// , textFontSize
// 		     );
//     cairo_stroke(cr);

  }
  
  //there is nothing to serialize.
  virtual void deserialize(const char* c)
  {
  }

  virtual void serialize(char* c) const
  {
  }

  virtual size_t serializesize() const
  {
    return 0;
  }

  virtual ~CairoTextDisplay()
  {
    delete[] text;
  }
};

#endif

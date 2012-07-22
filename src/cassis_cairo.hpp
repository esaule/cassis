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

#ifndef CASSIS_CAIRO
#define CASSIS_CAIRO

#include "GameState.hpp"
#include "IA.hpp"
#include "cairo_graphic_controller.hpp"
#include "SpriteCollection_cairo.hpp"

class CassisDisplay : public CairoGraphicController
{
private:
  //colors
  cairo_pattern_t * bgcolor;
  cairo_pattern_t * fgcolor;

  cairo_pattern_t * colorp1;
  cairo_pattern_t * colorp2;

  Cassis::Engine::GameState gs;
  Cassis::IA::IA* ia;

  int gamecenterx;
  int gamecentery;
  int gameradius;
  int vertexradius;
  int edgewidth;
  int edgewidth_highlight;
  int vertex_highlight;

  int difficulty;

  Cassis::Engine::Vertex selected;

  bool quitting;


  void displayStatus(cairo_t* cr)
  {
    Cassis::Engine::Vertex x,y,z;
    Cassis::Engine::Color win = gs.winner();
    if (win == Cassis::Engine::UNCOLORED)
      {

	cairo_move_to(cr, 30, 30);

	if (gs.whoseTurn() == Cassis::Engine::PLAYER1)
	  {
	    cairo_set_source(cr, colorp1);
	    scaled_show_text(cr, "Player 1's turn");
	  }
	else
	  {
	    cairo_set_source(cr, colorp2);
	    scaled_show_text(cr, "Player 1's turn");
	  }	
      }
    else
      {
	cairo_set_source(cr, fgcolor);
	cairo_move_to(cr, 30, 30);

	if (gs.whoseTurn() == Cassis::Engine::PLAYER1)
	  {
	    scaled_show_text(cr, "Player 1 won!");
	  }
	else
	  {
	    scaled_show_text(cr, "Player 2 won!");
	  }
      }
  }

  Cassis::Engine::Vertex matchVertex(int x, int y)
  {
    int xcand, ycand;
    Cassis::Engine::Vertex ret = -1;

    for (Cassis::Engine::Vertex v = 0; v < gs.nbVertex(); ++v)
      {
	centerOfVertex(v, xcand, ycand);
	double distance_squared = (x-xcand)*(x-xcand) + (y-ycand)*(y-ycand);

	if (distance_squared <= vertexradius * vertexradius)
	  ret = v;
      }

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
    cairo_save(cr);

    for (Cassis::Engine::Vertex v = 0; v < gs.nbVertex(); ++v)
      {
	int x;
	int y;
	cairo_save(cr);

	centerOfVertex (v, x, y);
	cairo_arc (cr, x,y , vertexradius, 0., 2 * M_PI);	

	cairo_set_line_width(cr, vertex_highlight);
	
	if (v == selected)
	  {
	    cairo_fill_preserve(cr);
	
	    switch (gs.whoseTurn())
	      {
	      case Cassis::Engine::PLAYER1:
		cairo_set_source(cr, colorp1);
		break;
	      case Cassis::Engine::PLAYER2:
		cairo_set_source(cr, colorp2);
		break;
	      }
	    cairo_stroke (cr);
	  }
	else
	  cairo_fill(cr);

	cairo_restore(cr);
      }

    cairo_restore(cr);
  }

  void printEdges(cairo_t* cr)
  {
    Cassis::Engine::Vertex x,y,z;
    Cassis::Engine::Color c = gs.winner(x,y,z);
    
    cairo_save(cr);
    
    for (Cassis::Engine::Vertex u = 0; u < gs.nbVertex(); ++u)
      for (Cassis::Engine::Vertex v = u+1; v < gs.nbVertex(); ++v)
      {
	Cassis::Engine::Color c = gs.edge(u,v);
	if (c == Cassis::Engine::UNCOLORED) continue;
	if (c == Cassis::Engine::PLAYER1) cairo_set_source(cr, colorp1);
	if (c == Cassis::Engine::PLAYER2) cairo_set_source(cr, colorp2);
	
	cairo_set_line_width(cr, edgewidth);

	if (c != Cassis::Engine::UNCOLORED)
	  {
	    if ((u == x && v == y) ||
		(u == x && v == z) ||
		(u == y && v == z))
	      cairo_set_line_width(cr, edgewidth_highlight);
	  }
	  
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

    cairo_restore(cr);
  }

  ///computes the various size and offset based on the size of the windows.
  ///should be called at least before each frame where the size changed.
  void setSizes()
  {
    if(getSizeX() == 0 || getSizeY() == 0) return;

    gamecenterx = getSizeX()/2;
    gamecentery = getSizeY()/2;

    int min_dim = (getSizeX()<getSizeY()?getSizeX():getSizeY());
    gameradius = ((float)min_dim) * .39;
    vertexradius = ((float)min_dim) * .09;
  }
  
  void setDifficulty(int difficulty)
  {
    this->difficulty = difficulty;
    if (ia != NULL)
      {
	delete ia;
      }

    switch(difficulty)
      {
      case 0:
	ia = new Cassis::IA::DumbIA();
	break;
      case 1:
	ia = new Cassis::IA::SimpleIA();
	break;
      case 2:
	ia = new Cassis::IA::RandomIA();
	break;
      case 3:
	ia = new Cassis::IA::RandomIA();
	break;
      default:
	ia = new Cassis::IA::RandomIA();
	break;
      }
  }

public:

  Cassis::Engine::GameState& getState(){return gs;}

  virtual void setSizeX(int sx)
  {
    CairoGraphicController::setSizeX(sx);
    setSizes();
  }

  virtual void setSizeY(int sy)
  {
    CairoGraphicController::setSizeY(sy);
    setSizes();
  }

  CassisDisplay(int difficulty)
    :ia(NULL)
  {
    setDifficulty(difficulty);

    quitting = false;

    selected = -1;
    gamecenterx = gamecentery = 250;
    gameradius = 150;
    vertexradius = 50;
      
    bgcolor = cairo_pattern_create_rgb(1,1,1);
    fgcolor = cairo_pattern_create_rgb(0,0,0);

    colorp1 = cairo_pattern_create_rgb(1,0,0);
    colorp2 = cairo_pattern_create_rgb(0,0,1);

    edgewidth=7;
    edgewidth_highlight=12;
    vertex_highlight=7;
  }

  virtual ~CassisDisplay()
  {
    cairo_pattern_destroy(bgcolor);
    cairo_pattern_destroy(fgcolor);

    cairo_pattern_destroy(colorp1);   
    cairo_pattern_destroy(colorp2);
    delete ia;
  }

  void clickat(int x, int y)
  {
    int m = matchVertex(x, y);

    if (gs.gameOver())
      {
	quitting = true;
	return;
      }

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

    if ( ! gs.gameOver())
      {
	while (gs.whoseTurn() != Cassis::Engine::PLAYER1)
	  {
	    //std::cout<<"IA's turn"<<std::endl;
	    try
	      {
		ia->play(gs);
	      }
	    catch (Cassis::Engine::InvalidParameter)
	      {
		//		std::cout<<"IA pooped itself"<<std::endl;
	      }	
	  }
      }

    selected = -1;
  }



  virtual void render(cairo_t* cr)
  { 
    //paint background
    cairo_set_source(cr, bgcolor);
    cairo_paint(cr);

    cairo_surface_t* bgsprite = SpriteCollection::sc.getSprite("wood3.png");
    //cairo_surface_t* bgsprite = NULL;
    if (bgsprite != NULL)
      {
	blit(cr,bgsprite,0,0, getSizeX(), getSizeY());
      }

    cairo_set_source(cr, fgcolor);

    printEdges(cr);
    printVertices(cr);
    
    displayStatus(cr);
  }


  virtual bool quit() const{return quitting;}

  virtual void deserialize(const char* c) 
  {
    int diff = *((int*)c);
    c += sizeof(diff);
    setDifficulty(diff);

    gs.deserialize(c);
  }

  virtual void serialize(char* c) const
  {
    *((int*)c) = difficulty;
    c += sizeof(difficulty);

    gs.serialize(c);
  }
  
  virtual size_t serializesize() const
  {
    return sizeof(difficulty) + gs.serializesize();
  }
};

#endif

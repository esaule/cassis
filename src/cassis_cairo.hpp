#ifndef CASSIS_CAIRO
#define CASSIS_CAIRO

#include "GameState.hpp"
#include "IA.hpp"
#include "cairo_graphic_controller.hpp"

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

  Cassis::Engine::Vertex selected;



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
    cairo_set_source(cr, fgcolor);

    for (Cassis::Engine::Vertex v = 0; v < gs.nbVertex(); ++v)
      {
	int x;
	int y;
	centerOfVertex (v, x, y);
	cairo_arc (cr, x,y , vertexradius, 0., 2 * M_PI);	
	    cairo_save(cr);
	    if (v == selected)
	      {
		switch (gs.whoseTurn())
		  {
		  case Cassis::Engine::PLAYER1:
		    cairo_set_source(cr, colorp1);
		    break;
		  case Cassis::Engine::PLAYER2:
		    cairo_set_source(cr, colorp2);
		    break;
		  }
	      }
	    else
	      {
		cairo_set_source(cr, bgcolor);
	      }
	    cairo_fill_preserve(cr);
	    cairo_restore(cr);

	cairo_stroke (cr);
      }
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
	
	cairo_set_line_width(cr, 3);

	if (c != Cassis::Engine::UNCOLORED)
	  {
	    if ((u == x && v == y) ||
		(u == x && v == z) ||
		(u == y && v == z))
	      cairo_set_line_width(cr, 7);
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


public:

  Cassis::Engine::GameState& getState(){return gs;}

  CassisDisplay()
  {
    ia = new Cassis::IA::RandomIA();
    selected = -1;
    gamecenterx = gamecentery = 250;
    gameradius = 150;
    vertexradius = 50;
      
    bgcolor = cairo_pattern_create_rgb(1,1,1);
    fgcolor = cairo_pattern_create_rgb(0,0,0);

    colorp1 = cairo_pattern_create_rgb(1,0,0);
    colorp2 = cairo_pattern_create_rgb(0,0,1);
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
      return;

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

    cairo_set_source(cr, fgcolor);

    printEdges(cr);
    printVertices(cr);
    
    displayStatus(cr);
  }

};

#endif

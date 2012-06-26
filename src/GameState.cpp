#include "GameState.hpp"

#ifndef NULL
#define NULL 0
#endif

namespace Cassis{
  namespace Engine{

    void GameState::validateVertex(Vertex i) const throw(InvalidParameter)
    {
      if (i < 0) throw InvalidParameter();
      if (i >= nbVertex()) throw InvalidParameter();
    }

    Vertex GameState::nbVertex() const
    {
      return 6;
    }
    
    Edge GameState::nbEdge() const
    {
      return 20;
      
      // int ret=0;
      // for (int i=1; i <= nbVertex(); ++i)
      //   {
      //     ret += i;
      //   }
      // return ret;
    }
    
    GameState::GameState()
      :board(new Color[nbEdge()]), turn(0)
    {
      for (Edge e = 0; e< nbEdge(); ++e)
	{
	  board[e] = UNCOLORED;
	}
    }
    
    GameState::~GameState()
    {
      delete[] board;
      board = NULL;
    }
    
    //returns the offset of neighbors of i in a board. so \sum_{j=0}^i 6-j
    static Edge base(Vertex i)
    {
      switch (i)
	{
	case 0:
	  return 0;
	case 1:
	  return 6;
	case 2:
	  return 11;
	case 3:
	  return 15;
	case 4:
	  return 18;
	case 5:
	  return 20;
	}
    }
    
    Color& GameState::edgeLoc(Vertex i, Vertex j)
    {
      return board[base(i)+(j-i-1)];
    }
    
    const Color& GameState::edgeLoc(Vertex i, Vertex j) const
    {
      return board[base(i)+(j-i-1)];
    }
    
    Color GameState::edge(Vertex i, Vertex j) const throw (InvalidParameter)
    {
      if (i == j)
	throw InvalidParameter();
      validateVertex(i);
      validateVertex(j);
      if (j < i)
	return edge(j, i);
      
      //assert (i < j);
      
      return edgeLoc(i,j);
    }
    
    Color GameState::whoseTurn() const
    {
      switch (turn % 2)
	{
	case 0: return PLAYER1;
	case 1: return PLAYER2;
	}
    }
    
    Color GameState::winner(Vertex& x, Vertex& y, Vertex& z) const
    {
      for (x = 0 ; x < nbVertex(); x++)
	for (y = x+1 ; y < nbVertex(); y++)
	  for (z = y+1 ; z < nbVertex(); z++)
	    {
	      if (edge(x,y) != UNCOLORED
		  && edge(x,y) == edge(x,z)
		  && edge(x,y) == edge(y,z))
		{
		  if (edge(x,y) == PLAYER1)
		    return PLAYER2;
		  else
		    return PLAYER1;
		}
	    }
      return UNCOLORED;
    }

    Color GameState::winner() const
    {
      Vertex x,y,z;

      return winner(x,y,z);
    }


    bool GameState::gameOver() const
    {
      return winner() != UNCOLORED;
    }
    
    void GameState::play (Vertex i, Vertex j, Color c) throw (InvalidParameter)
    {
      if (i == j)
	throw InvalidParameter();
      validateVertex(i);
      validateVertex(j);
      if (j < i)
	{
	  play (j,i,c);
	  return;
	}
      
      //assert (i<j);
      if (edgeLoc(i,j) != UNCOLORED) throw InvalidParameter();
      if (whoseTurn() != c) throw InvalidParameter();
      
      edgeLoc(i,j) = c;
      turn++;
    }
    
  }
}

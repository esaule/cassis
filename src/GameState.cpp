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

#include "GameState.hpp"

#ifndef NULL
#define NULL 0
#endif

#include <sstream>
#include "GameState.hpp"

namespace Cassis{
  namespace Engine{
    

    void GameState::validateVertex(Vertex i) const throw(InvalidParameter)
    {
      if (i < 0) {
	std::stringstream ss;
	ss<<"invalid vertex: "<<i;
	throw InvalidParameter(ss.str());
      }
      if (i >= nbVertex()) {
	std::stringstream ss;
	ss<<"invalid vertex: "<<i;
	throw InvalidParameter(ss.str());
      }
    }

    Vertex GameState::nbVertex() const
    {
      return 6;
    }
    
    Edge GameState::nbEdge() const
    {
      return 15;
      
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

    GameState::GameState(const GameState& gs)
      :board(new Color[nbEdge()]), turn(gs.turn)
    {
      for (Edge e = 0; e< nbEdge(); ++e)
	{
	  board[e] = gs.board[e];
	}
    }

    
    GameState::~GameState()
    {
      delete[] board;
      board = NULL;
    }
    
    //returns the offset of neighbors of i in a board. so \sum_{j=0}^i 5-j
    static Edge base(Vertex i)
    {
      switch (i)
	{
	case 0:
	  return 0;
	case 1:
	  return 5;
	case 2:
	  return 9;
	case 3:
	  return 12;
	case 4:
	  return 14;
	case 5:
	  return 15;
	}
      throw BaseException();
      return 0;
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
	{
	  std::stringstream ss;
	  ss<<"invalid edge: "<<i<<" "<<j;
	  throw InvalidParameter(ss.str());
	}
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
      throw BaseException();
      return UNCOLORED; //never reaches here.
    }

    Edge GameState::whichTurn() const
    {
      return turn;
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
	{
	  std::stringstream ss;
	  ss<<"can t play. invalid edge "<<i<<" "<<j;
	  throw InvalidParameter(ss.str());
	}
      validateVertex(i);
      validateVertex(j);
      if (j < i)
	{
	  play (j,i,c);
	  return;
	}
      
      //assert (i<j);
      if (edgeLoc(i,j) != UNCOLORED)
	{
	  std::stringstream ss;
	  ss<<"can not play. The edge is not UNCOLORED";
	  throw InvalidParameter(ss.str());
	}
      if (whoseTurn() != c){
	std::stringstream ss;
	ss<<"can not play. it is not "<<c<<" turn";
	throw InvalidParameter(ss.str());
      }
      
      edgeLoc(i,j) = c;
      turn++;
    }
  }
}

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
#include <algorithm>

namespace Cassis{
  namespace Engine{
    template <typename T>
    struct Comp
    {
      T* deg1;
      T* deg2;
      Comp(T* d1, T* d2)
	:deg1(d1), deg2(d2)
      {}
      
      bool operator() (T u, T v) 
      {
	if (deg1[u]+deg2[u] < deg1[v]+deg2[v])
	  return true;
	if (deg1[u]+deg2[u] > deg1[v]+deg2[v])
	  return false;
	//assert (deg1[u]+deg2[u] == deg1[v]+deg2[v]);
	return deg1[u] < deg1[v];
      }
    };
    

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
	  ss<<"can not play. UNCOLORED";
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

    int GameState::serializesize() const
    {
      return sizeof(int) + nbEdge()*sizeof(Color);
    }
    
    void GameState::serialize(char* buffer) const
    {
      int* t = (int*) buffer;
      *t = turn;
      Color* c = (Color*) (buffer+sizeof(int));
      for (int i=0; i<nbEdge(); ++i)
	{
	  *c = board[i];
	  c++;
	}
    }

    void GameState::deserialize(const char* buffer)
    {
      int* t = (int*) buffer;
      turn = *t ;
      Color* c = (Color*) (buffer+sizeof(int));
      for (int i=0; i<nbEdge(); ++i)
	{
	  board[i] = *c;
	  c++;
	}
    }

    ///returns the state of the game encoded in base 3.
    GameState::HashType GameState::hash() const
    {
      Vertex * degree1 = new Vertex[nbVertex()];
      Vertex * degree2 = new Vertex[nbVertex()];
      for (Vertex u = 0; u< nbVertex(); ++u)
	{
	  degree1[u] = 0;
	  degree2[u] = 0;
	  for (Vertex v = 0; v< nbVertex(); ++v)
	    {
	      if (u == v) continue;
	      if (edge(u,v) != PLAYER1)
		++degree1[u];
	      if (edge(u,v) != PLAYER2)
		++degree2[u];
	    }
	}
      
      Vertex* perm = new Vertex[nbVertex()];
      for (Vertex u=0; u<nbVertex(); ++u)
	perm[u] = u;
            
      Comp<Vertex> comp(degree1, degree2);

      std::sort<Vertex*, Comp<Vertex> >((Vertex*)perm, perm+nbVertex(), comp);

      HashType h;
      for (Vertex i=0; i<nbVertex(); ++i)
	{
	  Vertex u = perm[i];
	  for (Vertex j=i+1; j<nbVertex(); ++j)
	  {
	    Vertex v = perm[j];
	    
	    h *= 3;
	    h += edge(u,v);
	  }
	}
      delete[] perm;
      delete[] degree1;
      delete[] degree2;

      return h;
    }

  }
}

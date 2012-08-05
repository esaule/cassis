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

#ifndef ANDROID
#include <sstream>
using std::stringstream;
#else
#include "mysstream.hpp"
#endif
#include "GameState.hpp"
#include <assert.h>

namespace Cassis{
  namespace Engine{
    

    void GameState::validateVertex(Vertex i) const throw(InvalidParameter)
    {
      if (i < 0) {
	stringstream ss;
	ss<<"invalid vertex: "<<i;
	throw InvalidParameter(ss.str());
      }
      if (i >= nbVertex()) {
	stringstream ss;
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
	  stringstream ss;
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
	  stringstream ss;
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
	  stringstream ss;
	  ss<<"can not play. The edge is not UNCOLORED";
	  throw InvalidParameter(ss.str());
	}
      if (whoseTurn() != c){
	stringstream ss;
	ss<<"can not play. it is not "<<c<<" turn";
	throw InvalidParameter(ss.str());
      }
      
      edgeLoc(i,j) = c;
      turn++;
    }

    void GameState::compute_degrees(Vertex* deg1, Vertex* deg2, Vertex* deg) const
    {
      for (Vertex u = 0; u< nbVertex(); ++u)
	{
	  deg1[u] = 0;
	  deg2[u] = 0;
	  deg[u] = 0;
	  for (Vertex v = 0; v< nbVertex(); ++v)
	    {
	      if (u == v) continue;
	      if (edge(u,v) != PLAYER1)
		++deg1[u];
	      if (edge(u,v) != PLAYER2)
		++deg2[u];
	      if (edge(u,v) != UNCOLORED)
		++deg[u];
	    }
	}

    }

    ///if gs is permuted by perm, is the graph this?
    bool GameState::isomorph_check(const GameState &gs, Vertex* perm) const
    {
      for (Vertex u=0; u<nbVertex(); ++u)
	{
	  Vertex u_in_gs = perm[u];
	  for (Vertex v=0;v<nbVertex(); ++v)
	    {
	      if (u == v) continue;
	      Vertex v_in_gs = perm[v];
	      if (edge(u_in_gs,v_in_gs) != gs.edge(u,v))
		return false;
	    }
	}
      return true;
    }

    bool GameState::isomorph(const GameState &gs, Vertex* perm) const
    {
      assert (nbVertex() == 6);

      Vertex* deg1this = new Vertex[nbVertex()];
      Vertex* deg2this = new Vertex[nbVertex()];
      Vertex* degthis = new Vertex[nbVertex()];

      Vertex* deg1gs = new Vertex[nbVertex()];
      Vertex* deg2gs = new Vertex[nbVertex()];
      Vertex* deggs = new Vertex[nbVertex()];

      this->compute_degrees(deg1this, deg2this, degthis);
      gs.compute_degrees(deg1gs, deg2gs, deggs);


      bool* used= new bool[nbVertex()];
      for (Vertex u=0; u<nbVertex();++u) used[u] = false;

      bool found = false;
      
      for (Vertex v0=0; v0<nbVertex() && !found; ++v0) {
	if (used[v0]) continue;
	if (deg1this[0] != deg1gs[v0]) continue;
	if (deg2this[0] != deg2gs[v0]) continue;
	if (degthis[0] != deggs[v0]) continue;
	perm[v0] = 0;
	used[v0] = true;
	for (Vertex v1=0; v1<nbVertex() && !found; ++v1) {
	  if (used[v1]) continue;
	  if (deg1this[1] != deg1gs[v1]) continue;
	  if (deg2this[1] != deg2gs[v1]) continue;
	  if (degthis[1] != deggs[v1]) continue;
	  perm[v1] = 1;
	  used[v1] = true;
	  for (Vertex v2=0; v2<nbVertex() && !found; ++v2) {
	    if (used[v2]) continue;
	    if (deg1this[2] != deg1gs[v2]) continue;
	    if (deg2this[2] != deg2gs[v2]) continue;
	    if (degthis[2] != deggs[v2]) continue;
	    perm[v2] = 2;
	    used[v2] = true;
	    for (Vertex v3=0; v3<nbVertex() && !found; ++v3) {
	      if (used[v3]) continue;
	      if (deg1this[3] != deg1gs[v3]) continue;
	      if (deg2this[3] != deg2gs[v3]) continue;
	      if (degthis[3] != deggs[v3]) continue;
	      perm[v3] = 3;
	      used[v3] = true;
	      for (Vertex v4=0; v4<nbVertex() && !found; ++v4) {
		if (used[v4]) continue;
		if (deg1this[4] != deg1gs[v4]) continue;
		if (deg2this[4] != deg2gs[v4]) continue;
		if (degthis[4] != deggs[v4]) continue;
		perm[v4] = 4;
		used[v4] = true;
		for (Vertex v5=0; v5<nbVertex() && !found; ++v5) {
		  if (used[v5]) continue;
		  if (deg1this[5] != deg1gs[v5]) continue;
		  if (deg2this[5] != deg2gs[v5]) continue;
		  if (degthis[5] != deggs[v5]) continue;
		  perm[v5] = 5;
		  used[v5] = true;

		  if (isomorph_check(gs, perm))
		    found = true;
		  used[v5] = false;
		}
		used[v4] = false;
	      }
	      used[v3] = false;
	    }
	    used[v2] = false;
	  }
	  used[v1] = false;
	}
	used[v0] = false;
      }


      delete[] used;
      delete[] deg1this;
      delete[] deg2this;
      delete[] degthis;

      delete[] deg1gs;
      delete[] deg2gs;
      delete[] deggs;

      return found;
    }
  }
}

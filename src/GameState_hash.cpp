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

//0 for simple hash (no permutation)
//1 for simple degree
//2 for dual degree
#define CASSIS_HASH_FUNCTION 2

#if CASSIS_HASH_FUNCTION == 0
    void GameState::normalize_inverse_permutation(Vertex * perminv) const
    {
      for (Vertex u=0; u<nbVertex(); ++u)
	perminv[u] = u;
    }

#endif

#if CASSIS_HASH_FUNCTION == 1
    template <typename T>
    struct Comp
    {
      T* deg;

      Comp(T* d1)
	:deg(d1)
      {}
      
      bool operator() (T u, T v) 
      {
	return deg[u] < deg[v];
      }
    };

    void GameState::normalize_permutation(Vertex * perminv) const
    {
      Vertex * degree = new Vertex[nbVertex()];
      
      
      for (Vertex u = 0; u< nbVertex(); ++u)
	{
	  degree[u] = 0;
	  for (Vertex v = 0; v< nbVertex(); ++v)
	    {
	      if (u == v) continue;
	      if (edge(u,v) != UNCOLORED)
		++degree[u];
	    }
	}
      
      for (Vertex u=0; u<nbVertex(); ++u)
	perminv[u] = u;
      
      Comp<Vertex> comp(degree);
      

      std::sort<Vertex*, Comp<Vertex> >((Vertex*)perminv, perminv+nbVertex(), comp); 

      delete[] perminv;
      delete[] degree;
    }
#endif


#if CASSIS_HASH_FUNCTION == 2
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

    void GameState::normalize_inverse_permutation(Vertex * perminv) const
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
      
      for (Vertex u=0; u<nbVertex(); ++u)
	perminv[u] = u;
      
      Comp<Vertex> comp(degree1, degree2);
      
      std::sort<Vertex*, Comp<Vertex> >((Vertex*)perminv, perminv+nbVertex(), comp); 

      delete[] degree1;
      delete[] degree2;
    }
#endif

    void GameState::normalize_permutation(Vertex * perm) const
    {
      Vertex * perminv = new Vertex[nbVertex()];

      normalize_inverse_permutation(perminv);

      for (Vertex u=0; u<nbVertex(); ++u)
	perm[perminv[u]]=u;
      
      delete[] perminv;
    }

    ///returns the state of the game encoded in base 3.
    GameState::HashType GameState::hash() const
    {
      Vertex * perminv = new Vertex[nbVertex()];

      normalize_inverse_permutation(perminv);

      HashType h = 0;
      for (Vertex i=0; i<nbVertex(); ++i)
	{
	  Vertex u = perminv[i];
	  for (Vertex j=i+1; j<nbVertex(); ++j)
	  {
	    Vertex v = perminv[j];
	    
	    h *= 3;
	    h += edge(u,v);
	  }
	}
      delete[] perminv;

      return h;
    }

    void GameState::unhash(HashType h)
    {
      for (int i=nbEdge()-1; i>=0; --i)
	{
	  switch (h%3)
	    {
	    case 0:
	      board[i] = UNCOLORED;
	      break;
	    case 1:
	      board[i] = PLAYER1;
	      break;
	    case 2:
	      board[i] = PLAYER2;
	      break;
	    }
	  
	  h /= 3;
	}
      
      turn = 0;
      for (int i=0; i<nbEdge(); ++i)
	if (board[i] != UNCOLORED)
	  ++turn;
    }
  }
}

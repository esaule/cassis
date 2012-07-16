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

#include "IA.hpp"
#include "stdlib.h"

Cassis::IA::IA::~IA()
{}

Cassis::IA::DumbIA::~DumbIA()
{}

void Cassis::IA::DumbIA::play(Engine::GameState& gs)
{
  //find the first free edge and take it
  for (Engine::Vertex u = 0; u < gs.nbVertex(); ++u)
    for (Engine::Vertex v = u+1; v < gs.nbVertex(); ++v)
      if (gs.edge(u,v) == Engine::UNCOLORED)
	{
	  gs.play(u, v, gs.whoseTurn());
	  return;
	}
}

Cassis::IA::RandomIA::~RandomIA()
{}

void Cassis::IA::RandomIA::play(Engine::GameState& gs)
{
  while (true)
    {
      int r = rand();
      Engine::Vertex u = r%8;
      r /= 8;
      Engine::Vertex v = r%8;

      //rely on the engine throwing an exception in case of invalid move
      try
	{
	  gs.play(u, v, gs.whoseTurn());
	  return;
	}
      catch(Engine::InvalidParameter)
	{}
    }
}


Cassis::IA::SimpleIA::~SimpleIA()
{}

void Cassis::IA::SimpleIA::play(Engine::GameState& gs)
{
  Engine::Color whoami = gs.whoseTurn(); 

  Engine::Vertex ufree;
  Engine::Vertex vfree;
  

  //find the first free edge and take it
  for (Engine::Vertex u = 0; u < gs.nbVertex(); ++u)
    for (Engine::Vertex v = u+1; v < gs.nbVertex(); ++v)
      if (gs.edge(u,v) == Engine::UNCOLORED)
	{
	  //(u, v) is a candidate
	  ufree = u;
	  vfree = v;
	  
	  //determine if (u,v) is ok to take
	  bool ok = true;
	  for (Engine::Vertex w = 0; w < gs.nbVertex(); ++w)
	    {
	      if (w == u) continue;
	      if (w == v) continue;

	      if (gs.edge (u,w) == whoami &&
		  gs.edge (v,w) == whoami)
		{
		  ok = false;
		  break;
		}
	    }

	  if (ok)
	    {
	      gs.play(u, v, whoami);
	      return;
	    }
	}

  //if nothing is found, pick a triangle and lose.
  gs.play(ufree, vfree, whoami);
}

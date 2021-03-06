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

#ifndef CASSIS_IA
#define CASSIS_IA

#include "GameState.hpp"
#ifndef NOMAP
#include <map>
#else
#include "mymap.hpp"
#endif

namespace Cassis
{
  namespace IA
  {
    class IA
    {
    public:
      virtual void play(Engine::GameState& gs) = 0;
      virtual ~IA();
    };

    ///DumbIA looks for the first edge that is free and take it.
    class DumbIA : public IA
    {
    public:
      virtual void play(Engine::GameState& gs);
      virtual ~DumbIA();
    };

    ///SimpleIA looks for the first edge that is free and that does
    ///not make a triangle and take it.
    class SimpleIA : public IA
    {
    public:
      virtual void play(Engine::GameState& gs);
      virtual ~SimpleIA();
    };

    ///RandomIA looks randomly for the an edge that is free and take it.
    class RandomIA : public IA
    {
    public:
      virtual void play(Engine::GameState& gs);
      virtual ~RandomIA();
    };

    ///BruteIA looks at all the combination and picks the best one
    class BruteIA : public IA
    {
#ifdef NOMAP
      mymap<Cassis::Engine::GameState::HashType, bool>* winning;
      mymap<Cassis::Engine::GameState::HashType,
	    std::pair<Cassis::Engine::Vertex,
		      Cassis::Engine::Vertex> >* move;
#else

      std::map<Cassis::Engine::GameState::HashType, bool>* winning;
      std::map<Cassis::Engine::GameState::HashType,
	       std::pair<Cassis::Engine::Vertex,
			 Cassis::Engine::Vertex> >* move;
#endif
      int NBROUND;
      
      
      Cassis::Engine::Vertex* perm;
      Cassis::Engine::Vertex* permminus1;

      //used to initialize winning and move
      bool p1win(Cassis::Engine::GameState& gs, int round);
      

    public:
      BruteIA();
      virtual void play(Engine::GameState& gs);
      virtual ~BruteIA();
    };
    
  }
}

#endif

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
      virtual void play(Engine::GameState& gs);
      virtual ~DumbIA();
    };

    ///SimpleIA looks for the first edge that is free and that does
    ///not make a triangle and take it.
    class SimpleIA : public IA
    {
      virtual void play(Engine::GameState& gs);
      virtual ~SimpleIA();
    };

    ///RandomIA looks randomly for the an edge that is free and take it.
    class RandomIA : public IA
    {
      virtual void play(Engine::GameState& gs);
      virtual ~RandomIA();
    };

  }
}

#endif

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

namespace Cassis{
  namespace Engine{

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
  }
}

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
#include <iostream>
#include <assert.h>

int main()
{
  Cassis::Engine::GameState gs;
  
  Cassis::Engine::Vertex x,y,z;
  while (gs.winner(x,y,z) == Cassis::Engine::UNCOLORED)
    {
      Cassis::Engine::Color pl = gs.whoseTurn();
      assert (pl != Cassis::Engine::UNCOLORED);
      if (pl == Cassis::Engine::PLAYER1)
	std::cout<<"Player 1's turn"<<std::endl;
      else 
	std::cout<<"Player 2's turn"<<std::endl;



      for (Cassis::Engine::Vertex i = 0; i < gs.nbVertex(); ++i)
	{
	  for (Cassis::Engine::Vertex j = 0; j < gs.nbVertex(); ++j)
	    {
	      if (i == j)
		std::cout<<' ';
	      else
		switch (gs.edge(i,j))
		  {
		  case Cassis::Engine::PLAYER1:
		    std::cout<<'1';break;
		  case Cassis::Engine::PLAYER2:
		    std::cout<<'2';break;
		  case Cassis::Engine::UNCOLORED:
		    std::cout<<' ';break;
		  }
	    }
	  std::cout<<'\n';
	}
      std::cout<<std::flush;

      Cassis::Engine::Vertex i, j;

      try
	{
	  std::cin>>i>>j;
	  gs.play(i,j,pl);
	}
      catch (Cassis::Engine::InvalidParameter)
	{
	  std::cout<<"invalid move"<<std::endl;
	}
    }

  if (gs.winner(x,y,z) == Cassis::Engine::PLAYER1)
    std::cout<<"Player 1 wins"<<std::endl;
  else
    std::cout<<"Player 2 wins"<<std::endl;

  std::cout<<"Because of triangle "<<x<<" "<<y<<" "<<z<<std::endl; 

  return 0;
}

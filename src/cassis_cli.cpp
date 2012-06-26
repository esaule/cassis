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

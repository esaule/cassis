#include "GameState.hpp"
#include <iostream>
#include <assert.h>

int main()
{
  Cassis::Engine::GameState gs;
  
  while (gs.winner() != Cassis::Engine::UNCOLORED)
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

  if (gs.winner() == Cassis::Engine::PLAYER1)
    std::cout<<"Player 1 wins"<<std::endl;
  else
    std::cout<<"Player 2 wins"<<std::endl;

  return 0;
}

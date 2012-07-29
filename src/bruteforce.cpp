#include <assert.h>
#include <iostream>
#include "GameState.hpp"
#include <stack>
#include <map>
#include "timestamp.hpp"

std::map<Cassis::Engine::GameState::HashType, bool> winning;
std::map<Cassis::Engine::GameState::HashType,
	 std::pair<Cassis::Engine::Vertex,
		   Cassis::Engine::Vertex> > move;

bool p1win(Cassis::Engine::GameState& gs)
{
  static long long int eval = 0;
  switch(gs.winner())
    {
    case Cassis::Engine::PLAYER1:
      return true;
      break;
    case Cassis::Engine::PLAYER2:
      return false;
      break;
    default:
      break;
    }

  auto it = winning.find(gs.hash());
  if (it != winning.end())
    return it->second;

  eval++;
  if (eval%10000 == 0)
    {
      std::cout<<eval<<std::endl;
    }

  Cassis::Engine::Color player = gs.whoseTurn();
  if (player == Cassis::Engine::PLAYER1)
    {
      //try all move
      for (Cassis::Engine::Vertex i = 0; i< gs.nbVertex(); ++i)
	for (Cassis::Engine::Vertex j = i+1; j< gs.nbVertex(); ++j)
	{
	  if (gs.edge(i,j) == Cassis::Engine::UNCOLORED)
	    {
	      Cassis::Engine::GameState cp (gs);
	      cp.play(i,j,player);
	      if (p1win(cp))
		{
		  auto h = gs.hash();
		  winning[h] = true;
		  move[h] = std::pair<Cassis::Engine::Vertex, Cassis::Engine::Vertex> (i,j);
		  return true; //winning move for player 1
		}
	    }
	}
      winning[gs.hash()] = false;

      return false; //could not find a winning combination for player 2
    }
  else
    {
      //try all move
      for (Cassis::Engine::Vertex i = 0; i< gs.nbVertex(); ++i)
	for (Cassis::Engine::Vertex j = i+1; j< gs.nbVertex(); ++j)
	{
	  if (gs.edge(i,j) == Cassis::Engine::UNCOLORED)
	    {
	      Cassis::Engine::GameState cp (gs);
	      cp.play(i,j,player);
	      if (! p1win(cp))
		{
		  auto h = gs.hash();
		  winning[h] = false;
		  move[h] = std::pair<Cassis::Engine::Vertex, Cassis::Engine::Vertex> (i,j);
		  return false; //winning move for player 2
		}
	    }
	}
      winning[gs.hash()] = true;      

      return true; //could not find a winning combination for player 2
    }
  //never reach here
  assert (0);
}

int main()
{
  util::timestamp t1;

  Cassis::Engine::GameState gs;

  try{
    if (p1win(gs))
      {
	std::cout<<"p1 win"<<std::endl;
      }
    else
      {
	std::cout<<"p1 lose"<<std::endl;
      }
  }
  catch(Cassis::Engine::InvalidParameter ip) {
    std::cout<<ip.getMsg()<<std::endl;
  }

  util::timestamp t2;

  std::cout<<"winning: "<<winning.size()<<std::endl;
  std::cout<<"move: "<<move.size()<<std::endl;
  std::cout<<t2-t1<<" seconds"<<std::endl;

  return 0;
}

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

#include <assert.h>
#include <iostream>
#include "GameState.hpp"
#include <stack>
#include <map>
#include "timestamp.hpp"
#include "IA.hpp"

const int NBROUND=14;

std::map<Cassis::Engine::GameState::HashType, bool>* winning;
std::map<Cassis::Engine::GameState::HashType,
	 std::pair<Cassis::Engine::Vertex,
		   Cassis::Engine::Vertex> >* move;

Cassis::Engine::Vertex* perm;

bool p1win(Cassis::Engine::GameState& gs, int round)
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

  auto it = winning[round].find(gs.hash());
  if (it != winning[round].end())
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
	      if (p1win(cp, round+1))
		{
		  auto h = gs.hash();
		  winning[round][h] = true;
		  
		  gs.normalize_permutation(perm);

		  move[round][h] = std::pair<Cassis::Engine::Vertex,
					     Cassis::Engine::Vertex> (perm[i],perm[j]);
		  return true; //winning move for player 1
		}
	    }
	}
      winning[round][gs.hash()] = false;

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
	      if (! p1win(cp, round+1))
		{
		  auto h = gs.hash();
		  winning[round][h] = false;

		  gs.normalize_permutation(perm);

		  move[round][h] = std::pair<Cassis::Engine::Vertex,
					     Cassis::Engine::Vertex> (perm[i], perm[j]);
		  return false; //winning move for player 2
		}
	    }
	}
      winning[round][gs.hash()] = true;      

      return true; //could not find a winning combination for player 2
    }
  //never reach here
  assert (0);
}

int main()
{
  util::timestamp t1;

  Cassis::Engine::GameState gs;

  perm = new Cassis::Engine::Vertex[gs.nbVertex()];

  winning = new std::map<Cassis::Engine::GameState::HashType, bool>[NBROUND+1];
  move = new  std::map<Cassis::Engine::GameState::HashType,
		       std::pair<Cassis::Engine::Vertex,
				 Cassis::Engine::Vertex> > [NBROUND+1];
  
  try{
    if (p1win(gs, 0))
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

  for (int i=0; i<=NBROUND; ++i)
    {
      std::cout<<"round "<<i<<std::endl;
      std::cout<<"winning: "<<winning[i].size()<<std::endl;
      std::cout<<"move: "<<move[i].size()<<std::endl;
    }
  std::cout<<t2-t1<<" seconds"<<std::endl;

  //compression by simple
  Cassis::IA::SimpleIA ia;
  try
  {
    Cassis::Engine::GameState gs;
    int useless = 0;

    for (int i=0; i<=NBROUND; ++i)
      {
	std::cout<<"Round "<<i<<std::endl;
	for (auto it = move[i].begin(); it != move[i].end(); ++it)
	  {
	    auto h = it->first;
	    auto m = it->second;

	    gs.unhash(h);
	    std::cout<<(int) (gs.whoseTurn())<<std::endl;
	    ia.play(gs);
	    auto newh = gs.hash();

	    gs.unhash(h);
	    std::cout<<(int) (gs.whoseTurn())<<std::endl;
	    gs.play(m.first, m.second, gs.whoseTurn());
	    auto acth = gs.hash();

	    std::cout<<acth<<"\t"<<newh<<std::endl;

	    if (acth == newh)
	      ++useless;
	  }
	std::cout<<"useless : "<<useless<<std::endl;
      }
  }
  catch (Cassis::Engine::InvalidParameter ip)
    {
      std::cout<<ip.getMsg()<<std::endl;
    }  

  delete[] winning;
  delete[] move;

  return 0;
}

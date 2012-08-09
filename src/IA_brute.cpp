#include "IA.hpp"
#include <assert.h>

#include "log.hpp"
#include "timestamp.hpp"

namespace Cassis
{
  namespace IA
  {
    bool BruteIA::p1win(Cassis::Engine::GameState& gs, int round)
    {
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
						   Cassis::Engine::Vertex> (perm[i], perm[j]);
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

    void BruteIA::play(Engine::GameState& gs)
    {
      try{
	auto turn = gs.whichTurn();
	auto h = gs.hash();
	
	//figure out permutation
	gs.normalize_inverse_permutation(permminus1);
	
	//look for it in the database using hash first. Most likely to match
	auto it = move[turn].find(h);
	if (it != move[turn].end())
	  {
	    gs.play(permminus1[it->second.first], permminus1[it->second.second], gs.whoseTurn());
	    return;
	  }
	else
	  {
	    //fallback to isomorphism detection
	    for (auto it2 = move[turn].begin(); it2 != move[turn].end(); ++it2)
	      {
		Engine::GameState gs2;
		auto h2 = it2->first;
		gs2.unhash(h2);
		
		if (gs.isomorph(gs2, permminus1))
		  {
		    gs.play(permminus1[it2->second.first], permminus1[it2->second.second], gs.whoseTurn());
		    return;
		  }
		
	      }
	  }
      }
      catch(Cassis::Engine::InvalidParameter ip) {
	Log::log<<"InvalidParameter Exception in IABrute::play()."<<ip.getMsg()<<Log::endl;
      }
      //we should never EVER reach here. But if we do for some strange
      //reason, it is better to fallback to a simpler IA than to crash
      //the game.
      Log::log<<"Default to simple"<<Log::endl;
      
      SimpleIA s;
      s.play(gs);
    }

    BruteIA::BruteIA()
    {
      Log::log<<"starting IA Brute"<<Log::endl;

      util::timestamp t1;
      Cassis::Engine::GameState gs;

      NBROUND = gs.nbEdge();
      
      perm = new Cassis::Engine::Vertex[gs.nbVertex()];
      permminus1 = new Cassis::Engine::Vertex[gs.nbVertex()];
      
#ifndef NOMAP
      winning = new std::map<Cassis::Engine::GameState::HashType, bool>[NBROUND+1];
      move = new  std::map<Cassis::Engine::GameState::HashType,
			   std::pair<Cassis::Engine::Vertex,
				     Cassis::Engine::Vertex> > [NBROUND+1];
#else
      winning = new mymap<Cassis::Engine::GameState::HashType, bool>[NBROUND+1];
      move = new mymap<Cassis::Engine::GameState::HashType,
		       std::pair<Cassis::Engine::Vertex,
				 Cassis::Engine::Vertex> > [NBROUND+1];
#endif      
      try{
	p1win(gs, 0); //evaluating it fills up data structures
      }
      catch(Cassis::Engine::InvalidParameter ip) {
	Log::log<<"InvalidParameter Exception"<<Log::endl;
	Log::log<<ip.getMsg()<<Log::endl;
      }

      util::timestamp t2;
      Log::log<<"Time to precompute tree: "<<(t2-t1)<<Log::endl;

      for (int i=0; i<=NBROUND; ++i)
	{
	  Log::log<<"winning["<<i<<"].size() == "<<(int)winning[i].size()<<Log::endl;
	  Log::log<<"move["<<i<<"].size() == "<<(int)move[i].size()<<Log::endl;
	}

      //no longer needed
      if (winning) {delete[] winning; winning = NULL;}
      if (perm) {delete[] perm; perm = NULL;}
    }

    BruteIA::~BruteIA()
    {
      if (move)	{delete[] move; move = NULL;}
      if (perm) {delete[] perm; perm = NULL;}
      if (permminus1) {delete[] permminus1; permminus1 = NULL;}
      if (winning) {delete[] winning; winning = NULL;}
    }
  }
}

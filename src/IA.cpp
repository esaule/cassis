#include "IA.hpp"

Cassis::IA::IA::~IA()
{}

Cassis::IA::SimpleIA::~SimpleIA()
{}

void Cassis::IA::SimpleIA::play(Engine::GameState& gs)
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

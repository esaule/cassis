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

    class SimpleIA : public IA
    {
      virtual void play(Engine::GameState& gs);
      virtual ~SimpleIA();
    };
  }
}

#endif

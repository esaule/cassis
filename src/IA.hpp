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

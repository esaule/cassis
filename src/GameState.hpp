#ifndef CASSIS_GAMESTATE
#define CASSIS_GAMESTATE

namespace Cassis
{
  namespace Engine
  {
    class Exception
    {
    };

    class InvalidParameter : public Exception
    {
    };

    enum Color
      {
	UNCOLORED,
	PLAYER1,
	PLAYER2
      };

    typedef int Vertex;

    class GameState
    {
    public:
      ///returns the total number of vertex in the game. By default, return 6.
      Vertex nbVertex() const;

      ///@brief returns the color of edge (i,j)
      ///@param i a valid vertex. 0 \leq i < nbVertex()
      ///@param j an other valid vertex. 0 \leq j < nbVertex(). i \neq j
      ///@return the color of edge (i,j)
      Color edge(Vertex i, Vertex j) const throw InvalidParameter;

      ///@brief returns the winner of the game.
      ///@return UNCOLORED if no winner
      Color winner() const;

      ///@brief returns the color of the player whose turn it is
      ///@return either PLAYER1 or PLAYER2
      Color whoseTurn() const;

      ///@brief change the state of the game to reflect player c took
      ///edge (i,j).
      ///@param i a valid vertex. 0 \leq i < nbVertex()
      ///@param j an other valid vertex. 0 \leq j < nbVertex(). i \neq j
      ///@param c either PLAYER1 or PLAYER2
      void play(Vertex i, Vertex j, Color c) throw InvalidParameter;
    };
  }
}

#endif

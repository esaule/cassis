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
    typedef int Edge;

    class GameState
    {
      Color* board;
      int turn;

    private:
      ///@brief throws exception if i is invalid
      ///@param i a valid vertex. 0 \leq i < nbVertex()
      void validateVertex(Vertex i) const throw (InvalidParameter);

      ///@brief returns a reference on a given edge. Note, the constraint i < j MUST be enforced prior to calling this function. (Or pay a segfault)
      ///@param i a valid vertex. 0 \leq i < nbVertex()
      ///@param j an other valid vertex. 0 \leq j < nbVertex(). i \neq j
      ///@return a reference to the color of edge (i,j)
      const Color& edgeLoc(Vertex i, Vertex j) const;

      ///@brief returns a reference on a given edge. Note, the constraint i < j MUST be enforced prior to calling this function. (Or pay a segfault)
      ///@param i a valid vertex. 0 \leq i < nbVertex()
      ///@param j an other valid vertex. 0 \leq j < nbVertex(). i \neq j
      ///@return a reference to the color of edge (i,j)

      Color& edgeLoc(Vertex i, Vertex j);
    public:
      ///returns the total number of vertices in the game. By default, return 6.
      Vertex nbVertex() const;

      ///returns the total number of edges in the game.
      Edge nbEdge() const;

      ///@brief returns the color of edge (i,j)
      ///@param i a valid vertex. 0 \leq i < nbVertex()
      ///@param j an other valid vertex. 0 \leq j < nbVertex(). i \neq j
      ///@return the color of edge (i,j)
      Color edge(Vertex i, Vertex j) const throw (InvalidParameter);

      ///@brief returns the winner of the game. If there is a winner, the vertices that made a triangle are returned in x,y,z. Otherwise their value are undetermined
      ///@return UNCOLORED if no winner
      Color winner(Vertex& x, Vertex& y, Vertex& z) const;

      ///@brief returns the color of the player whose turn it is
      ///@return either PLAYER1 or PLAYER2
      Color whoseTurn() const;

      ///@brief change the state of the game to reflect player c took
      ///edge (i,j).
      ///@param i a valid vertex. 0 \leq i < nbVertex()
      ///@param j an other valid vertex. 0 \leq j < nbVertex(). i \neq j
      ///@param c either PLAYER1 or PLAYER2
      void play(Vertex i, Vertex j, Color c) throw (InvalidParameter);

      GameState();
      ~GameState();
    };
  }
}

#endif

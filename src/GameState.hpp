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

#ifndef CASSIS_GAMESTATE
#define CASSIS_GAMESTATE

#include "serializable.hpp"
#include "exception.hpp"
#include <string>

namespace Cassis
{
  namespace Engine
  {
    class InvalidParameter : public BaseException
    {
      std::string s;

    public:
      InvalidParameter();
      InvalidParameter(std::string msg)
	:s(msg)
      {}
      const std::string& getMsg() const{return s;}
    };

    enum Color
      {
	UNCOLORED = 0,
	PLAYER1 = 1,
	PLAYER2 = 2
      };

    typedef int Vertex;
    typedef int Edge;

    class GameState : public Serializable
    {
    public:
      typedef int HashType;

    protected:

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
      ///@brief returns the amount of memory to serialize state.
      ///@return the amount of memory to serialize state (in bytes)
      virtual size_t serializesize() const;

      ///@brief serialize state
      ///@param p address where the state will be serialized. serializesize() bytes will be written. 
      virtual void serialize(char* p)const;

      ///@brief restore the state from a previously serialized buffer
      ///@param p address where the state is stored. serializesize() bytes will be read.
      virtual void deserialize(const char* p);

      ///returns the total number of vertices in the game. By default, return 6.
      Vertex nbVertex() const;

      ///returns the total number of edges in the game.
      Edge nbEdge() const;

      ///@brief returns the color of edge (i,j)
      ///@param i a valid vertex. 0 \leq i < nbVertex()
      ///@param j an other valid vertex. 0 \leq j < nbVertex(). i \neq j
      ///@return the color of edge (i,j)
      Color edge(Vertex i, Vertex j) const throw (InvalidParameter);

      ///@brief returns the winner of the game. If there is a winner, the vertices that made a triangle are returned in x,y,z with x < y < z. Otherwise their value are undetermined
      ///@return UNCOLORED if no winner
      Color winner(Vertex& x, Vertex& y, Vertex& z) const;

      ///@brief returns the winner of the game.
      ///@return UNCOLORED if no winner
      Color winner() const;


      ///@brief tells if one player won (any player)
      ///@return true is anybody won
      bool gameOver() const;


      ///@brief returns the color of the player whose turn it is
      ///@return either PLAYER1 or PLAYER2
      Color whoseTurn() const;

      ///@brief change the state of the game to reflect player c took
      ///edge (i,j).
      ///@param i a valid vertex. 0 \leq i < nbVertex()
      ///@param j an other valid vertex. 0 \leq j < nbVertex(). i \neq j
      ///@param c either PLAYER1 or PLAYER2
      void play(Vertex i, Vertex j, Color c) throw (InvalidParameter);


      ///
      HashType hash() const;


      GameState();
      GameState(const GameState&);
      ~GameState();
    private:
      GameState& operator= (const GameState&); //undefined

    };
  }
}

#endif

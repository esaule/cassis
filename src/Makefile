CXXFLAGS=--std=c++0x -g
LD=g++
CC=g++

cassis_cli: GameState.o cassis_cli.o

clean:
	-rm *.o
	-rm cassis_cli
	rm *~

depend:
	makedepend *.cpp *.hpp -Y .
# DO NOT DELETE

GameState.o: GameState.hpp
IA.o: IA.hpp GameState.hpp
cassis_cli.o: GameState.hpp
cassis_gtk_cairo.o: GameState.hpp IA.hpp cassis_cairo.hpp
IA.o: GameState.hpp
cassis_cairo.o: GameState.hpp IA.hpp
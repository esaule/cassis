CXXFLAGS=--std=c++0x
LD=g++
CC=g++

cassis_cli: GameState.o cassis_cli.o

clean:
	-rm *.o
	-rm cassis_cli

depend:
	makedepend *.cpp *.hpp -Y .
# DO NOT DELETE

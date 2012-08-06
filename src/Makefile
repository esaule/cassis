CXXFLAGS=--std=c++0x -g -O2 -Wall
LD=g++
CC=g++


cassis_cli: GameState.o cassis_cli.o

bruteforce: GameState.o bruteforce.o GameState_hash.o IA.o GameState_serialize.o


clean:
	-rm *.o
	-rm cassis_cli
	rm *~

depend:
	makedepend *.cpp *.hpp -Y.
# DO NOT DELETE

GameState.o: GameState.hpp serializable.hpp exception.hpp
GameState_hash.o: GameState.hpp serializable.hpp exception.hpp
GameState_serialize.o: GameState.hpp serializable.hpp exception.hpp
IA.o: IA.hpp GameState.hpp serializable.hpp exception.hpp
IA_brute.o: IA.hpp GameState.hpp serializable.hpp exception.hpp log.hpp
IA_brute.o: timestamp.hpp
SpriteCollection_cairo.o: SpriteCollection_cairo.hpp
bruteforce.o: GameState.hpp serializable.hpp exception.hpp timestamp.hpp
bruteforce.o: IA.hpp
cassis_cli.o: GameState.hpp serializable.hpp exception.hpp
cassis_gtk_cairo.o: GameState.hpp serializable.hpp exception.hpp IA.hpp
cassis_gtk_cairo.o: cassis_cairo.hpp cairo_graphic_controller.hpp
cassis_gtk_cairo.o: SpriteCollection_cairo.hpp cairo_menu_selector.hpp
cassis_gtk_cairo.o: cairo_text_display.hpp
log.o: log.hpp
GameState.o: serializable.hpp exception.hpp
IA.o: GameState.hpp serializable.hpp exception.hpp
cairo_menu_selector.o: cassis_cairo.hpp GameState.hpp serializable.hpp
cairo_menu_selector.o: exception.hpp IA.hpp cairo_graphic_controller.hpp
cairo_menu_selector.o: SpriteCollection_cairo.hpp cairo_text_display.hpp
cairo_text_display.o: cairo_graphic_controller.hpp
cassis_cairo.o: GameState.hpp serializable.hpp exception.hpp IA.hpp
cassis_cairo.o: cairo_graphic_controller.hpp SpriteCollection_cairo.hpp
serializable.o: exception.hpp

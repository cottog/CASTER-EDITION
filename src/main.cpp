#include "main.hpp"

Engine engine(80,70);
 
 //g++ src/*.cpp -o tuto -lmingw32 -lSDLmain -lSDL -Iinclude -Llib -ltcod-mingw -static-libgcc -static-libstdc++ -Wall
 // look for the NOTE and TODO comments throughout the code, to remember important thoughts or find a new task
 
int main() {
	engine.load();
    while ( !TCODConsole::isWindowClosed() ) {
    	engine.render();
    	TCODConsole::flush();
        engine.update();
    }
	engine.save();
    return 0;
}
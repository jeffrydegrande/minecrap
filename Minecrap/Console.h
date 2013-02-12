#include <vector>
using namespace std;
#include <windows.h>
//include this header for CVars and GLConsole
#include <GLConsole/GLConsole.h>
//A CVar version of std::vector
#include <CVars/CVarVectorIO.h>
//A CVar version of std::map
#include <CVars/CVarMapIO.h>

void ConsoleToggle();
bool ConsoleIsOpen();
void ConsoleRender();
void ConsoleInput (int key, int char_code);
void ConsoleUpdate ();
void ConsoleLog (const char* message, ...);
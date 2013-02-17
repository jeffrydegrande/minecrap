#include "Engine.h"
#include "World.h"
#include <CVars/CVar.h>

#ifdef _WIN32
int PASCAL WinMain(HINSTANCE instance_in, HINSTANCE previous_instance, LPSTR command_line, int show_style)
#else
int main(int argc, char **argv)
#endif
{

    CVarUtils::CreateCVar("flying", false, "Player can fly?");

	Engine engine;
	engine.run();
	return 0;
}

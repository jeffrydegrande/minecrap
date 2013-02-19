#include "Engine.h"
#include "World.h"
#include <CVars/CVar.h>


#define CONFIG_FILE "user.cfg"


#ifdef _WIN32
int PASCAL WinMain(HINSTANCE instance_in, HINSTANCE previous_instance, LPSTR command_line, int show_style)
#else
int main(int argc, char **argv)
#endif
{

    CVarUtils::CreateCVar("player.info", true, "Show player info");
    CVarUtils::CreateCVar("flying", false, "Player can fly?");
    CVarUtils::CreateCVar("seed", 300, "Game seed (changing requires restart)");
    printf( "Loading %s\n", CONFIG_FILE);
    CVarUtils::Load(CONFIG_FILE);

	Engine engine;
	engine.run();

    printf( "Saving %s\n", CONFIG_FILE);
    CVarUtils::Save(CONFIG_FILE);
	return 0;
}

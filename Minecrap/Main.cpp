#include "Engine.h"
#include "World.h"

int PASCAL WinMain(HINSTANCE instance_in, HINSTANCE previous_instance, LPSTR command_line, int show_style) {
	Engine engine;
	engine.run();
	return 0;
}

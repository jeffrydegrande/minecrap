#ifndef MINECRAP_ENGINE_H
#define MINECRAP_ENGINE_H

#include "minecrap.h"
#include "Matrix.h"
#include "Vec3.h"

class World;
class Crosshair;
class Player;
class Engine
{
	int center_x;
	int center_y;
	long last_update;
	long elapsed;
	unsigned int fps_current; //the current FPS.

	bool quit;

	World *world;
	Player *player;
	Crosshair *crosshair;
    int blocksRendered;

public:
	Engine();
	~Engine();
	void run();

    static float elapsedSeconds();

private:
	void init();
	void initRenderer(int width, int height, int bits, bool fullscreen);
	
	void update();
	void collectInput();

	long tick();
	void stop();

	void render();
    void renderFPS();
    void renderPlayerPosition();
    void renderPlayerDirection();
    void renderRenderStats();
};

#endif
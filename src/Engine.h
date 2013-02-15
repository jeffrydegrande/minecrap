#ifndef MINECRAP_ENGINE_H
#define MINECRAP_ENGINE_H

#include "minecrap.h"

class World;
class Crosshair;
class Player;

class Engine
{

	int center_x;
	int center_y;
	long last_update;
	long elapsed;
	float elapsed_seconds;
	unsigned int fps_current; //the current FPS.

	int view_width;
	int view_height;
	float view_aspect;
	
	SDL_Surface* screen;

	bool quit;

	World *world;
	Player *player;
	Crosshair *crosshair;

public:
	Engine();
	~Engine();
	void run();

private:
	void init();
	void initRenderer(int width, int height, int bits, bool fullscreen);
	
	void update();
	void collectInput();


	void render();
	void render2D();
	void render3D();
	
	long tick();
	void stop();

    void renderFPS();
    void renderPlayerPosition();
    void renderPlayerDirection();
};

#endif
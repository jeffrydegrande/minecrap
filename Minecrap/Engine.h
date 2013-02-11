#ifndef MINECRAP_ENGINE_H
#define MINECRAP_ENGINE_H


struct SDL_Surface;
class World;
class Camera;
class Crosshair;

class Engine
{

	int center_x;
	int center_y;
	long last_update;
	long elapsed;
	float elapsed_seconds;

	int view_width;
	int view_height;
	float view_aspect;
	SDL_Surface* screen;

	bool quit;

	World *world;
	Camera *camera;
	Crosshair *crosshair;

public:
	Engine();
	~Engine();
	void run();

private:
	void init();
	void initRenderer(int width, int height, int bits, bool fullscreen);
	void update();
	void render();
	long tick();
	void stop();
};

#endif
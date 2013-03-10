#ifndef MINECRAP_ENGINE_H
#define MINECRAP_ENGINE_H

#include "minecrap.h"
#include "Matrix.h"
#include "Vec.h"
#include "Frustum.h"
#include "MatrixStack.h"

class World;
class Crosshair;
class Player;
class Shader;
class Engine
{
	int center_x;
	int center_y;
	long last_update;
	long elapsed;
	unsigned int fps_current; //the current FPS.
	int width;
	int height;

    Frustum frustum;

	bool quit;
    bool paused;

	World *world;
	Player *player;
	Crosshair *crosshair;

    Shader *shader;

    Vec3 cameraPosition;
    Vec3 cameraDirection;
 
    Vec4 lightIntensity;
    bool night;

    bool optionRenderWireframe;
    bool optionLighting;

    MatrixStack projection;

public:
	Engine();
	~Engine();
	void run();

    static float elapsedSeconds();

    void resizeWindow(int w, int h);

    int viewWidth() const;
    int viewHeight() const;

    void printError();

    void flush();
    bool withinFrustum(float x, float y, float z, float radius);
    void updateFrustum();

    void toggleDayNight();

private:

	void init();
    void compileShaders();
    void setupProjectionMatrix();
	
	void update(float elapsed);
	void collectInput();

	long tick();
	void stop();

	void render();
    void render3D();
    void render2D();
};

#endif
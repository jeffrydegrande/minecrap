#ifndef MINECRAP_GRAPHICS_H
#define MINECRAP_GRAPHICS_H

#include "Vec3.h"
#include "Matrix.h"
#include "Frustum.h"

#define RENDER_DISTANCE  1536.0f
#define NEAR_CLIP        0.2f
#define FOV              45


#define BUFFER_OFFSET(i) ((char *)NULL + (i))


class Player;
class SDL_Surface;
class Graphics {

	int width;
	int height;
	float aspect;
	
	SDL_Surface* screen;

    Vec3 cameraPosition;
    Vec3 cameraDirection;

    Frustum frustum;

    bool renderAsWireframe;
    bool renderWithLights;

    public:
        static void Init();
        static void Cleanup();

        Graphics();
        ~Graphics();

        int viewWidth() const;
        int viewHeight() const;

        void initRenderer(int width, int height, int bits, bool fullscreen);

        void begin3D();
        void end3D();

        void begin2D();
        void end2D();
        void flush();

        void setCameraFromPlayer(Player *player);
        bool withinFrustum(float x, float y, float z, float radius);
        void updateFrustum();

        void toggleRenderingAsWireframe();
        void toggleLights();

};

extern Graphics *graphics;

#endif

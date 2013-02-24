#ifndef MINECRAP_GRAPHICS_H
#define MINECRAP_GRAPHICS_H

#include "Vec3.h"
#include "Matrix.h"
#include "Frustum.h"

#define RENDER_DISTANCE  1536.0f
#define NEAR_CLIP        0.2f
#define FOV              45

class Player;
class SDL_Surface;
class Graphics {

	int width;
	int height;
	float aspect;
	
	SDL_Surface* screen;

    Matrix4 cameraMatrix;
    Vec3 cameraPosition;

    Frustum frustum;

    public:
        static void Init();
        static void Cleanup();

        int viewWidth() const;
        int viewHeight() const;

        void initRenderer(int width, int height, int bits, bool fullscreen);

        void begin3D();
        void end3D();

        void begin2D();
        void end2D();
        void flush();

        bool withinFrustum(float x, float y, float z, float radius);

        void setCameraMatrix(const Matrix4 &m);
        void setCameraPosition(const Vec3 &p);
        void setCameraFrom(Player *player);
        void renderFrustum();

    private:
        void buildFrustumPlanes();
};

extern Graphics *graphics;

#endif

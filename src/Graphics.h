#ifndef MINECRAP_GRAPHICS_H
#define MINECRAP_GRAPHICS_H

#include "Vec.h"
#include "Matrix.h"
#include "Frustum.h"

#define RENDER_DISTANCE  1536.0f
#define NEAR_CLIP        0.2f
#define FOV              45


#define BUFFER_OFFSET(i) ((char *)NULL + (i))


class Shader;
class Player;
class Graphics {

	int width;
	int height;
	float aspect;

	SDL_Surface* screen;
    Shader *shader;

    Vec3 cameraPosition;
    Vec3 cameraDirection;

    Frustum frustum;
    Matrix4 projection;

    bool renderAsWireframe;
    bool renderWithLights;

    public:
        static void Init();
        static void Cleanup();

        Graphics();
        ~Graphics();

        int viewWidth() const;
        int viewHeight() const;

        void printError();

        void initRenderer(int width, int height, int bits, bool fullscreen);

        void setupProjectionMatrix(float fovy, float aspect, float near, float far);
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

    private:

        void compileShaders();

};

extern Graphics *graphics;

#endif

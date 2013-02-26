#include "Graphics.h"
#include "minecrap.h"
#include "Player.h"
#include "Block.h"
#include <cassert>

Graphics *graphics = NULL;

void Graphics::Init() {
    graphics = new Graphics();
}

void Graphics::Cleanup() {
    delete graphics;
}

Graphics::Graphics()
{
    renderAsWireframe = false;
}

Graphics::~Graphics()
{
}

int Graphics::viewWidth() const {
    return width;
}

int Graphics::viewHeight() const {
    return height;
}

void Graphics::initRenderer(int width, int height, int bits, bool fullscreen) {
    int flags;
    float fovy;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    this->width = width;
    this->height = height;
    this->aspect = (float)width / (float) height;

    flags = SDL_OPENGL;
    if (fullscreen)
        flags |= SDL_FULLSCREEN;
    else
        flags |= SDL_RESIZABLE;

    screen = SDL_SetVideoMode(width, height, bits, flags);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    fovy = FOV;

    if (this->aspect > 1.0f)
        fovy /= this->aspect;

    glViewport (0, 0, this->width, this->height);

    gluPerspective(fovy, this->aspect, NEAR_CLIP, RENDER_DISTANCE);

    glMatrixMode(GL_MODELVIEW);
}

void Graphics::toggleRenderingAsWireframe()
{
    renderAsWireframe = !renderAsWireframe;

    if (renderAsWireframe)
        printf("Wireframe rendering enabled.\n" );
    else
        printf("Wireframe rendering disabled.\n");
}

void Graphics::updateFrustum() {
    float p[16];
    float m[16];
    glGetFloatv(GL_PROJECTION_MATRIX, p);
    glGetFloatv(GL_MODELVIEW_MATRIX, m);

    Matrix4 P = p;
    Matrix4 M = m;
    Matrix4 A = M * P;
    frustum.setFrustum(A.data());
}

bool Graphics::withinFrustum(float x, float y, float z, float radius) {
    Vec3 point(x, y, z);

    if ( point.y < cameraPosition.y - 4)
        return false;

    return frustum.sphereInFrustum(point, radius);
}

void Graphics::setCameraFromPlayer(Player *player) {
    cameraPosition = player->getPosition();
    cameraDirection = player->getDirection();
}

void Graphics::begin3D() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();

    GLfloat lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f};
    GLfloat light_diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f};
    // GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat light_full_on[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat white[]    = {1.0f, 1.0f, 1.0f, 1.0f};


    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_full_on);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glDepthFunc (GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);


    if (renderAsWireframe) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    } else {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK);


    glClearColor(0.52f, 0.74f, 0.84f, 1.0f);

}

void Graphics::end3D() {
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Graphics::begin2D() {
    // render 2D stuff
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, graphics->viewWidth(), graphics->viewHeight(), 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void Graphics::end2D() {
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

}

void Graphics::flush() {
    SDL_GL_SwapBuffers();
}

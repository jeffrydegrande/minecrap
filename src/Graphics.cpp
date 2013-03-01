#include "Graphics.h"
#include "minecrap.h"
#include "Player.h"
#include "Shader.h"
#include "Vec.h"

#include <cassert>
#include <cmath>

#define ASSERT_NO_GL_ERROR assert(GL_NO_ERROR == glGetError())

Graphics *graphics = NULL;

static void displayOpenGLInfo() {
    printf ("vendor: %s\n",  (const unsigned char *)glGetString(GL_VENDOR));
    printf ("renderer: %s\n", (const unsigned char *)glGetString(GL_RENDERER));
    printf ("version: %s\n", (const unsigned char *)glGetString(GL_VERSION));
    printf ("GLSL version: %s\n", (const unsigned char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf ("\n");
}

void Graphics::Init() {
    graphics = new Graphics();

}

void Graphics::Cleanup() {
    delete graphics;
}

Graphics::Graphics(): 
    shader(NULL),
    renderAsWireframe(false),
    renderWithLights(true)
{
}

Graphics::~Graphics()
{
    delete shader;
}

int Graphics::viewWidth() const {
    return width;
}

int Graphics::viewHeight() const {
    return height;
}

void Graphics::compileShaders()
{
    if (shader == NULL) {
        shader = new Shader();
#ifdef _WIN32
        shader->addVertexShader("..\\shaders\\hello_world.vert");
        shader->addFragmentShader("..\\shaders\\hello_world.frag");
#else
        shader->addVertexShader("shaders/hello_world.vert");
        shader->addFragmentShader("shaders/hello_world.frag");
#endif
        shader->done();
    }
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

    // TODO: this is probably a leak
    screen = SDL_SetVideoMode(width, height, bits, flags);
#ifdef _WIN32
    GLenum error = glewInit();
    if (GLEW_OK != error) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
        exit(1);
    }
#endif

    displayOpenGLInfo();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    fovy = FOV;
    if (this->aspect > 1.0f)
        fovy /= this->aspect;

    this->compileShaders();
    setupProjectionMatrix(fovy, aspect, NEAR_CLIP, RENDER_DISTANCE);
    shader->setPerspectiveMatrix(projection);
    ASSERT_NO_GL_ERROR;

    glLoadMatrixf(projection.value_ptr());
    glViewport (0, 0, this->width, this->height);
    glMatrixMode(GL_MODELVIEW);
}

void Graphics::setupProjectionMatrix(float fovy, float aspect, float znear, float zfar)
{
    float m[16];
    memset(m, 0, sizeof(m));

    float depth = zfar - znear;
    float f = 1.0f/tan(fovy*PI/360);

    Matrix4 M;
    m[0]  = f / aspect;
    m[5]  = f;
    m[10] = -(zfar + znear) / depth;
    m[14] = -2 * (zfar*znear) / depth;
    m[11] = -1.0f;

    projection = m;
}

void Graphics::printError()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("OpenGL error: %s\n", gluErrorString(error));
    }
}

void Graphics::toggleRenderingAsWireframe()
{
    renderAsWireframe = !renderAsWireframe;

    if (renderAsWireframe)
        printf("Wireframe rendering enabled.\n" );
    else
        printf("Wireframe rendering disabled.\n");
}

void Graphics::toggleLights()
{
    renderWithLights = !renderWithLights;
}

void Graphics::updateFrustum() {
    float p[16];
    float m[16];
    glGetFloatv(GL_PROJECTION_MATRIX, p);
    glGetFloatv(GL_MODELVIEW_MATRIX, m);

    Matrix4 P = p;
    Matrix4 M = m;
    Matrix4 A = M * P;
    frustum.setFrustum(A.value_ptr());
}

bool Graphics::withinFrustum(float x, float y, float z, float radius) {
    Vec3 point(x, y, z);

    if ( point.y < cameraPosition.y - 4)
        return false;

    if (Frustum::OUTSIDE != frustum.sphereInFrustum(point, radius))
        return true;
    else
        return false;
}

void Graphics::setCameraFromPlayer(Player *player) {
    Matrix4 camera = Matrix4::load(GL_MODELVIEW_MATRIX);
    shader->setCameraMatrix(camera);
    cameraPosition = player->getPosition();
    cameraDirection = player->getDirection();
}

void Graphics::begin3D() {
    ASSERT_NO_GL_ERROR;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    shader->use();

    GLfloat lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f};
    GLfloat light_diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f};
    //GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat light_full_on[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat white[]    = {1.0f, 1.0f, 1.0f, 1.0f};

    glDepthFunc (GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    if (renderWithLights) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glLightfv(GL_LIGHT0, GL_AMBIENT, white);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_full_on);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    } else {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }


    if (renderAsWireframe) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    } else {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK);
    glClearColor(0.52f, 0.74f, 0.84f, 1.0f);
    ASSERT_NO_GL_ERROR;
}

void Graphics::end3D() {
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    shader->dontUse();
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

    ASSERT_NO_GL_ERROR;
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

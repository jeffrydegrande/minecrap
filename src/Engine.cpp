#include "minecrap.h"
#include "Engine.h"
#include "World.h"
#include "Player.h"
#include "Crosshair.h"
#include "Console.h"
#include "OSD.h"
#include "Input.h"
#include "Shader.h"
#include "MatrixStack.h"

#include <sstream>

#include <cmath>
#include <cassert>

#define ZFAR  1536.0f
#define ZNEAR 0.2f
#define FOV   45

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768
#define FULLSCREEN false

#define FPS_INTERVAL     1.0f
#define ASSERT_NO_GL_ERROR assert(GL_NO_ERROR == glGetError())

// static float elapsed_seconds = 0.0f;

static void displayOpenGLInfo() {
    printf ("vendor: %s\n",  (const unsigned char *)glGetString(GL_VENDOR));
    printf ("renderer: %s\n", (const unsigned char *)glGetString(GL_RENDERER));
    printf ("version: %s\n", (const unsigned char *)glGetString(GL_VERSION));
    printf ("GLSL version: %s\n", (const unsigned char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf ("\n");
}

Engine::Engine():
    fps_current(0),
    quit(false),
    world(NULL),
    player(NULL),
    crosshair(NULL),
    shader(NULL),
    optionRenderWireframe(false),
    optionLighting(true)
{
    init();
}

Engine::~Engine() {
    delete this->world;
    delete this->player;
    delete this->crosshair;

    SDL_Quit();
}

void Engine::init() {
    int flags;


    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_JOYSTICK) != 0) {
        return;
    }

    this->width  = WINDOW_WIDTH;
    this->height = WINDOW_HEIGHT;

    flags = SDL_OPENGL;
    if (FULLSCREEN)
        flags |= SDL_FULLSCREEN;
    else
        flags |= SDL_RESIZABLE;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetVideoMode(width, height, 32, flags);

#ifdef _WIN32
    GLenum error = glewInit();
    if (GLEW_OK != error) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
        exit(1);
    }
#endif

    OSD_Init();

    displayOpenGLInfo();

    compileShaders();

    resizeWindow(width, height);

    SDL_WM_SetCaption("Minecrap","");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_ShowCursor (false);

    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        SDL_JoystickEventState(SDL_ENABLE);
        SDL_JoystickOpen(i);
    }

    world = new World(CVarUtils::GetCVar<int>("seed"));
    player = world->spawnPlayer();
    crosshair = new Crosshair(viewWidth(), viewHeight());
}

void Engine::resizeWindow(int width, int height) {
    float aspect = (float)width / (float)height;

    this->width  = width;
    this->height = height;

	projection.Perspective(FOV, aspect, ZNEAR, ZFAR);

	// TODO: executing this here throws an error while it's
	//		 really the optimal place to do this. Investigate
    // shader->setCameraToClipMatrix(perspective);
	// ASSERT_NO_GL_ERROR;

    glViewport (0, 0, this->width, this->height);

    if (crosshair) {
        delete crosshair;
        crosshair = new Crosshair(viewWidth(), viewHeight());
    }


    osd->setWidth(width);
    ASSERT_NO_GL_ERROR;
}

void printCurrentMatrix(GLenum matrix)
{
    Matrix4 M = Matrix4::Load(matrix);
    M.print();
}

void Engine::setupProjectionMatrix()
{
}

void Engine::compileShaders()
{
    printf("Compiling shaders\n");
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


//////////////////////////////////////////////////////
// Game Loop
//////////////////////////////////////////////////////

void Engine::stop() {
    this->quit = true;
}

inline long Engine::tick() {
    return SDL_GetTicks();
}

void Engine::run() {
    unsigned int fps_lasttime = tick(); //the last recorded time.
    unsigned fps_frames = 0; // frames passed since the last recorded fps.

    long last_update = 0;
    long now = 0;

    while(!quit) {
        now = tick();
        update((float)last_update);
        render();
        last_update = tick() - now;
        fps_frames++;
        if (fps_lasttime < tick() - FPS_INTERVAL*1000)
        {
            fps_lasttime = tick();
            fps_current = fps_frames;
            fps_frames = 0;
        }
    }
}

//////////////////////////////////////////////////////
// Advancing the simulation
//////////////////////////////////////////////////////

void Engine::update(float elapsed) {
    float elapsedInSeconds = elapsed / 1000.f;

    this->collectInput();
#ifdef SUPPORT_GLCONSOLE
    ConsoleUpdate ();
#endif
    player->update(elapsedInSeconds);
    world->update();
}

void Engine::collectInput() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            stop();
            break;
        case SDL_KEYUP:
            Input::keyReleased(event.key.keysym.sym);
            break;

        case SDL_KEYDOWN:
#ifdef SUPPORT_GLCONSOLE
            if (ConsoleIsOpen ()) {
                ConsoleInput (event.key.keysym.sym, event.key.keysym.unicode);
                break;
            }
#endif

            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    stop();
                    break;
#ifdef SUPPORT_GLCONSOLE
                case SDLK_BACKQUOTE:
                    ConsoleToggle();
                    break;
#endif
                case SDLK_F2:
                    optionLighting = !optionLighting;
                    break;
                case SDLK_F3:
                    optionRenderWireframe = !optionRenderWireframe;
                    break;
                default:
                    Input::keyPressed(event.key.keysym.sym);
            }
            break;
        case SDL_JOYAXISMOTION:
            break;
        case SDL_JOYBUTTONDOWN:
            break;
        case SDL_MOUSEMOTION:
            {
              int w = viewWidth();
              int h = viewHeight();

              if (event.motion.x < 20 || event.motion.x > w - 20 ||
                  event.motion.y < 20 || event.motion.y > h - 20) {
#ifdef SUPPORT_GLCONSOLE
                if (ConsoleIsOpen())
                    break;
#endif
                SDL_WarpMouse(w / 2, h / 2);
              } else {
                player->look(event.motion.xrel, event.motion.yrel);
              }
            }
            break;
        case SDL_VIDEORESIZE:
            resizeWindow(event.resize.w, event.resize.h);
            break;
        default:
            // noop
            break;
        }
    }
}


//////////////////////////////////////////////////////
// Rendering
//////////////////////////////////////////////////////

void Engine::render() {
    glClearColor(0.52f, 0.74f, 0.84f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render3D();
    render2D();
    flush();

	GLenum error;

	while (GL_NO_ERROR != (error=glGetError())) {
		std::string s = reinterpret_cast<const char *>(gluErrorString(error));
		fprintf(stderr, "Error: %s\n", s.c_str());
        exit(1);
	}
}

void Engine::render3D() {
    ASSERT_NO_GL_ERROR;

    glPolygonMode( GL_FRONT_AND_BACK, optionRenderWireframe ? GL_LINE : GL_FILL );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

	UseShader use(*shader);

	Matrix4 cameraToClipMatrix = projection.top();

    MatrixStack model;
    Matrix4 camera = player->getCameraMatrix();
    model.apply(camera);

    Vec4 lightDirectionCameraSpace = camera * Vec4(0.866f, 1.0f, 0.0f, 0.0f);
    Matrix3 normal(camera);

    shader->setCameraToClipMatrix(cameraToClipMatrix);
    shader->setNormalModelToCameraMatrix(normal);
    shader->setDirectionToLight(lightDirectionCameraSpace);
    shader->setModelToCameraMatrix(camera);
    world->render();

    ASSERT_NO_GL_ERROR;
}

void Engine::render2D() {
    ASSERT_NO_GL_ERROR;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, viewWidth(), viewHeight(), 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    crosshair->render();
    renderOnScreenDisplay();

#ifdef SUPPORT_GLCONSOLE
    ConsoleRender();
#endif

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    ASSERT_NO_GL_ERROR;
}

void Engine::renderOnScreenDisplay() {
    Vec3 pos   = player->getPosition();
    Vec3 angle = player->getDirection();

    osd->write("FPS: %d", fps_current);
    osd->write("Loc: %0.2f, %0.2f, %0.2f",
                pos.x, pos.y, pos.z);
    osd->write("Ang: %0.2f, %0.2f, %0.2f, facing %s",
                angle.x, angle.y, angle.z, player->getDirectionAsString());

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        osd->write("OpenGL error: %s\n", gluErrorString(error));
    }

    osd->reset();
}


int Engine::viewWidth() const {
    return width;
}

int Engine::viewHeight() const {
    return height;
}


void Engine::flush() {
    SDL_GL_SwapBuffers();
}

void Engine::updateFrustum() {
    return;

    float p[16];
    float m[16];
    glGetFloatv(GL_PROJECTION_MATRIX, p);
    glGetFloatv(GL_MODELVIEW_MATRIX, m);

    Matrix4 P = p;
    Matrix4 M = m;
    Matrix4 A = M * P;
    frustum.setFrustum(A.value_ptr());
}

bool Engine::withinFrustum(float x, float y, float z, float radius) {
    Vec3 point(x, y, z);

    if ( point.y < cameraPosition.y - 4)
        return false;

    if (Frustum::OUTSIDE != frustum.sphereInFrustum(point, radius))
        return true;
    else
        return false;
}


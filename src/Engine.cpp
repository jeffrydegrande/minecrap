#include "minecrap.h"
#include "Engine.h"
#include "World.h"
#include "Player.h"
#include "Crosshair.h"
#include "Console.h"
#include "OSD.h"
#include "Input.h"
#include "Shader.h"

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

static float elapsed_seconds = 0.0f;

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
    renderAsWireframe(false),
    renderWithLights(true)
{
    init();
}

Engine::~Engine() {
    delete this->world;
    delete this->player;
    delete this->crosshair;
}

void Engine::init() {
    int flags;

    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_JOYSTICK) != 0) {
        return;
    }

    width  = WINDOW_WIDTH;
    height = WINDOW_HEIGHT;

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

    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        SDL_JoystickEventState(SDL_ENABLE);
        SDL_JoystickOpen(i);
    }

    SDL_ShowCursor (false);


    world = new World(CVarUtils::GetCVar<int>("seed"));
    player = world->spawnPlayer();
    crosshair = new Crosshair(viewWidth(), viewHeight());
}

void Engine::resizeWindow(int width, int height) {
    this->width  = width;
    this->height = height;

    printf( "Setting window size to %d by %d\n", width, height );

    glMatrixMode(GL_PROJECTION);

    setupProjectionMatrix();
    glLoadMatrixf(projection.value_ptr());
    shader->setPerspectiveMatrix(projection);
    glViewport (0, 0, this->width, this->height);

    if (crosshair) {
        delete crosshair;
        crosshair = new Crosshair(viewWidth(), viewHeight());
    }

    osd->setWidth(width);

    ASSERT_NO_GL_ERROR;
}

void Engine::setupProjectionMatrix()
{
    float fovy   = FOV;
    float aspect = (float)this->width / (float)this->height;

    if (aspect > 1.0f)
        fovy /= aspect;


    float m[16];
    memset(m, 0, sizeof(m));

    float depth = ZFAR - ZNEAR;
    float f = 1.0f/tan(fovy*PI/360);

    Matrix4 M;
    m[0]  = f / aspect;
    m[5]  = f;
    m[10] = -(ZFAR + ZNEAR) / depth;
    m[14] = -2 * (ZFAR*ZNEAR) / depth;
    m[11] = -1.0f;

    projection = m;
}

void Engine::compileShaders()
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
    long stop;
    long remaining;

    unsigned int fps_lasttime = tick(); //the last recorded time.
    unsigned fps_frames = 0; // frames passed since the last recorded fps.

    while(!quit) {
        stop = tick() + 15;
        update();
        render();

        remaining = stop - tick();
        if (remaining > 0) {
            sleep(remaining);
        }

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

void Engine::update() {
    this->collectInput();
    ConsoleUpdate ();
    world->update();
    player->update();

}

void Engine::collectInput() {
    SDL_Event event;
    long now;

    while (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            stop();
            break;
        case SDL_KEYUP:
            Input::keyReleased(event.key.keysym.sym);
            break;

        case SDL_KEYDOWN:
            if (ConsoleIsOpen ()) {
                ConsoleInput (event.key.keysym.sym, event.key.keysym.unicode);
                break;
            } else {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        stop();
                        break;
                    case SDLK_BACKQUOTE:
                        ConsoleToggle();
                        break;
                    case SDLK_F2:
                        toggleLights();
                        break;
                    case SDLK_F3:
                        toggleRenderingAsWireframe();
                        break;
                    default:
                        Input::keyPressed(event.key.keysym.sym);
                }
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
                if (!ConsoleIsOpen()) {
                  SDL_WarpMouse(w / 2, h / 2);
                }
              } else {
                player->look(event.motion.yrel, event.motion.xrel);
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

    now = this->tick();
    elapsed = now - last_update;

    if (elapsed > 0) {
        elapsed_seconds = (float)elapsed / 1000.0f;
    }
    last_update = now;
}


//////////////////////////////////////////////////////
// Rendering
//////////////////////////////////////////////////////

void Engine::render() {
    glClearColor(0.52f, 0.74f, 0.84f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    if (renderAsWireframe) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    } else {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK);
    glShadeModel(GL_SMOOTH);

    Matrix4 M = Matrix4::load(GL_MODELVIEW_MATRIX);

    Vec4 lightDirection(0.0f, 100.0f, 0.0f, 0.0f);
    Vec4 lightDirectionCameraSpace = M * lightDirection;

    shader->setDirectionToLight(lightDirectionCameraSpace);

    Matrix3 NM(M);
    shader->setNormalToCameraMatrix(NM);
    shader->use();

    player->render();
    setCameraFromPlayer(player);
    updateFrustum();
    world->render();

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    shader->dontUse();

    ASSERT_NO_GL_ERROR;

    // render 2D stuff
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

    ConsoleRender();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    flush();

	GLenum error;

	while (GL_NO_ERROR != (error=glGetError())) {
		std::string s = reinterpret_cast<const char *>(gluErrorString(error));
		printf( "Error: %s\n", s.c_str());
        exit(1);
	}
}

void Engine::renderOnScreenDisplay() {
    Vec3 pos   = player->getPosition();
    Vec3 angle = player->getDirection();

    osd->write("FPS: %d\n", fps_current);
    osd->write("Loc: %0.2f, %0.2f, %0.2f",
                pos.x, pos.y, pos.z);
    osd->write("Ang: %0.2f, %0.2f, %0.2f, facing %s\n",
                angle.x, angle.y, angle.z, player->getDirectionAsString());
    osd->write("Blocks: %d\n", blocksRendered);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        osd->write("OpenGL error: %s\n", gluErrorString(error));
    }

    osd->reset();
}


float Engine::elapsedSeconds() {
    return elapsed_seconds;
}

int Engine::viewWidth() const {
    return width;
}

int Engine::viewHeight() const {
    return height;
}

void Engine::toggleRenderingAsWireframe()
{
    renderAsWireframe = !renderAsWireframe;

    if (renderAsWireframe)
        printf("Wireframe rendering enabled.\n" );
    else
        printf("Wireframe rendering disabled.\n");
}

void Engine::toggleLights()
{
    renderWithLights = !renderWithLights;
}

void Engine::updateFrustum() {
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

void Engine::setCameraFromPlayer(Player *player) {
    Matrix4 camera = Matrix4::load(GL_MODELVIEW_MATRIX);
    shader->setCameraMatrix(camera);
    cameraPosition = player->getPosition();
    cameraDirection = player->getDirection();
}

void Engine::flush() {
    SDL_GL_SwapBuffers();
}

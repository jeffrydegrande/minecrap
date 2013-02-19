#include "minecrap.h"
#include "Engine.h"
#include "World.h"
#include "Player.h"
#include "Crosshair.h"
#include "Console.h"
#include "Text.h"
#include "Input.h"

#include <sstream>

#define RENDER_DISTANCE  1536
#define NEAR_CLIP        0.2f
#define FOV              60
#define FPS_INTERVAL    1.0f

static float elapsed_seconds = 0.0f;

Engine::Engine() : fps_current(0) {
    this->quit = false;
    init();

    world = new World(CVarUtils::GetCVar<int>("seed"));
    player = world->spawnPlayer();
}

Engine::~Engine() {
    delete this->world;
    delete this->player;
    delete this->crosshair;
}

void Engine::stop() {
    this->quit = true;
}

long Engine::tick() {
    return SDL_GetTicks();
}

void Engine::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_JOYSTICK) != 0) {
        return;
    }
    SDL_WM_SetCaption("","");
    initRenderer(800, 600, 32, FULLSCREEN);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        SDL_JoystickEventState(SDL_ENABLE);
        SDL_JoystickOpen(i);    
    }

    SDL_ShowCursor (false);

    TextInit();

}

void Engine::initRenderer(int width, int height, int bits, bool fullscreen) {
    int flags;
    float fovy;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    view_width = width;
    view_height = height;
    view_aspect = (float)width / (float) height;

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

    if (view_aspect > 1.0f)
        fovy /= view_aspect;

    gluPerspective(fovy, view_aspect, NEAR_CLIP, RENDER_DISTANCE);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.52f, 0.74f, 0.84f, 1.0f);
    crosshair = new Crosshair(view_width, view_height);
}

void Engine::run() {
    long stop;
    long remaining;

    unsigned int fps_lasttime = tick(); //the last recorded time.
    unsigned fps_frames = 0; //frames passed since the last recorded fps.

    while(!quit) {
        stop = tick() + 15;
        update();
        render();

        remaining = stop - tick();
        if (remaining > 0)
            sleep(remaining);

        fps_frames++;
        if (fps_lasttime < tick() - FPS_INTERVAL*1000)
        {
            fps_lasttime = tick();
            fps_current = fps_frames;
            fps_frames = 0;
        }
    }
}

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
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    stop();
                } else if (event.key.keysym.sym == SDLK_BACKQUOTE) {
                    ConsoleToggle();
                } else {
                    Input::keyPressed(event.key.keysym.sym);
                }

            }
            break;
        case SDL_JOYAXISMOTION:
            break;
        case SDL_JOYBUTTONDOWN:
            break;
        case SDL_MOUSEMOTION:
            // manipulate player
            player->look(event.motion.yrel, -event.motion.xrel);
            break;
        case SDL_VIDEORESIZE:
            center_x = event.resize.w / 2;
            center_y = event.resize.h / 2;
            initRenderer(event.resize.w, event.resize.h, 32, FULLSCREEN);
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

void Engine::render() {
    this->render3D();
    this->render2D();

    /*
    [self updateMenuBar:fps :blocksRendered];

    */

    SDL_GL_SwapBuffers();
}

void Engine::render3D() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    GLfloat lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f};
    GLfloat light_diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light_full_on[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat white[]    = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_full_on);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_EMISSION, light_specular);

    glViewport (0, 0, view_width, view_height);
    glDepthFunc (GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    //Culling and shading
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK);

    // render 3D stuff
    player->render();
    world->render();
}

void Engine::render2D() {
    // render 2D stuff
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, view_width, view_height, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();

    glLoadIdentity();

    crosshair->render();

    if(CVarUtils::GetCVar<bool>("player.info")) {
        this->renderFPS();
        this->renderPlayerPosition();
        this->renderPlayerDirection();
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    ConsoleRender();

}

void Engine::renderFPS() {
    std::ostringstream s;
    s << "FPS: " << fps_current;
    TextWrite(view_width / 2 - 40, 13, s.str().c_str());
}

void Engine::renderPlayerPosition() {
    char s[96];
    Vec3 pos = player->getPosition();
    snprintf(s, 95, "loc: %0.2f, %0.2f, %0.2f", pos.x, pos.y, pos.z);
    TextWrite(view_width / 2 - 40, 26, s);
}

void Engine::renderPlayerDirection() {
    char s[96];
    Vec3 angle = player->getDirection();

    snprintf(s, 95, "ang: %0.2f, %0.2f, %0.2f, facing %s\n",
            angle.x, angle.y, angle.z, player->getDirectionAsString());
    TextWrite(view_width / 2 - 40, 39, s);
}


float Engine::elapsedSeconds() {
    return elapsed_seconds;
}

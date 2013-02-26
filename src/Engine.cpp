#include "minecrap.h"
#include "Engine.h"
#include "World.h"
#include "Player.h"
#include "Crosshair.h"
#include "Console.h"
#include "Text.h"
#include "Input.h"
#include "Graphics.h"

#include <sstream>
#define FPS_INTERVAL     1.0f

static float elapsed_seconds = 0.0f;

Engine::Engine() : fps_current(0),quit(false) {
    Graphics::Init();

    init();
    world = new World(CVarUtils::GetCVar<int>("seed"));
    player = world->spawnPlayer();
    crosshair = new Crosshair(graphics->viewWidth(), graphics->viewHeight());
}

Engine::~Engine() {
    delete this->world;
    delete this->player;
    delete this->crosshair;
    Graphics::Cleanup();
}


void Engine::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_JOYSTICK) != 0) {
        return;
    }
    SDL_WM_SetCaption("","");
    graphics->initRenderer(800, 600, 32, FULLSCREEN);

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


void Engine::stop() {
    this->quit = true;
}

long Engine::tick() {
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
                    case SDLK_F3:
                        graphics->toggleRenderingAsWireframe();
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
              int w = graphics->viewWidth();
              int h = graphics->viewHeight();

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
            graphics->initRenderer(event.resize.w, event.resize.h, 32, FULLSCREEN);
            delete crosshair;
            crosshair = new Crosshair(event.resize.w, event.resize.h);
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
    // 3D stuff
    graphics->begin3D();

    player->render();
    graphics->setCameraFromPlayer(player);
    graphics->updateFrustum();

    blocksRendered = world->render();
    graphics->end3D();

    // 2D stuff
    graphics->begin2D();
    crosshair->render();
    this->renderFPS();
    this->renderPlayerPosition();
    this->renderPlayerDirection();
    this->renderRenderStats();
    graphics->end2D();

    ConsoleRender();

    graphics->flush();

	GLenum error;
	while (GL_NO_ERROR != (error=glGetError())) {
		std::string s = reinterpret_cast<const char *>(gluErrorString(error));
		printf( "Error: %s\n", s.c_str());
        exit(1);
	}

}

void Engine::renderFPS() {
    std::ostringstream s;
    s << "FPS: " << fps_current;
    TextWrite(graphics->viewWidth() / 2 - 40, 13, s.str().c_str());
}

void Engine::renderPlayerPosition() {
    char s[96];
    Vec3 pos = player->getPosition();
    snprintf(s, 95, "loc: %0.2f, %0.2f, %0.2f", pos.x, pos.y, pos.z);
    TextWrite(graphics->viewWidth() / 2 - 40, 26, s);
}

void Engine::renderPlayerDirection() {
    char s[96];
    Vec3 angle = player->getDirection();

    snprintf(s, 95, "ang: %0.2f, %0.2f, %0.2f, facing %s\n",
            angle.x, angle.y, angle.z, player->getDirectionAsString());
    TextWrite(graphics->viewWidth() / 2 - 40, 39, s);
}

void Engine::renderRenderStats() {
    char s[96];
    snprintf(s, 95, "Blocks: %d\n", blocksRendered);
    TextWrite(graphics->viewWidth() / 2 - 40, 52, s);
}

float Engine::elapsedSeconds() {
    return elapsed_seconds;
}

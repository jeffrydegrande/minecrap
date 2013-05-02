#include "minecrap.h"
#include "Engine.h"
#include "World.h"
#include "Player.h"
#include "Crosshair.h"
#include "Console.h"
#include "Input.h"
#include "Shader.h"
#include "MatrixStack.h"
#include "Image.h"

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

// static float elapsed_seconds = 0.0f;

static SDL_Window *window = 0;
static SDL_GLContext context;

struct ProgramData {
    GLuint cameraToClipMatrix;
    GLuint directionToLight;
    GLuint modelToCameraMatrix;
    GLuint normalModelToCameraMatrix;
    GLuint lightIntensity;
    GLuint ambientLightIntensity;
    GLuint materials;
};

ProgramData basicShader;

Vec4 directionToLight(0.866f, 1.0f, 0.0f, 0.0f);

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
    paused(false),
    world(NULL),
    player(NULL),
    crosshair(NULL),
    shader(NULL),
    lightIntensity(0.8f, 0.8f, 0.8f, 0.8f),
    ambientLightIntensity(0.5f, 0.5f, 0.5f, 0.5f),
    night(false),
    optionRenderWireframe(false),
    optionLighting(true),
    optionDrawRayToLightSource(false)
{
    init();
}

Engine::~Engine() {

    delete this->world;
    delete this->player;
    // delete this->crosshair;

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Engine::init() {
    this->width  = WINDOW_WIDTH;
    this->height = WINDOW_HEIGHT;


    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Quit();
        return;
    }

    // set the opengl context version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    window = SDL_CreateWindow("Minecrap", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, width, height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Unable to create window");
        exit(1);
    }

    context = SDL_GL_CreateContext(window);

    printf("OpenGL Context created! \n ");

    SDL_GL_SetSwapInterval(1);

    CHECK_OPENGL_ERRORS(__LINE__);

#ifdef _WIN32
    GLenum error = glewInit();
    if (GLEW_OK != error) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
        exit(1);
    }
#endif


    displayOpenGLInfo();
    resizeWindow(width, height);

    compileShaders();
    loadTextures();

    SDL_ShowCursor (0);
	SDL_WarpMouseInWindow(window, width / 2, height / 2);

    /*
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        printf("Joystick found: %s\n", SDL_JoystickName(i));
        SDL_JoystickEventState(SDL_ENABLE);
        SDL_JoystickOpen(i);
    }
    */

    world = new World(CVarUtils::GetCVar<int>("seed"));
    player = world->spawnPlayer();
}

void Engine::resizeWindow(int width, int height) {
    printf("Setting window size: %d / %d\n", width, height);
    float aspect = (float)width / (float)height;

    this->width  = width;
    this->height = height;

	projection.Perspective(FOV, aspect, ZNEAR, ZFAR);

	// TODO: executing this here throws an error while it's
	//		 really the optimal place to do this. Investigate
    // shader->setCameraToClipMatrix(perspective);

    glViewport (0, 0, this->width, this->height);
    /* if (crosshair != NULL) { */
    /*     delete crosshair; */
    /* } */
    /* crosshair = new Crosshair(viewWidth(), viewHeight()); */

    CHECK_OPENGL_ERRORS(__LINE__);
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
        shader->addVertexShader("..\\shaders\\GLSL33\\hello_world.vert");
        shader->addFragmentShader("..\\shaders\\GLSL33\\hello_world.frag");
#endif

        shader->addVertexShader("shaders/GLSL33/hello_world.vert");
        shader->addFragmentShader("shaders/GLSL33/hello_world.frag");

        shader->done();

        // set program data
        basicShader.cameraToClipMatrix        = shader->getUniformLocation("cameraToClipMatrix");
        basicShader.directionToLight          = shader->getUniformLocation("directionToLight");
        basicShader.modelToCameraMatrix       = shader->getUniformLocation("modelToCameraMatrix");
        basicShader.normalModelToCameraMatrix = shader->getUniformLocation("normalModelToCameraMatrix");
        basicShader.lightIntensity            = shader->getUniformLocation("lightIntensity");
        basicShader.ambientLightIntensity     = shader->getUniformLocation("ambientLightIntensity");
        basicShader.materials                 = shader->getUniformLocation("materials");
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

    if (paused)
        return;

    player->update(elapsedInSeconds);
    world->update();

    /* osd->write("Current block: %s (%d)", */
    /*         player->getInventory()->getCurrentBlockName(), */
    /*         player->getInventory()->getCurrentBlock()); */

    /* osd->write("FPS: %d", fps_current); */
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
                case SDLK_F4:
                    toggleDayNight();
                    break;
                case SDLK_F5:
                    optionDrawRayToLightSource = !optionDrawRayToLightSource;
                    break;
                case SDLK_p:
                    paused = !paused;
                    break;
                default:
                    Input::keyPressed(event.key.keysym.sym);
            }
            break;
        case SDL_JOYAXISMOTION:
            Input::joystickSet(event.jaxis.axis, event.jaxis.value);
            {
                float x = Input::joystickGet(3) / (3 * 3276.8f);
                float y = Input::joystickGet(4) / (3 * 3276.8f);
                player->look(x, y);
            }
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
                SDL_WarpMouseInWindow(window, w / 2, h / 2);
              } else {
                player->look(event.motion.xrel, event.motion.yrel);
              }
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            /* printf("Mouse button %d pressed at (%d,%d)\n", */
            /*     event.button.button, event.button.x, event.button.y); */
            printf( "Mouse button down\n");
			switch(event.button.button) {
				case 1:
					world->addBlock(player);
					break;
				case 3:
					world->removeBlock(player);
					break;
                case 4:
                    player->getInventory()->setNextBlock();
                    break;
                case 5:
                    player->getInventory()->setPreviousBlock();
                    break;
			}
            break;
            /*
        case SDL_VIDEORESIZE:
            resizeWindow(event.resize.w, event.resize.h);
            break;
            */
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
    if (paused)
        return;

    if (night) {
        glClearColor(14.0f/255, 0.0f, 51.0f/255.0f, 1.0f);
        lightIntensity = Vec4(0.3f, 0.3f, 0.3f, 0.3f);
    } else {
        glClearColor(0.52f, 0.74f, 0.84f, 1.0f);
        lightIntensity = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render3D();
    render2D();
    flush();

    CHECK_OPENGL_ERRORS(__LINE__);
}

void Engine::loadTextures()
{
    printf("Loading textures\n");

#ifndef HAVE_APPLE_OPENGL_FRAMEWORK

    Image_Init();

    std::vector<Image *> images;

    images.push_back(new Image("./bricks/grass_side.jpg"));
    images.push_back(new Image("./bricks/grass_top.jpg"));
    images.push_back(new Image("./bricks/water.jpg"));
    images.push_back(new Image("./bricks/sand.jpg"));
    images.push_back(new Image("./bricks/stone.jpg"));
    images.push_back(new Image("./bricks/bedrock.jpg"));
    images.push_back(new Image("./bricks/redstone_ore.jpg"));
    images.push_back(new Image("./bricks/lava.jpg"));

    glGenTextures(1, &blockTextureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, blockTextureArray);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //Always set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT  /*GL_CLAMP_TO_EDGE*/);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT /*GL_CLAMP_TO_EDGE*/);

    // create 

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, 128, 128, images.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    CHECK_OPENGL_ERRORS(__LINE__);

    for (size_t i=0; i<images.size(); i++)
    {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 128, 128, 1,
                GL_RGBA, GL_UNSIGNED_BYTE, images[i]->data_ptr());

        CHECK_OPENGL_ERRORS(__LINE__);
    }

    // glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	CHECK_OPENGL_ERRORS

    for (size_t i=0; i<images.size(); i++) {
        delete images[i];
    }
#endif
}


void Engine::render3D() {
    CHECK_OPENGL_ERRORS(__LINE__);
    glPolygonMode( GL_FRONT_AND_BACK, optionRenderWireframe ? GL_LINE : GL_FILL );

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_DEPTH_CLAMP);

#ifndef HAVE_APPLE_OPENGL_FRAMEWORK
    glBindTexture(GL_TEXTURE_2D_ARRAY, blockTextureArray);
#endif

	UseShader use(*shader);

	Matrix4 cameraToClipMatrix = projection.top();
    MatrixStack model;
    Matrix4 camera = player->getCameraMatrix();
    model.apply(camera);

    Vec4 cameraDirectionToLight = camera * directionToLight;
    Matrix3 normal(camera);

    shader->setUniformMatrix4(basicShader.cameraToClipMatrix, cameraToClipMatrix);
    shader->setUniformMatrix3(basicShader.normalModelToCameraMatrix, normal);
    shader->setDirectionToLight(cameraDirectionToLight);
    shader->setUniformMatrix4(basicShader.modelToCameraMatrix, camera);
    shader->setUniformVec4(basicShader.lightIntensity, lightIntensity);
    shader->setUniformVec4(basicShader.ambientLightIntensity, ambientLightIntensity);
    shader->setUniform1i(basicShader.materials, 0);

    CHECK_OPENGL_ERRORS(__LINE__);

    world->render();

#if 0
    if (optionDrawRayToLightSource) {
        Vec4 point = Vec4(0.0f, 0.0f, 0.0f, 0.f) + (directionToLight * 10000);
        glLineWidth(3.0f);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(point.x, point.y, point.z);
        glEnd();
        glColor3f(1.0, 1.0, 1.0);
    }
#endif

    CHECK_OPENGL_ERRORS(__LINE__);
}

void Engine::render2D() {
#if 0
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, viewWidth(), viewHeight(), 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    // crosshair->render();
    // osd->render();

#ifdef SUPPORT_GLCONSOLE
    ConsoleRender();
#endif

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
#endif
}

int Engine::viewWidth() const {
    return width;
}

int Engine::viewHeight() const {
    return height;
}


void Engine::flush() {
    SDL_GL_SwapWindow(window);
}

/* void Engine::updateFrustum() { */
/*     return; */

/*     float p[16]; */
/*     float m[16]; */
/*     glGetFloatv(GL_PROJECTION_MATRIX, p); */
/*     glGetFloatv(GL_MODELVIEW_MATRIX, m); */

/*     Matrix4 P = p; */
/*     Matrix4 M = m; */
/*     Matrix4 A = M * P; */
/*     frustum.setFrustum(A.value_ptr()); */
/* } */

/* bool Engine::withinFrustum(float x, float y, float z, float radius) { */
/*     Vec3 point(x, y, z); */

/*     if ( point.y < cameraPosition.y - 4) */
/*         return false; */

/*     if (Frustum::OUTSIDE != frustum.sphereInFrustum(point, radius)) */
/*         return true; */
/*     else */
/*         return false; */
/* } */

void Engine::toggleDayNight()
{
    night = !night;
}


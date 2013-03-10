#include "Input.h"
#include <cstdio>
#include <cstdlib>
#include "minecrap.h"

#define MAX_KEYS 512
#define MAX_AXIS 6

static bool down[MAX_KEYS];
static float joystick[MAX_AXIS];

void Input::keyPressed(int key) {
    if (key < 0 || key >= MAX_KEYS)
        return;
    down[key] = true;
}

void Input::keyReleased(int key) {

    if (key < 0 || key >= MAX_KEYS)
        return;
    down[key] = false;
}

bool Input::isKeyPressed(int key) {
    if (key < 0 || key >= MAX_KEYS)
        return false;

    return down[key];
}

void Input::joystickSet(int axis, int value)
{
    if (axis > MAX_AXIS)
        printf( "extra axis!\n");

    if (axis < 0 || axis >= MAX_AXIS)
        return;

    joystick[axis] = (float)value; // / 32768.0f;
    if (abs(joystick[axis]) < 0.15f)
        joystick[axis] = 0.0f;
}

float Input::joystickGet(int axis)
{
    if (axis < 0 || axis >= MAX_AXIS)
        return 0.0f;

    return joystick[axis];
}

bool Input::isMovingForward()
{
    return (isKeyPressed(SDLK_w) || joystickGet(1) < -20000);
}

bool Input::isMovingBackward()
{
    return (isKeyPressed(SDLK_s) || joystickGet(1) > 20000);
}

bool Input::isStrafingLeft()
{
    return (isKeyPressed(SDLK_a) || joystickGet(0) < -20000);
}

bool Input::isStrafingRight()
{
    return (isKeyPressed(SDLK_d) || joystickGet(0) > 20000);
}

bool Input::isAscending()
{
    return (isKeyPressed(SDLK_q) || joystickGet(2) > 20000);
}

bool Input::isDescending()
{
    return (isKeyPressed(SDLK_e) || joystickGet(5) > 20000);
}

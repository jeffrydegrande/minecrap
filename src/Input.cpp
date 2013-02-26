#include "Input.h"
#include <cstdio>

#define MAX_KEYS 512

static bool down[MAX_KEYS];


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

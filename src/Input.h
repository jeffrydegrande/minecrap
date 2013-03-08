#ifndef MINECRAP_INPUT_H
#define MINECRAP_INPUT_H


class Input {

public:
    static void keyPressed(int key);
    static void keyReleased(int key);
    static bool isKeyPressed(int key);

    static void JoystickSet(int axis, int value);
    static float JoystickGet(int axis);
};

#endif

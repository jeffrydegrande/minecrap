#ifndef MINECRAP_INPUT_H
#define MINECRAP_INPUT_H


class Input {

public:
    static void keyPressed(int key);
    static void keyReleased(int key);
    static bool isKeyPressed(int key);

    static void joystickSet(int axis, int value);
    static float joystickGet(int axis);

    static bool isMovingForward();
    static bool isMovingBackward();
    static bool isStrafingLeft();
    static bool isStrafingRight();
    static bool isAscending();
    static bool isDescending();

};

#endif

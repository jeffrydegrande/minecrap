#include "minecrap.h"
#include "Player.h"
#include "Input.h"

#include "Vec2.h"
#include <math.h>
#include <CVars/CVar.h>

#define EYE_HEIGHT      1.75f

Player::Player() {
	this->setPosition(Vec3(0,0,0));
}

Player::Player(float x, float y, float z) {
	this->setPosition(Vec3(x, y, z));
}

Player::Player(const Vec3 & position) {
	this->setPosition(position);
}

Player::~Player(void)
{
}

const Vec3 Player::getPosition() const {
    return position;
}

const Vec3 Player::getDirection() const {
    return angle;
}

void Player::move(Vec3 delta) {
    Vec3 movement;
    float forward;

    if(CVarUtils::GetCVar<bool>("flying")) {
        forward = sin(angle.x * DEGREES_TO_RADIANS);
        movement.x = cos(angle.z * DEGREES_TO_RADIANS) * delta.x
                    + sin(angle.z * DEGREES_TO_RADIANS) * delta.y * forward;
        movement.y = -sin(angle.z * DEGREES_TO_RADIANS) * delta.x
                    + cos(angle.z * DEGREES_TO_RADIANS) * delta.y * forward;
        movement.z = cos (angle.x * DEGREES_TO_RADIANS) * delta.y;
        position += movement;
    } else {
        ConsoleLog("Not allowed to fly");
    }
}

void Player::strafeRight() {
    this->move(Vec3(1, 0, 0));
}

void Player::strafeLeft() {
    this->move(Vec3(-1, 0, 0));
}

void Player::moveForward() {
    this->move(Vec3(0, -1, 0));
}

void Player::moveBackward() {
    this->move(Vec3(0, 1, 0));
}

void Player::render() {
    glRotatef (camera_angle.x, 1.0f, 0.0f, 0.0f);
    glRotatef (camera_angle.y, 0.0f, 1.0f, 0.0f);
    glRotatef (camera_angle.z, 0.0f, 0.0f, 1.0f);
    glTranslatef (-camera_position.x, -camera_position.y, -camera_position.z);
}

void Player::update()
{
    // update movement
    if (Input::isKeyPressed(SDLK_w)) {
        moveForward();
    }
    if (Input::isKeyPressed(SDLK_s)) {
        moveBackward();
    }
    if (Input::isKeyPressed(SDLK_a)) {
        strafeLeft();
    }
    if (Input::isKeyPressed(SDLK_d)) {
        strafeRight();
    }

    this->updateCamera();
}

void Player::updateCamera() {
    camera_position = position;
    camera_angle = angle;
    camera_position.y += EYE_HEIGHT;
}

void Player::look(int x, int y) {
  float   mouse_sense;
  // avoid getting the very first update throwing
  // the player completely off
  if (abs(x) > 200 && abs(y) > 200)
      return;

  x = - x;
  mouse_sense = 1.0f;
  angle.x -= (float)x * mouse_sense;
  // angle.x = clamp (angle.x, -90.0f, 90.0f);

  angle.y += (float)y * mouse_sense;
  angle.y = fmod (angle.y, 360.0f);
  if (angle.y < 0.0f)
    angle.y += 360.0f;
}

void Player::setPosition(const Vec3 &position) {
	this->position = position;
	this->angle = Vec3(180.0f, 0, 180.0f );

    updateCamera();
}

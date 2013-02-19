#include "minecrap.h"
#include "Player.h"
#include "Input.h"
#include "Engine.h"
#include "World.h"

#include "Vec2.h"
#include <math.h>
#include <CVars/CVar.h>

#define GRAVITY         5.5f
#define EYE_HEIGHT      1.75f
#define JUMP_SPEED      4.0f
#define SPEED			5.0f

Player::Player(World *world, const Vec3 & position) {
    this->world = world;
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

const char *Player::getDirectionAsString() {
    float angle = this->angle.y;

    if (angle < 22.5f || angle >= 337.5f)
        return "N";
    else if (angle < 67.5f)
        return "NE";
    else if (angle < 112.5f)
        return "E";
    else if (angle < 157.5f)
        return "SE";
    else if (angle < 202.5f)
        return "S";
    else if (angle < 247.5f)
        return "SW";
    else if (angle < 292.5f)
        return "W";
    else if (angle < 337.5f)
        return "NW";

    return "Unknown";
}

void Player::strafeRight() {
	Vec3 right;
	camera_matrix.invertPt(Vec3(1.0f, 0.0f, 0.0f), right);
	position += right;
}

void Player::strafeLeft() {
	Vec3 right;
	camera_matrix.invertPt(Vec3(-1.0f, 0.0f, 0.0f), right);
	position += right;
}

void Player::moveForward() {
	Vec3 head;
	camera_matrix.invertPt(Vec3(0.0f, 0.0f, -1.0f), head);
	position += head * SPEED;
}

void Player::moveBackward() {
	Vec3 head, right;
	camera_matrix.invertPt(Vec3(0.0f, 0.0f, 1.0f), head);
	position += head * SPEED;
}

void Player::render() {
    glRotatef (angle.x, 1.0f, 0.0f, 0.0f);
    glRotatef (angle.y, 0.0f, 1.0f, 0.0f);
    glRotatef (angle.z, 0.0f, 0.0f, 1.0f);
    glTranslatef (-position.x, -position.y, -position.z);
}

void Player::update()
{
    float elapsed = std::min(Engine::elapsedSeconds(), 0.25f);
    bool flying = CVarUtils::GetCVar<bool>("flying");

	camera_matrix.loadIdentity();
	camera_matrix.rotateX(angle.x);
	camera_matrix.rotateY(angle.y);
	camera_matrix.rotateZ(angle.z);

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

    if (Input::isKeyPressed(SDLK_SPACE) && onGround) {
        velocity = JUMP_SPEED;
        onGround = false;
    }

    float ground = position.y;

    // apply gravity
    if (!flying) {
        velocity   -= GRAVITY * elapsed;
        position.y += velocity * elapsed;
    }

    if (world->isGround(position.x, position.y, position.z)) {
        velocity = 0.0f;
        position.y = ground;
        onGround = true;
    } else {
        onGround = false;
    }

    // TODO:
    // * acceleration
    // * jumping
    // * collision detecting
    // * gravity
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
  angle.x = clamp (angle.x, -90.0f, 90.0f);

  angle.y -= (float)y * mouse_sense;
  if (angle.y < -180.0f)
	  angle.y += 360.0f;
  if (angle.y > 180.0f)
	  angle.y -= 360.0f;
}

void Player::setPosition(const Vec3 &position) {
	this->position = position;
	this->angle = Vec3(0.0f, 0, 0.0f );
    this->velocity = 0;
    this->onGround = true;
}

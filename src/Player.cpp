#include "minecrap.h"
#include "Player.h"
#include "Input.h"
#include "Engine.h"
#include "World.h"
#include "Text.h"

#include "Vec2.h"
#include <math.h>
#include <CVars/CVar.h>

#define GRAVITY         9.2f
#define EYE_HEIGHT      1.75f
#define JUMP_SPEED      5.5f
#define SPEED		    25.0f

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

void Player::render() {
	angle.normalize();
	right = angle * up;
	right.normalize();

	up = right * angle;
	up.normalize();

    float v[16];

	v[0] =  right.x;
	v[1] =  up.x;
	v[2] = -angle.x;
	v[3] =  0.0f;

	v[4] =  right.y;
	v[5] =  up.y;
	v[6] = -angle.y;
	v[7] =  0.0f;

	v[8]  =  right.z;
	v[9]  =  up.z;
	v[10] = -angle.z;
	v[11] =  0.0f;

	v[12] = -right.dotProduct(position);
	v[13] = -up.dotProduct(position);
	v[14] =  angle.dotProduct(position);
	v[15] =  1.0f;

	glMultMatrixf(v);
}

void Player::look(int x, int y) {
    float mouse_sense = 0.3f;

    // avoid getting the very first update throwing
    // the player completely off
    if (abs(x) > 200 || abs(y) > 200)
      return;

    // Matrix4 camera;
    camera.loadIdentity();

    // rotate on the x-axis
    camera.rotate((float)x * mouse_sense, right);
    camera.transformVector(angle);
    camera.transformVector(up);

    // rotate on the y-axis
    Vec3 u(0.0f, 1.0f, 0.0f);
    camera.rotate((float)y * mouse_sense, u);
    camera.transformVector(angle);
    camera.transformVector(up);


  // TODO: how to clamp rotations on X&Z & how to not rotate on Z at all
  // rotate on the y-axis
  /*
  x = - x;
  mouse_sense = 0.8f;
  angle.x -= (float)x * mouse_sense;
  angle.x = clamp (angle.x, -90.0f, 90.0f);

  angle.y -= (float)y * mouse_sense;
  if (angle.y < -180.0f)
	  angle.y += 360.0f;
  if (angle.y > 180.0f)
	  angle.y -= 360.0f;
  */

}

void Player::update()
{
    float elapsed = (std::min)(Engine::elapsedSeconds(), 0.25f);
    bool flying = CVarUtils::GetCVar<bool>("flying");

    // update movement
    if (Input::isKeyPressed(SDLK_w)) { 
        position += angle * SPEED * elapsed;
    }

    if (Input::isKeyPressed(SDLK_s)) { 
        position -= angle * SPEED * elapsed;
    }
    if (Input::isKeyPressed(SDLK_a)) { 
        position -= right * SPEED * elapsed;
    }
    if (Input::isKeyPressed(SDLK_d)) { 
        position += right * SPEED * elapsed;
    }

    // jumping
    if (Input::isKeyPressed(SDLK_SPACE) && onGround) {
        velocity = JUMP_SPEED;
        onGround = false;
    }

    // apply gravity
    if (!flying) {
        float ground = position.y;
        velocity   -= GRAVITY * elapsed;
        position.y += velocity * elapsed;

        if (position.y < 0 || world->isGround((int)position.x, (int)position.y - 1, (int)position.z)) {
            position.y = ground;
            velocity = 0.0f;
            onGround = true;
        } else {
            onGround = false;
        }
    }
}

void Player::setPosition(const Vec3 &position) {
	this->position = position;
	this->angle = Vec3(-13.0f, 180.0f, 0.0f );
    this->up    = Vec3(0.0f, 1.0f, 0.0f);
    this->right = Vec3(1.0f, 0.0f, 0.0f);
    this->velocity = 0;
    this->onGround = true;
}

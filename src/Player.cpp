#include "minecrap.h"
#include "Player.h"
#include "Input.h"
#include "Engine.h"
#include "World.h"
#include "Text.h"
#include "OSD.h"

#include "Vec.h"
#include <math.h>
#include <CVars/CVar.h>

#define GRAVITY         9.2f
#define EYE_HEIGHT      1.75f
#define JUMP_SPEED      5.5f
#define SPEED		    8.0f

#define ACCELERATION Vec3(8.0f, 8.0f, 8.0f)
#define VELOCITY Vec3(8.0f, 8.0f, 8.0f)

Player::Player(World *world, const Vec3 & position) {
    this->world = world;
	this->setPosition(position);

    camera.setAcceleration(ACCELERATION);
    camera.setVelocity(VELOCITY);
}

Player::~Player(void)
{
}

const Vec3 Player::getPosition() const {
	return camera.getPosition();
}

const Vec3 Player::getDirection() const {
	return camera.getViewDirection();
}

const char *Player::getDirectionAsString() {
    float angle = camera.getViewDirectionInDegrees();

    if (angle < 22.5f || angle >= 337.5f)
        return "S";
    else if (angle < 67.5f)
        return "SW";
    else if (angle < 112.5f)
        return "W";
    else if (angle < 157.5f)
        return "NW";
    else if (angle < 202.5f)
        return "N";
    else if (angle < 247.5f)
        return "NE";
    else if (angle < 292.5f)
        return "E";
    else if (angle < 337.5f)
        return "SE";

    return "Unknown";
}

void Player::look(int x, int y) {
    bool flying = CVarUtils::GetCVar<bool>("flying");
    float mouse_sense = 0.5f;

	if (flying) {
		float heading = -direction.x * 100.0f * 0.05f;
		float pitch  = -y * mouse_sense;
		float roll = -x * mouse_sense;

		camera.rotate(heading, pitch, roll);
	} else {
		float heading = -x * mouse_sense;
		float pitch   = -y * mouse_sense;
		camera.rotate(heading, pitch, 0.0f);
	}
}

void Player::update(float elapsed)
{
    bool flying = CVarUtils::GetCVar<bool>("flying");

	static bool movingForward  = false;
	static bool movingBackward = false;
	static bool movingLeft     = false;
	static bool movingRight    = false;
	static bool movingUp	   = false;
	static bool movingDown     = false;

    direction.set(0.0f, 0.0f, 0.0f);
    Vec3 cameraVelocity = camera.getCurrentVelocity();

    // update movement from keyboard
    if (Input::isMovingForward()) {
		if (!movingForward) {
			movingForward = true;
			camera.setCurrentVelocity(cameraVelocity.x, cameraVelocity.y, 0.0f);
		}
        direction.z += 1.0f;
    } else {
		movingForward = false;
	}

    if (Input::isMovingBackward()) {
		if (!movingBackward) {
			movingBackward = true;
			camera.setCurrentVelocity(cameraVelocity.x, cameraVelocity.y, 0.0f);
		}
		direction.z -= 1.0f;
    } else {
		movingBackward = false;
	}

    if (Input::isStrafingRight()) {
		if (!movingRight) {
			movingRight = true;
			camera.setCurrentVelocity(0.0f, cameraVelocity.y, cameraVelocity.z);
		}
        direction.x += 1.0f;
    } else {
		movingRight = false;
	}

    if (Input::isStrafingLeft()) {
		if (!movingLeft) {
			movingLeft = true;
			camera.setCurrentVelocity(0.0f, cameraVelocity.y, cameraVelocity.z);
		}
        direction.x -= 1.0f;
    } else {
		movingLeft = false;
	}

    if (Input::isAscending()) {
        if (!movingUp) {
            movingUp = true;
            camera.setCurrentVelocity(cameraVelocity.x, 0.0f, cameraVelocity.z);
        }
        direction.y += 1.0f;
    } else{
        movingUp = false;
    }

    if (Input::isDescending()) {
        if (!movingDown) {
            movingDown = true;
            camera.setCurrentVelocity(cameraVelocity.x, 0.0f, cameraVelocity.z);
        }
        direction.y -= 1.0f;
    } else {
        movingDown = false;
    }

    camera.updatePosition(direction, elapsed);

    // jumping
    if (Input::isKeyPressed(SDLK_SPACE) && onGround) {
        velocity = JUMP_SPEED;
        onGround = false;
    }


    // apply gravity
    if (!flying) {
#if 0
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
#endif
    }
}

void Player::setPosition(const Vec3 &position) {
    this->onGround = true;
    camera.setViewDirection(Vec3(0.0, 0.0f, -1.0f));
    camera.setPosition(position);
}

const Matrix4 & Player::getCameraMatrix() {
    return camera.getViewMatrix();
}

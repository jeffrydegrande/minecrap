#include "minecrap.h"
#include "Player.h"
#include "Input.h"
#include "Engine.h"
#include "World.h"
#include "Text.h"

#include "Vec.h"
#include <math.h>
#include <CVars/CVar.h>

#define GRAVITY         9.2f
#define JUMP_SPEED      6.0f
#define SPEED		    4.0f
#define EYE_HEIGHT      1.72f

#define ACCELERATION  Vec3(8.0f, 8.0f, 8.0f)
#define VELOCITY      Vec3(4.0f, 4.0f, 4.0f)
#define WALKING_SPEED Vec3(4.0f, 4.0f, 4.0f)
#define RUNNING_SPEED Vec3(12.0f, 12.0f, 12.0f)

#define EPSILON 1e-6f;

bool closeEnough(float a, float b) {
    return fabsf((a - b) / ((b == 0.0f) ? 1.0f : b)) < EPSILON;
}

Player::Player(World *world, const Vec3 & position) {
    this->world = world;
	this->setPosition(position);
    this->onGround = true;
    this->inventory = new Inventory();

    distanceTraveled = 0.0f;
    speed = 0;

    setAcceleration(ACCELERATION);
    setVelocity(VELOCITY);
    currentVelocity.set(0.0f, 0.0f, 0.0f);
}

Player::~Player(void)
{
}

void Player::setPosition(const Vec3 &position) {
    currentPosition = position;

    Vec3 cameraPosition(currentPosition);
    cameraPosition.y += EYE_HEIGHT;
    camera.setViewDirection(Vec3(0.0, 0.0f, -1.0f));
    camera.setPosition(cameraPosition);
}

bool Player::checkCollision(const Vec3 &pos) {

    if (CVarUtils::GetCVar<bool>("noclip"))
        return true;

    Vec3 head(pos.x, (int)pos.y+1, pos.z);
    Vec3 feet(pos.x, (int)pos.y, pos.z);

    GLubyte hb = world->blockAt(head);
    GLubyte hf = world->blockAt(feet);

    // osd->write("head: %d\n", hb);
    // osd->write("feet: %d\n", hf);

    return (hb == AIR && hf == AIR);
}

void Player::update(float elapsed)
{
    calculateMovementDirection(elapsed);
    move(direction, elapsed);


    /* osd->write("LOC: %0.2f, %0.2f, %0.2f", currentPosition.x, currentPosition.y, currentPosition.z); */
    /* osd->write("Facing %s (%0.2f)", getDirectionAsString(), getDirectionInDegrees()); */
    /* osd->write("Distance Traveled: %0.2fm (%0.2f km/h)\n", */ 
    /*         getDistanceTraveled(), getSpeed() / 1000); */

    /* if (onGround) */
    /*     osd->write("On Ground: yes"); */
    /* else */
    /*     osd->write("On Ground: no"); */
}

void Player::look(int x, int y) {
    // avoid mouse pointer warp throwing off the camera
    if (abs(x) > 200 || abs(y) > 200)
        return;

    bool flying = CVarUtils::GetCVar<bool>("flying");
    float mouse_sense = 0.5f;

	if (flying) {
		float heading = -direction.x * 100.0f * 0.05f;
		float pitch   = -y * mouse_sense;
		float roll    = -x * mouse_sense;

		camera.rotate(heading, pitch, roll);
	} else {
		float heading = -x * mouse_sense;
		float pitch   = -y * mouse_sense;
		camera.rotate(heading, pitch, 0.0f);
	}
}

void Player::calculateMovementDirection(float elapsed)
{
	static bool movingForward  = false;
	static bool movingBackward = false;
	static bool movingLeft     = false;
	static bool movingRight    = false;
	static bool movingUp	   = false;
	static bool movingDown     = false;

    (void)elapsed;
    direction.set(0.0f, 0.0f, 0.0f);

    // update movement from keyboard
    if (Input::isMovingForward()) {
		if (!movingForward) {
			movingForward = true;
			setCurrentVelocity(currentVelocity.x, currentVelocity.y, 0.0f);
		}
        direction.z += 1.0f;
    } else {
		movingForward = false;
	}

    if (Input::isMovingBackward()) {
		if (!movingBackward) {
			movingBackward = true;
			setCurrentVelocity(currentVelocity.x, currentVelocity.y, 0.0f);
		}
		direction.z -= 1.0f;
    } else {
		movingBackward = false;
	}

    if (Input::isStrafingRight()) {
		if (!movingRight) {
			movingRight = true;
			setCurrentVelocity(0.0f, currentVelocity.y, currentVelocity.z);
		}
        direction.x += 1.0f;
    } else {
		movingRight = false;
	}

    if (Input::isStrafingLeft()) {
		if (!movingLeft) {
			movingLeft = true;
			setCurrentVelocity(0.0f, currentVelocity.y, currentVelocity.z);
		}
        direction.x -= 1.0f;
    } else {
		movingLeft = false;
	}

    if (CVarUtils::GetCVar<bool>("flying")) {
        if (Input::isAscending()) {
            if (!movingUp) {
                movingUp = true;
                setCurrentVelocity(currentVelocity.x, 0.0f, currentVelocity.z);
            }
            direction.y += 1.0f;
        } else{
            movingUp = false;
        }

        if (Input::isDescending()) {
            if (!movingDown) {
                movingDown = true;
                setCurrentVelocity(currentVelocity.x, 0.0f, currentVelocity.z);
            }
            direction.y -= 1.0f;
        } else {
            movingDown = false;
        }
    } else {
        // jumping
        if (Input::isKeyPressed(SDLK_SPACE) && onGround) {
            onGround = false;
            setCurrentVelocity(currentVelocity.x, JUMP_SPEED, currentVelocity.z);
            direction.y += 1.0f;
        }
    }

    if (Input::isKeyPressed(SDLK_LSHIFT)) {
        setVelocity(RUNNING_SPEED);
    } else {
        setVelocity(WALKING_SPEED);
    }
}


void Player::move(const Vec3 &direction, float elapsed)
{
    float distance = 0.0f;
    if (currentVelocity.lengthSq() != 0.0f) {
        Vec3 displacement = (currentVelocity * elapsed) 
            + (acceleration * 0.5f * elapsed * elapsed);

        if (direction.x == 0.0f && closeEnough(currentVelocity.x, 0.0f))
            displacement.x = 0.0f;

        if (direction.y == 0.0f && closeEnough(currentVelocity.y, 0.0f))
            displacement.y = 0.0f;

        if (direction.z == 0.0f && closeEnough(currentVelocity.z, 0.0f))
            displacement.z = 0.0f;

        distance = displacement.length();

        Vec3 newPosition = camera.move(displacement);

        if (checkCollision(newPosition)) {
            camera.setPosition(newPosition);
            currentPosition = newPosition;
            currentPosition.y -= EYE_HEIGHT;

            distanceTraveled += distance;
        }
    }

    float elapsedInSeconds = elapsed / 1000.0f;

    speed = 1.0f / elapsedInSeconds * distance; // seconds
    if (closeEnough(elapsedInSeconds, 0.0f)) {
        speed = 0.0f;
    }
    updateVelocity(direction, elapsed);
}

void Player::updateVelocity(const Vec3 &direction, float elapsed)
{
    // Updates the camera's velocity based on the supplied movement direction
    // and the elapsed time (since this method was last called). The movement
    // direction is in the range [-1,1].

    if (direction.x != 0.0f)
    {
        // Camera is moving along the x axis.
        // Linearly accelerate up to the camera's max speed.

        currentVelocity.x += direction.x * acceleration.x * elapsed;
        currentVelocity.x = clamp(currentVelocity.x, -velocity.x, velocity.x);
    }
    else
    {
        // Camera is no longer moving along the x axis.
        // Linearly decelerate back to stationary state.

        if (currentVelocity.x > 0.0f) {
            if ((currentVelocity.x -= acceleration.x * elapsed) < 0.0f)
                currentVelocity.x = 0.0f;
        } else {
            if ((currentVelocity.x += acceleration.x * elapsed) > 0.0f)
                currentVelocity.x = 0.0f;
        }
    }

    if (CVarUtils::GetCVar<bool>("flying")) {
        if (direction.y != 0.0f) {
            // Camera is moving along the y axis.
            // Linearly accelerate up to the camera's max speed.
            currentVelocity.y += direction.y * acceleration.y * elapsed;
            currentVelocity.y = clamp(currentVelocity.y, -velocity.y, velocity.y);
        } else {
            // Camera is no longer moving along the y axis.
            // Linearly decelerate back to stationary state.
            if (currentVelocity.y > 0.0f) {
                if ((currentVelocity.y -= acceleration.y * elapsed) < 0.0f)
                    currentVelocity.y = 0.0f;
            } else {
                if ((currentVelocity.y += acceleration.y * elapsed) > 0.0f)
                    currentVelocity.y = 0.0f;
            }
        }
    } else {
        onGround = world->isGround(currentPosition);

        if (!onGround) {
            // according to wikipedia 195km/h is the terminal velocity of a skydiver.
            if (currentVelocity.y > -195.0f) {
                currentVelocity.y -= GRAVITY * elapsed;
            }
        } else {
            currentVelocity.y = 0;
        }
    }

    if (direction.z != 0.0f) {
        // Camera is moving along the z axis.
        // Linearly accelerate up to the camera's max speed.
        currentVelocity.z += direction.z * acceleration.z * elapsed;
        currentVelocity.z = clamp(currentVelocity.z, -velocity.z, velocity.z);
    } else {
        // Camera is no longer moving along the z axis.
        // Linearly decelerate back to stationary state.
        if (currentVelocity.z > 0.0f) {
            if ((currentVelocity.z -= acceleration.z * elapsed) < 0.0f)
                currentVelocity.z = 0.0f;
        } else {
            if ((currentVelocity.z += acceleration.z * elapsed) > 0.0f)
                currentVelocity.z = 0.0f;
        }
    }
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

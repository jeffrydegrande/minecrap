#include "minecrap.h"
#include "Player.h"

#include "Vec2.h"
#include <math.h>

#define EYE_HEIGHT      1.75f
#define CAM_MIN         1
#define CAM_MAX         12


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


void Player::move(Vec3 delta) {
    Vec3 movement;
    float forward;

    forward = sin (angle.x * DEGREES_TO_RADIANS);
    movement.x = cos (angle.z * DEGREES_TO_RADIANS) * delta.x +  sin (angle.z * DEGREES_TO_RADIANS) * delta.y * forward;
    movement.y = -sin (angle.z * DEGREES_TO_RADIANS) * delta.x +  cos (angle.z * DEGREES_TO_RADIANS) * delta.y * forward;
    movement.z = cos (angle.x * DEGREES_TO_RADIANS) * delta.y;
    position += movement;
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
    this->updateCamera();
}

void Player::updateCamera ()
{
  Vec3  cam;
  float     vert_delta;
  float     horz_delta;
  float radsx;

  radsx = angle.x * DEGREES_TO_RADIANS;
  vert_delta = cos (radsx) * cam_distance;
  horz_delta = sin (radsx);

  cam = position;
  cam.z += EYE_HEIGHT;
  
  cam.x += sin (angle.z * DEGREES_TO_RADIANS) * cam_distance * horz_delta;
  cam.y += cos (angle.z * DEGREES_TO_RADIANS) * cam_distance * horz_delta;
  cam.z += vert_delta;

  //ground = CacheElevation (cam.x, cam.y) + 0.2f;
  //cam.z = max (cam.z, ground);
  
  camera_angle = angle;
  camera_position = cam;

}

void Player::look(int x, int y) {
  float   mouse_sense;
  x = - x;
  mouse_sense = 1.0f;
  angle.x -= (float)x * mouse_sense;
  angle.z += (float)y * mouse_sense;

  // angle.x = clamp (angle.x, 0.0f, 180.0f);
 /*
  if (angle.x < 0.0f)
      angle.x = 0.0f;
  if (angle.x > 180.0f)

	  angle.x = 180.0f;
	  */

  angle.z = fmod (angle.z, 360.0f);
  if (angle.z < 0.0f)
    angle.z += 360.0f;
}

void Player::setPosition(const Vec3 &position) {
	this->position = position;
	this->angle = Vec3(90.0f, 0, 0);

	this->cam_distance = 1;
	this->camera_position = position;
	this->camera_angle = angle;
}

/*
- (NSString *) stringFromDirection {    
    int absyrot = yrot;
    
    if (absyrot < 0)
        absyrot += 360;
    
    if (absyrot >= 0 && absyrot < 45) {
        return @"N";
    } else if (absyrot >= 45 && absyrot < 90) {
        return @"NE";
    } else if (absyrot >= 90 && absyrot < 135) {
        return @"E";
    } else if (absyrot >= 135 && absyrot < 180) {
        return @"SE";
    } else if (absyrot >= 180 && absyrot < 225) {
        return @"S";
    } else if (absyrot >= 225 && absyrot < 270) {
        return @"SW";
    } else if (absyrot >= 270 && absyrot < 315) {
        return @"W";
    } else if (absyrot >= 315 && yrot < 360) {
        return @"NW";
    } else {
        return @"Unknown";
    }
}


- (NSString *) stringFromPosition {
    return
    [NSString stringWithFormat:@"COORDS: x=%0.2f, y=%0.2f,z=%0.2f, xrot=%0.3f, yrot=%0.2f, facing %@",
     xpos,
     ypos,
     zpos,
     xrot,
     yrot,
     [self stringFromDirection]];
}
*/

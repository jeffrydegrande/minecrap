//
//  Player.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "Vec.h"
#include "Matrix.h"
#include "Camera.h"

class World;
class Player
{
    Camera camera;
    World *world;

    Vec3 direction;
    Vec3 velocity;
    Vec3 acceleration;
    Vec3 currentVelocity;
    bool onGround;

    float distanceTraveled;
    float speed;
    float accumPitchDegrees;


public:
	Player(World *world, const Vec3 & position);
	~Player(void);

	void update(float elapsed);
	void look(int x, int y);

	void setPosition(const Vec3 &);

    const Vec3 getPosition() const;
    const Vec3 getDirection() const;
    const char *getDirectionAsString();
    float getDirectionInDegrees() const;
    const Matrix4 & getCameraMatrix() const;
    void setCurrentVelocity(const Vec3 &velocity);
    void setCurrentVelocity(float x, float y, float z);
    const Vec3 &getCurrentVelocity() const;
    const float &getDistanceTraveled() const;
    const float &getSpeed() const;


    void setAcceleration(const Vec3 &acceleration);
    const Vec3 &getAcceleration() const;

    void setVelocity(const Vec3 &velocity);
    const Vec3 &getVelocity() const;

private:
    void calculateMovementDirection();
    void updatePosition(const Vec3 &direction, float elapsed);
    void updateVelocity(const Vec3 &direction, float elapsed);

};

inline const float &Player::getDistanceTraveled() const
{ return distanceTraveled; }

inline float Player::getDirectionInDegrees() const
{ return camera.getViewDirectionInDegrees(); }

inline const Vec3 Player::getPosition() const 
{ return camera.getPosition(); }

inline const Vec3 Player::getDirection() const 
{ return camera.getViewDirection(); }


inline const float &Player::getSpeed() const
{ return this->speed; }

inline void Player::setCurrentVelocity(const Vec3 &velocity) 
{ this->currentVelocity = velocity; }

inline void Player::setCurrentVelocity(float x, float y, float z)
{ this->currentVelocity.set(x, y, z); }

inline const Vec3 &Player::getCurrentVelocity() const 
{ return this->currentVelocity; }

inline void Player::setAcceleration(const Vec3 &acceleration) 
{ this->acceleration = acceleration; }

inline const Vec3 &Player::getAcceleration() const 
{ return this->acceleration; }

inline void Player::setVelocity(const Vec3 &velocity) 
{ this->velocity = velocity; }

inline const Vec3 &Player::getVelocity() const 
{ return this->velocity; }

inline const Matrix4 & Player::getCameraMatrix() const
{ return camera.getViewMatrix(); }


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
    Vec3 direction;
    Camera camera;
    World *world;

    float velocity;
    bool onGround;

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
    const float &getDistanceTraveled() const;
    const Matrix4 & getCameraMatrix();

    const float &getSpeed() const;
};

inline const float &Player::getDistanceTraveled() const
{ return camera.getDistanceTraveled(); }

inline float Player::getDirectionInDegrees() const
{ return camera.getViewDirectionInDegrees(); }

inline const float &Player::getSpeed() const
{ return camera.getSpeed(); }

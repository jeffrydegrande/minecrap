//
//  Player.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "Vec.h"
#include "Matrix.h"

class World;
class Player
{
	Vec3 position;
	Vec3 angle;
    Vec3 up;
    Vec3 right;
    Matrix4 camera;

    World *world;

    float velocity;
    bool onGround;

public:
	Player(World *world, const Vec3 & position);
	~Player(void);

	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();

	void update();
	void render();

	void look(int x, int y);

	void setPosition(const Vec3 &);
    const Vec3 getPosition() const;

    const Vec3 getDirection() const;
    const char *getDirectionAsString();

    Matrix4 & getCameraMatrix();
    //const Vec3 & getCameraLookingAt();

};

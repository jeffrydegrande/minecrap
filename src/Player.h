//
//  Player.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "Vec3.h"
#include "Matrix.h"

class Player
{
	Vec3 position;
	Vec3 angle;

    Matrix4 camera_matrix;

public:
	Player();
	Player(float xpos, float ypos, float zpos);
	Player(const Vec3 & position);


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
};




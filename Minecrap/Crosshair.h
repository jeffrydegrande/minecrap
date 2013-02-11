//
//  Crosshair.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/9/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "minecrap.h"

class Crosshair {
	float centerX;
	float centerY;

	GLuint crosshair;

public:
	Crosshair(int screenWidth, int screenHeight);
	~Crosshair();

	void render();
private:

	void draw();
};

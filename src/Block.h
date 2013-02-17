//
//  Block.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/9/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "minecrap.h"

class Block {
public:
	static void setup();
	static void render(GLubyte blockType);
    static void draw(GLfloat size);

protected:
    static void drawRock();
    static void drawDirt();
    static void drawWater();
    static void drawTest();
    static void drawGrass();
};
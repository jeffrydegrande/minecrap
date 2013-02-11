
//
//  Crosshair.cpp
//  Minecrap
//
//  Created by Jeffry Degrande on 2/9/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//


#include "Crosshair.h"
#include "Color.h"

#include <assert.h>

Crosshair::Crosshair(int screenWidth, int screenHeight) {
	
	OutputDebugString("Initializing crosshair\n");
	
	this->centerX = (float)screenWidth / 2;
	this->centerY = (float)screenHeight / 2;

	this->crosshair = glGenLists(1);
    assert(this->crosshair != 0);
    glNewList(this->crosshair, GL_COMPILE);
	this->draw();
	glEndList();
}

Crosshair::~Crosshair() {
	glDeleteLists(this->crosshair, 1);
}

void Crosshair::render() {
	glCallList(this->crosshair);
}

void Crosshair::draw() {
    glLineWidth(2.0f);    
    glMaterialfv(GL_FRONT, GL_AMBIENT, white);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    
    glBegin(GL_LINES);
    glVertex2f(this->centerX - 16.0f, this->centerY);
    glVertex2f(this->centerX - 2.0f, this->centerY);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2f(this->centerX + 16.0f, this->centerY);
    glVertex2f(this->centerX + 2.0f, this->centerY);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2f(this->centerX, this->centerY - 16.0f);
    glVertex2f(this->centerX, this->centerY - 2.0f);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2f(this->centerX, this->centerY + 16.0f);
    glVertex2f(this->centerX, this->centerY + 2.0f);
    glEnd();
}
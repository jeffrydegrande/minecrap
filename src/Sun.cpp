#include "Sun.h"
#include "Block.h"
#include "Color.h"
#include <assert.h>

Sun::Sun() {
	sun = glGenLists(1);
	assert(sun != 0);
	glNewList(sun, GL_COMPILE);
	this->draw();
	glEndList();
}

Sun::~Sun() {
	glDeleteLists(this->sun, 1);
}

int Sun::render() {
	glCallList(this->sun);
	return 1;
}

void Sun::draw() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, dyellow);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
    glMaterialfv(GL_FRONT, GL_EMISSION, yellow);
    glPushMatrix();
    glTranslatef(0, 300, 0);
	Block::draw(8.0f);
    glPopMatrix();
    glMaterialfv(GL_FRONT, GL_EMISSION, black);
}
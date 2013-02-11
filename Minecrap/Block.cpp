#include <Windows.h>
#include <gl/GLU.h>
#include <assert.h>



#include "Color.h"
#include "Block.h"

static GLuint theRockBlock;
static GLuint theWaterBlock;
static GLuint theTestBlock;
static GLuint theDirtBlock;

void Block::setup() {
    theRockBlock = glGenLists(1);
    assert(theRockBlock != 0);
    glNewList(theRockBlock, GL_COMPILE);
	Block::drawRock();
    glEndList();

    theWaterBlock = glGenLists(1);
    assert(theWaterBlock != 0);
    glNewList(theWaterBlock, GL_COMPILE);
    Block::drawWater();
    glEndList();
    
    theTestBlock = glGenLists(1);
    assert(theTestBlock != 0);
    glNewList(theTestBlock, GL_COMPILE);
	Block::drawTest();
	glEndList();
    
    theDirtBlock = glGenLists(1);
    assert(theDirtBlock != 0);
    glNewList(theDirtBlock, GL_COMPILE);
	Block::drawDirt();
	glEndList();
}


void Block::render(GLubyte blockType) {
    switch (blockType) {
        case ROCK:
             glCallList(theRockBlock);
            break;
        case DIRT:
            glCallList(theDirtBlock);
            break;
        case WATER:
            glCallList(theWaterBlock);
            break;
        case RED:
            glCallList(theTestBlock);
    }
}


#if 0
/* select colour based on value in the world array */
glMaterialfv(GL_FRONT, GL_SPECULAR, white);
glMaterialf(GL_FRONT, GL_SHININESS, 90.0);

if (block == ROCK)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, dstone);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, stone);
} else if (block == GRASS) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, dgreen);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
} else if (block == DIRT) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, dbrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);
} else if (block == WATER) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, dblue);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
} else if (block == RED) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, red);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
} else {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
}
#endif

void Block::drawRock() {
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, dstone);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, stone);
	Block::drawBlock();
}

void Block::drawDirt() {
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, dbrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);
	Block::drawBlock();
}

void Block::drawWater() {
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, dblue);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    Block::drawBlock();
}

void Block::drawTest() {
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, dred);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	Block::drawBlock();
}


void Block::drawBlock() {
  GLfloat size = 1;
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}
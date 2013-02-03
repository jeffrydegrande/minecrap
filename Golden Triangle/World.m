//
//  World.m
//  Golden Triangle
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "World.h"
#include "defs.h"
#include "perlin.h"
#include "simplex.h"

#include <OpenGl/gl.h>
#include <GLUT/glut.h>
#include <GLUT/gutil.h>


#define AIR 0
#define ROCK 1
#define GEMS 2
#define DIRT 3
#define GRASS 4
#define LAVA 5
#define WATER 6

#define WORLDX 64
#define WORLDY 64
#define WORLDZ 64

GLubyte world[WORLDX][WORLDY][WORLDZ];

@implementation World

- (void) build {
    
    int blockCount = [self buildSimplex];
    [self addDirt];
    
    int x, y, z;
    int stone = 0;
    int dirt = 0;
    int empty = 0;
    
    for(x=0; x<WORLDX-1; x++) {
        for (y=0; y<WORLDY-1; y++) {
            for (z=0; z < WORLDZ-1; z++) {
                if (world[x][y][z] == ROCK)
                    stone++;
                else if (world[x][y][z] == DIRT)
                    dirt++;
                else
                    empty++;
            }
        }
    }
    
    NSLog(@"%d blocks in this world. %d stone, %d dirt, %d empty", blockCount, stone,dirt, empty);
}

- (void) buildTest {
    int blockCount = 0;
    for (int x=0; x<WORLDX; x++) {
        for (int y=0; y< WORLDY; y++) {
            for (int z=0; z < WORLDZ; z++) {
                world[x][y][z] = rand() % 6 + 1;
                
                if (world[x][y][z] < 1 || world[x][y][z] > 6)
                    NSLog(@"UNKNOWN BLOCK TYPE PICKED");
                
                blockCount++;
            }
        }
    }
    NSLog(@"World consists of %d blocks", blockCount);
}

- (void) renderSun {
    glMaterialfv(GL_FRONT, GL_AMBIENT, dyellow);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
    
    glPushMatrix();
    glTranslatef(0, 80, 0);
    glutSolidCube(8.0);
    glPopMatrix();
}

- (int) addDirt {
    int x, y, z;
    for(x=0; x<WORLDX; x++) {
        for (y=0; y<WORLDY; y++) {
            for (z=0; z < WORLDZ; z++) {
                int block = world[x][y][z];
                int ontop = world[x][y+1][z];
                if (block == ROCK && ontop == AIR) {
                    world[x][y][z] = DIRT;
                }
            }
        }
    }
 
    return 0;
}

- (int) buildSimplex {
    int blockCount = 0;
    int x, y, z;
    float caves, center_falloff, plateau_falloff, density;
    
    for(x=0; x<WORLDX; x++) {
        for (y=0; y<WORLDY; y++) {
            for (z=0; z < WORLDZ; z++) {
                float xf = (float)x/(float)WORLDX;
                float yf = (float)y/(float)WORLDY;
                float zf = (float)z/(float)WORLDZ;
                
                if(yf <= 0.8){
                    plateau_falloff = 1.0;
                }
                else if(0.8 < yf && yf < 0.9){
                    plateau_falloff = 1.0-(yf-0.8)*10.0;
                }
                else{
                    plateau_falloff = 0.0;
                }
                
                center_falloff = 0.1/(
                                      pow((xf-0.5)*1.5, 2) +
                                      pow((yf-1.0)*0.8, 2) +
                                      pow((zf-0.5)*1.5, 2)
                                      );
                caves = pow(simplex_noise(1, xf*5, yf*5, zf*5), 3);
                density = (
                           simplex_noise(5, xf, yf*0.5, zf) *
                           center_falloff *
                           plateau_falloff
                           );
                density *= pow(
                               noise((xf+1)*3.0, (yf+1)*3.0, (zf+1)*3.0)+0.4, 1.8
                               );
                if(caves<0.5){
                    density = 0;
                }
                world[x][y][z] = (density > 3.1 ? ROCK : AIR);
                blockCount++;
            }
        }
    }
    return blockCount;
}



- (int) render {
    [self renderSky];
    [self renderSun];
    return [self renderBlocks];
}



bool exposedToAir(int x, int y, int z)
{
    if (x == 0 || x == 0 || z == 0)
        return false;
    if (x == WORLDX-1 || y == WORLDY - 1 || z == WORLDZ - 1)
        return false;
    
    if (world[x+1][y][z] == AIR || world[x-1][y][z] == AIR
        || world[x][y+1][z] == AIR || world[x][y-1][z] == AIR
        || world[x][y][z+1] == AIR || world[x][y][z-1] == AIR
        )
        return true;
    else
        return false;
}


- (int) renderBlocks
{
    int x=0, y=0, z=0;
    int c=0;
    
    // render blocks with fixed x
    for (x= 0; x < WORLDX; x++) {
        for (y=0; y<WORLDY; y++) {
            for (z=0; z<WORLDZ; z++) {
                if (world[x][y][z] != AIR && exposedToAir(x, y, z)) {
                    renderBlock(x, y, z);
                    c++;
                }
            }
        }
    }
    return c;
}

- (void) renderSky {
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_AMBIENT, black);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, skyblue);
    glPushMatrix();
    glTranslatef(50, 25, 50);
    glutSolidCube(150.0);
    glPopMatrix();
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
}



static void renderBlock(int x, int y, int z) {
    GLubyte block = world[x][y][z];
    
    if (block == AIR)
        return;
    
    /* select colour based on value in the world array */
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
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
    } else {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    }
    
    glPushMatrix();
    glTranslatef(x+0.5f, y+0.5f, z+0.5f);
    
    
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
    
    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1.0f;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1.0f;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1.0f;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1.0f;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -1.0f;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = 1.0f;
    
    for (i = 5; i >= 0; i--) {
        glBegin(GL_QUADS);
        glNormal3fv(&n[i][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
    
    glPopMatrix();
    
}

@end

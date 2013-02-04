//
//  World.m
//  Golden Triangle
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "World.h"
#import "defs.h"
#include "perlin.h"
#include "simplex.h"

#include <OpenGl/gl.h>
#include <GLUT/glut.h>
#include <GLUT/gutil.h>


#define AIR   0
#define ROCK  1
#define GEMS  2
#define DIRT  3
#define GRASS 4
#define LAVA  5
#define WATER 6
#define RED  7

#define WORLDX 64
#define WORLDY 64
#define WORLDZ 64

GLubyte world[WORLDX][WORLDY][WORLDZ];

#define foreach_xyz \
  for (int x=0; x<WORLDX; x++) { \
    for (int y=0; y< WORLDY; y++) { \
      for (int z=0; z < WORLDZ; z++)

#define endforeach }}

@implementation World


# pragma mark - TerrainBuilding

- (void) build {
    [self buildWithSimplexNoise];
    [self addMarkersAtTerrainBoundaries];
    [self summarizeTerrain];
}

- (void) buildWithRandomValues {
    foreach_xyz
        world[x][y][z] = rand() % 6 + 1;
    endforeach
}

- (int) buildWithSimplexNoise {
    int blockCount = 0;
    float caves, center_falloff, plateau_falloff, density;
    
    foreach_xyz {
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
        density = simplex_noise(5, xf, yf*0.5, zf) * center_falloff * plateau_falloff;
        
        density *= pow(
                       noise((xf+1)*3.0, (yf+1)*3.0, (zf+1)*3.0)+0.4, 1.8
                       );
        if(caves<0.5){
            density = 0;
        }
        world[x][y][z] = (density > 3.1 ? ROCK : AIR);
        blockCount++;
    }
    endforeach
    
    [self addDirt];
    
    return blockCount;
}

- (void) summarizeTerrain {
    int rock =0, dirt=0, empty=0;
    int blockCount = 0;
    
    foreach_xyz {
        switch(world[x][y][z]) {
            case ROCK: rock++; break;
            case DIRT: dirt++; break;
            default: empty++;
        }
        blockCount++;
    } endforeach

    NSLog(@"%d blocks in this world. %d stone, %d dirt, %d empty", blockCount, rock, dirt, empty);
}



- (void) addMarkersAtTerrainBoundaries {
    world[0][0][0] = RED;
    world[0][0][WORLDZ-1] = RED;
    world[WORLDX-1][0][0] = RED;
    world[WORLDX-1][0][WORLDZ-1] = RED;
    
    world[0][WORLDY-1][0] = RED;
    world[0][WORLDY-1][WORLDZ-1] = RED;
    world[WORLDX-1][WORLDY-1][0] = RED;
    world[WORLDX-1][WORLDY-1][WORLDZ-1] = RED;
    
}

- (int) addDirt {
    foreach_xyz {
        int block = world[x][y][z];
        int ontop = world[x][y+1][z];
        if (block == ROCK && ontop == AIR) {
            world[x][y][z] = DIRT;
        }
    } endforeach
    return 0;
}



# pragma mark Rendering

- (int) render {
    int blocksRendered = 0;
    blocksRendered += [self renderBlocks];
    blocksRendered += [self renderSun];
    blocksRendered += [self renderSky];
    
    return blocksRendered;
}


- (int) renderSun {
    glMaterialfv(GL_FRONT, GL_AMBIENT, dyellow);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
    
    glPushMatrix();
    glTranslatef(0, 80, 0);
    glutSolidCube(8.0);
    glPopMatrix();
    
    return 1; // only one block rendered
}


- (int) renderSky {
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
    
    return 1; // only one block rendered
}



bool exposedToAir(int x, int y, int z)
{
    return (world[x+1][y][z] == AIR || world[x-1][y][z] == AIR
            || world[x][y+1][z] == AIR || world[x][y-1][z] == AIR
            || world[x][y][z+1] == AIR || world[x][y][z-1] == AIR);
}


- (int) renderBlocks {
    int renderedBlocksCount=0;
    foreach_xyz {
        if (world[x][y][z] != AIR && exposedToAir(x, y, z)) {
            [self renderBlock:x :y :z];
            renderedBlocksCount++;
        }
    } endforeach
    return renderedBlocksCount;
}

- (void) renderBlock:(int) x :(int)y :(int)z {
    GLubyte block = world[x][y][z];
    
    if (block == AIR)
        return;
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

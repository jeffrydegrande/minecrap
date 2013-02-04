//
//  World.m
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "World.h"
#import "defs.h"
#include "simplex.h"

#include <OpenGl/gl.h>
#include <GLUT/glut.h>
#include <GLUT/gutil.h>

#include "minecrap.h"


@implementation World

# pragma mark - TerrainBuilding

- (id)init
{
    self = [super init];
    if (self) {
        chunk = [[Chunk alloc] init];
    }
    return self;
}

# pragma mark Rendering

- (int) render {
    int blocksRendered = 0;
    blocksRendered += [self renderBlocks];
    blocksRendered += [self renderSun];
    [self renderSky];
    
    return blocksRendered;
}


- (int) renderSun {
    glMaterialfv(GL_FRONT, GL_AMBIENT, dyellow);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
    glMaterialfv(GL_FRONT, GL_EMISSION, yellow);
    glPushMatrix();
    glTranslatef(0, 300, 0);
    glutSolidCube(8.0);
    glPopMatrix();
    glMaterialfv(GL_FRONT, GL_EMISSION, black);

    
    return 1; // only one block rendered
}


- (int) renderSky {
/*
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_AMBIENT, black);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, skyblue);
    glPushMatrix();
    glTranslatef(50, 25, 50);
    glutSolidCube(300.0);
    glPopMatrix();
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    */
    return 1; // only one block rendered
}


- (int) renderBlocks {
    int renderedBlocksCount=0;
    foreach_xyz {
        if ([chunk blockAt:x :y :z] != AIR && [chunk isExposedToAir:x :y :z]) {
            [self renderBlock:x :y :z];
            renderedBlocksCount++;
        }
    } endforeach
    return renderedBlocksCount;
}

- (void) renderBlock:(int) x :(int)y :(int)z {
    GLubyte block = [chunk blockAt:x :y :z];
    
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
    //    glTranslatef(x+0.5f, y+0.5f - 60.f, z+0.5f);
    glTranslatef(x, y, z);
    
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

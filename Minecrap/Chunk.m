//
//  Chunk.m
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "Chunk.h"
#include "Color.h"

#include "minecrap.h"
#include "simplex.h"

#include <OpenGl/gl.h>
#include <GLUT/glut.h>
#include <GLUT/gutil.h>


@implementation Chunk

@synthesize worldX, worldY;

- (id)initWithWorldPosition:(int)x :(int)y
{
    self = [super init];
    if (self) {
        self.worldX = x;
        self.worldY = y;
        [self build];
    }
    return self;
}


- (void) build {
    [self generateTerrain];
    [self addDirt];
    [self addWaterLevel];
    [self addBedrock];
    [self addMarkersAtTerrainBoundaries];
    [self summarizeTerrain];
}

- (int) buildSimple {
    int blockCount = 0;
    
    foreach_xyz {
        blocks[x][y][z] = ROCK;
        blockCount++;
    } endforeach
    
    return blockCount;
}

float terrainNoise(float x, float y, float frequency, float amplitude) {
    float result = 0;
    float amp = 1;
    float freq = 1;
    float max = 0;
    
    x*= 1/64.0f;
    y*= 1/64.0f;
    
    for (int i = 0; i < 8; i++) {
        result += noise2D(x * freq, y*freq) * amp;
        max += amp;
        freq *= frequency;
        amp *= amplitude;
    }
    
    result /= max;
    
    return result;
}

- (int) generateTerrain {
    int blockCount = 0;

    for(int x=0; x<CHUNKX; x++) {
        for(int z=0; z<CHUNKZ; z++) {            
            float maxHeight = terrainNoise(x + (worldX << 4), z + (worldY << 4), 0.5, 0.05) * WATER_LEVEL + WATER_LEVEL;
            assert(maxHeight <= CHUNKY && maxHeight >= 0);
            // NSLog(@"%ld,%ld, height: %f (%d)", realX, realZ, maxheight, (int)maxheight);
            blocks[x][(int)maxHeight][z] = ROCK;
        }
    }
    return blockCount;
}

- (void) summarizeTerrain {
    int rock =0, dirt=0, air=0, test=0, empty=0;
    int blockCount = 0;
    
    foreach_xyz {
        switch(blocks[x][y][z]) {
            case AIR: air++; break;
            case RED: test++; break;
            case ROCK: rock++; break;
            case DIRT: dirt++; break;
            default: empty++;
        }
        blockCount++;
    } endforeach
    
    NSLog(@"chunk at %ld/%ld: %d blocks.\n%d stone,\n%d dirt,\n%d empty,\n%d test,\n%d air\n", self.worldX, self.worldY, blockCount, rock, dirt, empty, test, air);
}



- (void) addMarkersAtTerrainBoundaries {
    blocks[0][0][0] = RED;
    blocks[0][0][CHUNKZ-1] = RED;
    blocks[CHUNKX-1][0][0] = RED;
    blocks[CHUNKX-1][0][CHUNKZ-1] = RED;
    
    blocks[0][CHUNKY-1][0] = RED;
    blocks[0][CHUNKY-1][CHUNKZ-1] = RED;
    blocks[CHUNKX-1][CHUNKY-1][0] = RED;
    blocks[CHUNKX-1][CHUNKY-1][CHUNKZ-1] = RED;
}

- (int) addDirt {
    foreach_xyz {
        int block = blocks[x][y][z];
        int ontop = blocks[x][y+1][z];
        if (block == ROCK && ontop == AIR) {
            blocks[x][y][z] = DIRT;
        }
    } endforeach
    return 0;
}

- (int) addBedrock {
    int blockCount = 0;
    for (int x=0; x<CHUNKX; x++) {
        for (int z=0; z<CHUNKZ; z++ ) {
            blocks[x][0][z] = ROCK;
        }
    }
    return blockCount;
}

- (int) addWaterLevel {
    int blockCount = 0;
    for (int x=0; x<CHUNKX; x++) {
        for (int z=0; z<CHUNKZ; z++ ) {
            blocks[x][WATER_LEVEL][z] = WATER;
        }
    }
    return blockCount;
}

- (bool) isBorderBlock:(int)x :(int)y :(int)z {
    return (x == 0 || z == 0 || x == CHUNKX || z == CHUNKZ);
}

- (bool) isExposedToAir:(int)x :(int)y :(int)z {
    return (blocks[x+1][y][z] == AIR || blocks[x-1][y][z] == AIR
            || blocks[x][y+1][z] == AIR || blocks[x][y-1][z] == AIR
            || blocks[x][y][z+1] == AIR || blocks[x][y][z-1] == AIR);
}

- (int) renderBlock:(int) x :(int)y :(int)z {
    /*
    
    if (blocks [x][y][z] == AIR || ![self isExposedToAir:x :y :z]) {
        return 0;
    }
     */
    if (blocks[x][y][z] != ROCK && blocks[x][y][z] != WATER)
        return 0;
     
    GLubyte block = blocks[x][y][z];
    
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
    glTranslatef((self.worldX << 4) + x,  y, (self.worldY << 4) + z);
    
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
 
    return 1;
}

@end
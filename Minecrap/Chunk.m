//
//  Chunk.m
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "Chunk.h"
#include "Color.h"

#include <OpenGl/gl.h>
#include <GLUT/glut.h>
#include <GLUT/gutil.h>

#include "minecrap.h"
#include "simplex.h"
#include "Block.h"

@implementation Chunk

@synthesize worldX, worldY, seed;

- (id)initWithWorldPosition:(int)x :(int)y :(int)worldSeed
{
    self = [super init];
    if (self) {
        self.seed = worldSeed;
        self.worldX = x;
        self.worldY = y;
        [self build];
    }
    return self;
}

- (int) heightAtPosition:(int)x :(int)z {
    for (int y=CHUNKY; y >= 0; y--) {
        if (blocks[x][y][z] != AIR) {
            return y;
        }
    }
    return 0;
}

- (void) setBlock:(int)x :(int)y :(int)z :(GLubyte)blockType
{    
    blocks[x][y][z] = blockType;
}

- (void) build {
    [self generateTerrain];
    [self addDirt];
    
    // [self addWaterLevel];
    // [self addBedrock];
    // [self addMarkersAtTerrainBoundaries];
    // [self summarizeTerrain];
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
                 float maxHeight = terrainNoise(x + self.seed + (worldX << 4), z + self.seed + (worldY << 4), 0.5, 0.5) * 32 + WATER_LEVEL;
                 
                 assert(maxHeight <= CHUNKY && maxHeight >= 0);
                 // NSLog(@"%ld,%ld, height: %f (%d)", realX, realZ, maxheight, (int)maxheight);
                 blocks[x][(int)maxHeight][z] = ROCK;
                blocks[x][(int)maxHeight - 1][z] = ROCK;
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
    
    if (blocks[x][y][z] == AIR)
        return 0;
    
    
    /*
    if (blocks [x][y][z] == AIR || ![self isExposedToAir:x :y :z]) {
        return 0;
    }
     */

    
    GLubyte block = blocks[x][y][z];
    
    
    glPushMatrix();
    glTranslatef((self.worldX << 4) + x,  y, (self.worldY << 4) + z);
    [Block render:block];
 
    glPopMatrix();


 
    return 1;
}

@end
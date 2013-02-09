//
//  World.m
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include <time.h>
#include <stdlib.h>

#include <OpenGl/gl.h>
#include <GLUT/glut.h>
#include <GLUT/gutil.h>

#import "minecrap.h"
#import "World.h"
#import "Color.h"


@implementation World

@synthesize seed, playerSpawnPoint;

# pragma mark - TerrainBuilding

#define INITIAL_WORLD_SIZE 8

- (id) initWithSeed:(int) worldSeed
{
    self = [super init];
    if (self) {
        srand((unsigned int)worldSeed);
        self.seed = rand() % 65536;
        [self buildChunks:INITIAL_WORLD_SIZE];
        NSLog(@"World of size %ld by %ld created", (unsigned long)[self size], (unsigned long)[self size]);
        [self calculatePlayerSpawnPoint];
    }
    return self;
}


- (id)init
{
    self = [super init];
    if (self) {
        srand((unsigned int)time(NULL));
        self.seed = rand() % 65536;
        [self buildChunks:INITIAL_WORLD_SIZE];
        [self calculatePlayerSpawnPoint];
    }
    return self;
}

- (void) buildChunks:(int) count {
    if (rowOfChunks == NULL) {
        rowOfChunks = [[NSMutableArray alloc] initWithCapacity:count];
    }
    
    for (int x=0; x<count; x++) {
        NSMutableArray *row = [[NSMutableArray alloc] initWithCapacity:count];
        for (int y=0; y < count; y++) {
            Chunk *chunk = [[Chunk alloc] initWithWorldPosition:x :y :seed];
            [row addObject: chunk];
        }
        
        [rowOfChunks addObject:row];
    }
}


- (NSUInteger) size {
    return [rowOfChunks count] << 4;
}

- (void) calculatePlayerSpawnPoint {
    
    NSUInteger size = [self size];
    
    // we need an x and a z that are randoms within the world size
    
    int x = rand() % size;
    int z = rand() % size;
    
    // find the chunk where x,z can be found
    int xrow = x / 16;
    int zrow = z / 16;

    Chunk *chunk = [self findChunkAtPosition:xrow :zrow];
    
    int xChunk = x % 16;
    int zChunk = z % 16;
    
    int y = [chunk heightAtPosition:xChunk :zChunk];

    [chunk setBlock:xChunk :(y+1) :zChunk :RED];
    
    playerSpawnPoint.x = x;
    playerSpawnPoint.y = y+1;
    playerSpawnPoint.z = z;
    
    NSLog(@"spawning at %d,%d,%d (found in chunk %d, %d / %d, %d)", x, y, z, xrow, zrow, xChunk, zChunk);
}

- (Chunk *) findChunkAtPosition:(int)x :(int)y {
    NSMutableArray *chunks = [rowOfChunks objectAtIndex:x];
    return [chunks objectAtIndex:y];
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
    return 0; // only one block rendered
}

- (int) renderBlocks {
    int renderedBlocksCount=0;
    
    for (NSMutableArray *row in rowOfChunks) {
        for (Chunk *chunk in row) {
            foreach_xyz {
                renderedBlocksCount += [chunk renderBlock:x :y :z];
            } endforeach
        }
    }
    return renderedBlocksCount;
}


@end

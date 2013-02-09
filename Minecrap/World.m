//
//  World.m
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "World.h"
#import "Color.h"

#include <OpenGl/gl.h>
#include <GLUT/glut.h>
#include <GLUT/gutil.h>

#include "minecrap.h"
#include <time.h>
#include <stdlib.h>


@implementation World

@synthesize seed;

# pragma mark - TerrainBuilding


- (id) initWithSeed:(int) worldSeed
{
    self = [super init];
    if (self) {
        srand((unsigned int)worldSeed);
        self.seed = rand() % 65536;
        int count = 3;
        rowOfChunks = [[NSMutableArray alloc] initWithCapacity:count];
        for (int i =0; i < count; i++) {
            [self makeRow:count :i];
            
        }
    }
    return self;
}

- (id)init
{
    self = [super init];
    if (self) {
        srand((unsigned int)time(NULL));
        self.seed = rand() % 65536;
        int count = 3;
        rowOfChunks = [[NSMutableArray alloc] initWithCapacity:count];
        for (int i =0; i < count; i++) {
            [self makeRow:count :i];
            
        }
    }
    return self;
}

- (void) makeRow:(int)size :(int)rowNumber
{
    NSMutableArray *row = [[NSMutableArray alloc] initWithCapacity:size];
    for (int i=0; i < size; i++) {
        Chunk *chunk = [[Chunk alloc] initWithWorldPosition:i :rowNumber :self.seed];
        [row addObject:chunk];
    }
    
    [rowOfChunks addObject:row];
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

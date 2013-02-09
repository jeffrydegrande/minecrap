//
//  Chunk.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "minecrap.h"
#include <OpenGl/gl.h>
#include <GLUT/glut.h>
#include <GLUT/gutil.h>

@interface Chunk : NSObject
{
    GLubyte blocks[CHUNKX][CHUNKY][CHUNKZ];
}

@property (assign) NSInteger worldX;
@property (assign) NSInteger worldY;


- (id) initWithWorldPosition:(int)x :(int)y;
- (int) renderBlock:(int)x :(int)y :(int)z;
- (bool) isExposedToAir:(int)x :(int)y :(int)z;
@end

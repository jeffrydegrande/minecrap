//
//  MyOpenGLView.m
//  Golden Triangle
//
//  Created by Jeffry Degrande on 1/28/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "MyOpenGLView.h"
#include <OpenGl/gl.h>
#include <GLUT/glut.h>
#include <GLUT/gutil.h>
#include <time.h>

#define RENDER_DISTANCE 1000  // was 1536
#define NEAR_CLIP       1.0f  // was 0.2f
#define FOV             60



#define BLOCK  1

#define WORLDX 10
#define WORLDY 10
#define WORLDZ 10

GLubyte world[WORLDX][WORLDY][WORLDZ];

@implementation MyOpenGLView


// The init function since we're loading from a NIB so regular
// init messages arent' being sent.

- (void) awakeFromNib {
    lastTicks = clock();
    cubeRotationAngle = 0;
    
    camera = [Camera alloc];
    if (!camera)
        NSLog(@"No camera bro!");
    
    [self buildWorld];

}

- (void)prepareOpenGL {
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    [[self window] setAcceptsMouseMovedEvents:YES];
}


- (void)reshape {
    NSRect rect = [self bounds];
    glViewport(0, 0, rect.size.width, rect.size.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    CGFloat view_aspect = rect.size.width / rect.size.height;
    CGFloat fovy = FOV;
    
    if (view_aspect > 1.0f) {
        fovy /= view_aspect;
    }
    gluPerspective(fovy, view_aspect, NEAR_CLIP, RENDER_DISTANCE);    
    glMatrixMode(GL_MODELVIEW);
}


- (BOOL)acceptsFirstResponder
{
    return YES;
}



- (void)keyDown:(NSEvent *)theEvent{
    
    NSString *theArrow = [theEvent charactersIgnoringModifiers];
    unichar keyChar = 0;
    
    keyChar = [theArrow characterAtIndex:0];
    
    
    if ( [theArrow length] == 0 )
        return; // reject dead keys
    
    
    if ( [theArrow length] == 1 ) {
        keyChar = [theArrow characterAtIndex:0];
        if ( keyChar == NSLeftArrowFunctionKey || keyChar == 97) {
            [camera strafeLeft];
            [self setNeedsDisplay:YES];
            return;
        }
        if ( keyChar == NSRightArrowFunctionKey || keyChar == 100) {
            [camera strafeRight];
            [self setNeedsDisplay:YES];
            return;
        }
        if ( keyChar == NSUpArrowFunctionKey || keyChar == 119) {
            [camera moveForward];
            [self setNeedsDisplay:YES];
            return;
        }
        if ( keyChar == NSDownArrowFunctionKey || keyChar == 115 ) {
            [camera moveBackward];
            [self setNeedsDisplay:YES];

            return;
        }
        
        if (keyChar == 113) {
            [camera lookUp];
            [self setNeedsDisplay:YES];

            return;
        }
        
        if (keyChar == 122) {
            [camera lookDown];
            [self setNeedsDisplay:YES];

            return;
        }
        [super keyDown:theEvent];
    }
    [super keyDown:theEvent];
}


- (void)mouseMoved:(NSEvent *)theEvent {
    NSPoint point = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    [camera updateWithPoint:point];
    [self setNeedsDisplay:YES];
}


- (void)drawRect:(NSRect)dirtyRect
{
    long ticks = clock();
    // delta_t in millis;
    int delta_t = (int)((ticks - lastTicks)/(CLOCKS_PER_SEC/1000));
    int fps = delta_t > 0 ? (int) 1000 / delta_t : 1000;
    
    [[self window] setTitle:[NSString stringWithFormat:@"%d fps", fps]];
    lastTicks = ticks;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    glLoadIdentity();
    [camera update];
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    [self renderCubes];
    glFlush();
}



- (void) buildWorld {
    int blockCount = 0;
    for (int x=0; x<WORLDX; x++) {
        for (int y=0; y< WORLDY; y++) {
            for (int z=0; z < WORLDZ; z++) {
                world[x][y][z] = BLOCK;
                blockCount++;
            }
        }
    }
    NSLog(@"World consists of %d blocks", blockCount);

}


static void renderCube(int x, int y, int z) {    
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(x + 0.5f, y+0.5f, z+0.5f);
    glutSolidCube(1.0);
    glPopMatrix();
    
}

- (void) renderCubes
{
    for (int x=0; x<WORLDX; x++) {
        for (int y=0; y< WORLDY; y++) {
            for (int z=0; z < WORLDZ; z++) {
                renderCube(x, y, z);
            }
        }
    }
}

@end

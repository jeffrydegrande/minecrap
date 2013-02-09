//
//  MyOpenGLView.m
//  Minecrap
//
//  Created by Jeffry Degrande on 1/28/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "MyOpenGLView.h"
#include <OpenGl/gl.h>
#include <GLUT/glut.h>
#include <GLUT/gutil.h>
#include <time.h>

#include "Block.h"

#define RENDER_DISTANCE 300  // was 1536
#define NEAR_CLIP       0.2f  // was 0.2f
#define FOV             60

int optionDrawWireframes = 0;  // toggled by pressing '1'
int optionSmoothShading  = 1;  // toggled by pressing '2'
int optionLighting = 1;        // toggled by pressing '3'


@implementation MyOpenGLView

// The init function since we're loading from a NIB so regular
// init messages arent' being sent.

- (void) awakeFromNib {
    lastTicks = clock();

    world = [[World alloc] initWithSeed:300];
    camera = [Camera alloc];
    
    [camera setPosition:[world playerSpawnPoint]];
}

- (void)prepareOpenGL {
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    [[self window] setAcceptsMouseMovedEvents:YES];
    
    glClearColor(0.52, 0.74, 0.84, 1.0);
    
    [Block setup];

    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
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
    glLoadIdentity();
}




- (void)drawRect:(NSRect)dirtyRect
{
    long ticks = clock();
    // delta_t in millis;
    int delta_t = (int)((ticks - lastTicks)/(CLOCKS_PER_SEC/1000));
    int fps = delta_t > 0 ? (int) 1000 / delta_t : 1000;
    
    lastTicks = ticks;
    
    if (optionDrawWireframes == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    if (optionSmoothShading == 1)
        glShadeModel(GL_SMOOTH);
    else
        glShadeModel(GL_FLAT);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    

    
    GLfloat lightPosition[] = {0.0, 100.0, 0.0, 0.0};
    GLfloat light_diffuse[]  = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_full_on[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat white[]    = {1.0, 1.0, 1.0, 1.0};


    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_full_on);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_EMISSION, light_specular);
    
    [camera update];


    int blocksRendered = [world render];
    glSwapAPPLE();
    
    [self updateMenuBar:fps :blocksRendered];
}

- (void) updateMenuBar:(int)fps :(int)blocksRendered {
    [[self window] setTitle:[NSString stringWithFormat:@"FPS: %d/%@ (%d blocks)",
                             fps,
                             [camera stringFromPosition],
                             blocksRendered]];
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

        if (keyChar == 49) { // '1'
            optionDrawWireframes = !optionDrawWireframes;
            [self setNeedsDisplay:YES];
            return;
        }
        
        if (keyChar == 50) { // '2'
            optionSmoothShading = !optionSmoothShading;
            [self setNeedsDisplay:YES];
            return;
        }
        
        if (keyChar == 51) { // '3'
            optionLighting = !optionLighting;
            if (optionLighting)
                glEnable(GL_LIGHT0);
            else
                glDisable(GL_LIGHT0);
    
            [self setNeedsDisplay:YES];
            return;
        }
        
        if (keyChar == 110) { // 'n'
            [ camera cycle];
            [self setNeedsDisplay:YES];
            return;
        }
        
        /// NSLog(@"%d", keyChar);
    }

    [super keyDown:theEvent];
}

- (void)mouseMoved:(NSEvent *)theEvent {
    NSPoint point = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    NSRect rect = [self bounds];

    if (point.x > rect.size.width || point.x < 0) {
        //  [NSCursor unhide];
        return;
    }
    
    if (point.y > rect.size.height || point.y < 0) {
        //        [NSCursor unhide];
        return;
    }
    
    //    [NSCursor hide];
    
    [camera updateWithPoint:point];
    [self setNeedsDisplay:YES];
}

@end

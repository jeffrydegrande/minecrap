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

#define STONE 1
#define GRASS 2
#define DIRT  3
#define WATER 5
#define AIR   6

#define WORLDX 10
#define WORLDY 10
#define WORLDZ 10

GLubyte world[WORLDX][WORLDY][WORLDZ];

int optionDrawWireframes = 0;  // toggled by pressing '1'
int optionSmoothShading  = 1;  // toggled by pressing '2'
int optionLighting = 0;        // toggled by pressing '3'


/* location for the light source (the sun), the first three
 values are the x,y,z coordinates */
GLfloat lightPosition[] = {0.0, 99.0, 0.0, 0.0};
/* location for light source that is kept at viewpoint location */
GLfloat viewpointLight[] = {-50.0, -50.0, -50.0, 1.0};

// COLORS

GLfloat blue[]     = {0.0, 0.0, 1.0, 1.0};
GLfloat red[]      = {1.0, 0.0, 0.0, 1.0};
GLfloat green[]    = {0.0, 1.0, 0.0, 1.0};
GLfloat yellow[]   = {1.0, 1.0, 0.0, 1.0};
GLfloat purple[]   = {1.0, 0.0, 1.0, 1.0};
GLfloat orange[]   = {1.0, 0.64, 0.0, 1.0};
GLfloat white[]    = {1.0, 1.0, 1.0, 1.0};
GLfloat black[]    = {0.0, 0.0, 0.0, 1.0};
GLfloat coal[]     = {0.15, 0.15, 0.15, 1.0};
GLfloat stone[]    = {0.5, 0.5, 0.5, 1.0};
GLfloat brown[]    = {102.0/255, 69.0/255, 35.0/255, 1.0};

GLfloat dcoal[]    = {0.075, 0.075, 0.075, 1.0};
GLfloat dstone[]   = {0.25, 0.25, 0.25, 1.0};
GLfloat dbrown[]   = {102.0/510, 69.0/510, 35.0/510, 1.0};
GLfloat dblue[]    = {0.0, 0.0, 0.5, 1.0};
GLfloat dred[]     = {0.5, 0.0, 0.0, 1.0};
GLfloat dgreen[]   = {0.0, 0.5, 0.0, 1.0};
GLfloat dyellow[]  = {0.5, 0.5, 0.0, 1.0};
GLfloat dpurple[]  = {0.5, 0.0, 0.5, 1.0};
GLfloat dorange[]  = {0.5, 0.32, 0.0, 1.0};

GLfloat skyblue[]  = {0.52, 0.74, 0.84, 1.0};

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
    
    [self setupLighting];
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
        
        if (keyChar == 49) { // 1
            optionDrawWireframes = !optionDrawWireframes;
            [self setNeedsDisplay:YES];
            return;
        }
        if (keyChar == 50) { // 2
            optionSmoothShading = !optionSmoothShading;
            [self setNeedsDisplay:YES];
            return;
        }
        if (keyChar == 51) { // 3
            optionLighting = !optionLighting;
            [self setupLighting];
            [self setNeedsDisplay:YES];
            return;
        }
    }

    [super keyDown:theEvent];
}


- (void)mouseMoved:(NSEvent *)theEvent {
    NSPoint point = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    NSRect rect = [self bounds];

    if (point.x > rect.size.width || point.x)
        return;
    
    if (point.y > rect.size.height || point.y)
        return;

    
    
    [camera updateWithPoint:point];
    [self setNeedsDisplay:YES];
}

/*  Initialize material property and light source.  */
- (void) setupLighting
{
    
    GLfloat light_ambient[]  = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[]  = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_full_off[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light_full_on[]  = {1.0, 1.0, 1.0, 1.0};
    
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    
    /* if lighting is turned on then use ambient, diffuse and specular
     lights, otherwise use ambient lighting only */
    if (optionLighting == 1)
    {
        /* sun light */
        glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        /* no specular reflection from sun, it is too distracting */
        glLightfv (GL_LIGHT0, GL_SPECULAR, light_full_off);
    }
    else
    {
        glLightfv (GL_LIGHT0, GL_AMBIENT, light_full_on);
        glLightfv (GL_LIGHT0, GL_DIFFUSE, light_full_off);
        glLightfv (GL_LIGHT0, GL_SPECULAR, light_full_off);
    }
    glLightfv (GL_LIGHT0, GL_POSITION, lightPosition);
    
    /* viewpoint light */
    glLightfv (GL_LIGHT1, GL_POSITION, viewpointLight);
    glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
    
    
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable (GL_LIGHT1);
    
    glEnable(GL_DEPTH_TEST);
    
}


- (void)drawRect:(NSRect)dirtyRect
{
    long ticks = clock();
    // delta_t in millis;
    int delta_t = (int)((ticks - lastTicks)/(CLOCKS_PER_SEC/1000));
    int fps = delta_t > 0 ? (int) 1000 / delta_t : 1000;
    
    
    [[self window] setTitle:[NSString stringWithFormat:@"FPS: %d/%@",
                             fps,
                             [camera stringFromPosition]]];
    lastTicks = ticks;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    [camera update];
    
    if (optionDrawWireframes == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (optionSmoothShading == 1)
        glShadeModel(GL_SMOOTH);
    else
        glShadeModel(GL_FLAT);

    glMaterialf(GL_FRONT, GL_SHININESS, 90.0);

    glPushMatrix();
    
    [self renderSky];
    [self renderBlocks];
    
    glPopMatrix();
    glFlush();
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


- (void) buildWorld {
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


static void renderBlock(int x, int y, int z) {
    GLubyte block = world[x][y][z];
    
    if (block == AIR) {
        return;
    }
    
    /* select colour based on value in the world array */
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    if (block == STONE)
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
    }
    
    glPushMatrix();
    glTranslatef(x + 0.5f, y+0.5f, z+0.5f);
    glutSolidCube(1.0);
    glPopMatrix();
    
}

- (void) renderBlocks
{
    for (int x=WORLDX; x >= 0; x--) {
        for (int y=WORLDY; y >= 0; y--) {
            for(int z=WORLDZ; z >= 0; z--) {
                renderBlock(x,y,z);
            }
        }
    }
}

@end

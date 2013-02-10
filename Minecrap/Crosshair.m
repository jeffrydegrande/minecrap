//
//  Crosshair.m
//  Minecrap
//
//  Created by Jeffry Degrande on 2/9/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "Crosshair.h"
#import "Color.h"

@implementation Crosshair

static GLuint theCrosshair;

static int centerx;
static int centery;


+ (void) setup:(int)screenWidth :(int)screenHeight {

    centerx = screenWidth / 2;
    centery = screenHeight / 2;
    
    if (theCrosshair == 0) {
        [Crosshair draw];
     } else {
        // redraw the list
        glDeleteLists(theCrosshair, 1);
        theCrosshair = 0;
        [Crosshair draw];
    }
}

+ (void) render {
    glCallList(theCrosshair);
}

+ (void) draw {
    theCrosshair = glGenLists(1);
    assert(theCrosshair != 0);
    glNewList(theCrosshair, GL_COMPILE);
    glLineWidth(3.0f);
    glColor3fv(white);
    glBegin(GL_LINES);
    glVertex2f(centerx - 16.0f, centery);
    glVertex2f(centerx - 2.0f, centery);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2f(centerx + 16.0f, centery);
    glVertex2f(centerx + 2.0f, centery);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2f(centerx, centery - 16.0f);
    glVertex2f(centerx, centery - 2.0f);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2f(centerx, centery + 16.0f);
    glVertex2f(centerx, centery + 2.0f);
    glEnd();

    glEndList();
}


@end
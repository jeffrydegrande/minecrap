//
//  Camera.m
//  Golden Triangle
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "Camera.h"

@implementation Camera

#define STRAFE_DISTANCE 0.4

// camera positions
float xpos = 0;
float ypos = 0;
float zpos = 0;
float xrot = 0;
float yrot = 0;

float lastx = 0;
float lasty = 0;


- (int) bound:(int) value {
    if (value < -360) {
        value += 360;
    }
    if (value >= 360) {
        value -= 360;
    }
    
    return value;
}

- (void) moveLeft {
    yrot = [self bound:yrot - 1];
}

- (void) moveRight {
    yrot = [self bound:yrot + 1];
}

- (void) strafeRight {
    float yrotrad;
    yrotrad = (yrot / 180 * PI);
    xpos += (float) cos(yrotrad) * STRAFE_DISTANCE;
    zpos += (float) sin(yrotrad) * STRAFE_DISTANCE;
}

- (void) strafeLeft {
    float yrotrad;
    yrotrad = (yrot / 180 * PI);
    xpos -= (float) cos(yrotrad) * STRAFE_DISTANCE;
    zpos -= (float) sin(yrotrad) * STRAFE_DISTANCE;
    
    
}

- (void) moveForward {
    float xrotrad, yrotrad;
    xrotrad = (xrot / 180 * PI);
    yrotrad = (yrot / 180 * PI);
    
    xpos += (float)sin(yrotrad);
    ypos -= (float)sin(xrotrad);
    zpos -= (float)cos(yrotrad);
}

- (void) moveBackward {
    float xrotrad, yrotrad;
    xrotrad = (xrot / 180 * PI);
    yrotrad = (yrot / 180 * PI);
    
    xpos -= (float)sin(yrotrad);
    ypos += (float)sin(xrotrad);
    zpos += (float)cos(yrotrad);
}


- (void) lookDown {
    xrot = [self bound: xrot + 1];
}

- (void) lookUp {
    xrot -= 1;
    xrot = [self bound: xrot - 1];
}


- (void) update {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on they-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}

- (void) updateWithPoint: (NSPoint)point {


    if (lastx == 0 && lasty == 0) {
        lastx = point.x;
        lasty = point.y;
        
    } else {
        int diffx = point.x - lastx; // check the difference between the current x and the last x position
        int diffy = point.y - lasty; // check the difference between the current y and the last y position
        lastx = point.x;             // set lastx to the current x position
        lasty = point.y;             // set lasty to the current y position
    

    
        // set the xrot to xrot with the additionof the difference in the y position
        xrot = [self bound: xrot - (float) diffy];

        // set the yrot to yrot with the addition of the difference in the position
        yrot = [self bound: yrot + (float) diffx];
    }
}

@end

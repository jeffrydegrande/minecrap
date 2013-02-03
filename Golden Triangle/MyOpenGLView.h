//
//  MyOpenGLView.h
//  Golden Triangle
//
//  Created by Jeffry Degrande on 1/28/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Camera.h"

@interface MyOpenGLView : NSOpenGLView
{
    Camera *camera;
    long lastTicks;
 
    float cubeRotationAngle;
}

- (void) buildWorld;
- (void) renderBlocks;

@end

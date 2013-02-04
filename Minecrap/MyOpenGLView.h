//
//  MyOpenGLView.h
//  Minecrap
//
//  Created by Jeffry Degrande on 1/28/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Camera.h"
#import "World.h"

@interface MyOpenGLView : NSOpenGLView
{
    Camera *camera;
    World *world;
    
    long lastTicks;
}

@end

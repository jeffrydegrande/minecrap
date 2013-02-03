//
//  Camera.h
//  Golden Triangle
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import <Foundation/Foundation.h>


#define PI 3.141592654f


@interface Camera : NSObject

- (void) update;
- (void) updateWithPoint: (NSPoint)point;

- (void) moveLeft;
- (void) moveRight;
- (void) moveForward;
- (void) moveBackward;
- (void) strafeLeft;
- (void) strafeRight;

- (void) lookUp;
- (void) lookDown;

- (NSString *) stringFromPosition;

@end

//
//  Crosshair.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/9/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Crosshair : NSObject

+ (void) setup:(int) screenWidth :(int)screenHeight;
+ (void) render;

@end

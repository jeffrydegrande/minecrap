//
//  Block.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/9/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Block : NSObject

+ (void) setup;
+ (void) render:(GLubyte)blockType;
@end

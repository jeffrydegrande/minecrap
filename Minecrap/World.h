//
//  World.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Chunk.h"

@interface World : NSObject
{
    NSMutableArray *rowOfChunks;
}

@property int seed;

- (id)initWithSeed:(int)worldSeed;
- (int) render;
@end

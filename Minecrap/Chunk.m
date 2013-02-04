//
//  Chunk.m
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#import "Chunk.h"

#include "minecrap.h"
#include "simplex.h"

@implementation Chunk

- (id)init
{
    self = [super init];
    if (self) {
        [self build];
    }
    return self;
}

- (GLubyte) blockAt:(int)x :(int)y :(int)z {
    return blocks[x][y][z];
}

- (bool) isExposedToAir:(int)x :(int)y :(int)z {
    return (blocks[x+1][y][z] == AIR || blocks[x-1][y][z] == AIR
            || blocks[x][y+1][z] == AIR || blocks[x][y-1][z] == AIR
            || blocks[x][y][z+1] == AIR || blocks[x][y][z-1] == AIR);
}


- (void) build {
    [self buildWithSimplexNoise];
    [self addDirt];
    [self addWaterLevel];
    [self addBedrock];
    [self addMarkersAtTerrainBoundaries];
}

- (int) buildWithSimplexNoise {
    int blockCount = 0;
    float caves, center_falloff, plateau_falloff, density;
    
    int caveBlocks = 0;
    
    foreach_xyz {
        float xf = (float)x/(float)CHUNKX;
        float yf = (float)y/(float)CHUNKY;
        float zf = (float)z/(float)CHUNKZ;
        
        if(yf <= 0.8){
            plateau_falloff = 1.0;
        }
        else if(0.8 < yf && yf < 0.9){
            plateau_falloff = 1.0-(yf-0.8)*10.0;
        }
        else{
            plateau_falloff = 0.0;
        }
        
        center_falloff = 0.2/(
                              pow((xf-0.5)*1.5, 2) +
                              pow((yf-1.5)*0.2, 2) +
                              pow((zf-0.5)*1.5, 2)
                              );
        
        caves = pow(simplex_noise(1, xf*5, yf*5, zf*5), 3);
        density = simplex_noise(5, xf, yf*0.5, zf) * center_falloff * plateau_falloff;
        
        density *= pow(
                       noise((xf+1)*3.0, (yf+1)*3.0, (zf+1)*3.0)+0.4, 1.8
                       );
        
        if(caves < 0.5){
            density = 0;
            blocks[x][y][z] = RED;
            caveBlocks++;
            blockCount++;
            continue;
        }
        
        blocks[x][y][z] = (density > 3.1 ? ROCK : AIR);
        blockCount++;
    }
    endforeach
    
    NSLog(@"%d of %d are cave blocks", caveBlocks, blockCount);
    
    
    return blockCount;
}

- (void) summarizeTerrain {
    int rock =0, dirt=0, empty=0;
    int blockCount = 0;
    
    foreach_xyz {
        switch(blocks[x][y][z]) {
            case ROCK: rock++; break;
            case DIRT: dirt++; break;
            default: empty++;
        }
        blockCount++;
    } endforeach
    
    NSLog(@"%d blocks in this world. %d stone, %d dirt, %d empty", blockCount, rock, dirt, empty);
}



- (void) addMarkersAtTerrainBoundaries {
    blocks[0][0][0] = RED;
    blocks[0][0][CHUNKZ-1] = RED;
    blocks[CHUNKX-1][0][0] = RED;
    blocks[CHUNKX-1][0][CHUNKZ-1] = RED;
    
    blocks[0][CHUNKY-1][0] = RED;
    blocks[0][CHUNKY-1][CHUNKZ-1] = RED;
    blocks[CHUNKX-1][CHUNKY-1][0] = RED;
    blocks[CHUNKX-1][CHUNKY-1][CHUNKZ-1] = RED;
}

- (int) addDirt {
    foreach_xyz {
        int block = blocks[x][y][z];
        int ontop = blocks[x][y+1][z];
        if (block == ROCK && ontop == AIR) {
            blocks[x][y][z] = DIRT;
        }
    } endforeach
    return 0;
}

- (int) addBedrock {
    int blockCount = 0;
    for (int x=0; x<CHUNKX; x++) {
        for (int z=0; z<CHUNKZ; z++ ) {
            blocks[x][0][z] = ROCK;
        }
    }
    return blockCount;
}

- (int) addWaterLevel {
    int blockCount = 0;
    for (int x=0; x<CHUNKX; x++) {
        for (int z=0; z<CHUNKZ; z++ ) {
            blocks[x][WATER_LEVEL][z] = WATER;
        }
    }
    return blockCount;
}

@end
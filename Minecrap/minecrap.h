//
//  minecrap.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#ifndef Minecrap_minecrap_h
#define Minecrap_minecrap_h

#define CHUNKX 16
#define CHUNKY 128
#define CHUNKZ 16

#define WATER_LEVEL 62

#define AIR   0
#define ROCK  1
#define GEMS  2
#define DIRT  3
#define GRASS 4
#define LAVA  5
#define WATER 6

#define RED   7




#define foreach_xyz \
    for (int x=0; x<CHUNKX; x++) { \
        for (int y=0; y< CHUNKY; y++) { \
            for (int z=0; z < CHUNKZ; z++)

#define endforeach }}

#endif

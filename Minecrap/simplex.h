//
//  simplex.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/3/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#ifndef Minecrap_simplex_h
#define Minecrap_simplex_h


float noise(float xin, float yin, float zin);
float noise2D(float xin, float zin);
float simplex_noise(int octaves, float x, float y, float z);

#endif

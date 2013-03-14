#include "Terrain.h"
#include "simplex.h"
#include <cmath>

///////////////////////////////////////////////////////////
// helpers
///////////////////////////////////////////////////////////
float terrainNoise(float x, float z, float frequency, float amplitude) {
    float result = 0;
    float amp = 1.0f;
    float freq = 1.0f;
    float max = 0.0f;

    x*= 1/64.0f;
    z*= 1/64.0f;

    for (int i = 0; i < 8; i++) {
        result += noise2D(x * freq, z*freq) * amp;
        max += amp;
        freq *= frequency;
        amp *= amplitude;
    }

    result /= max;

    return result;
}

///////////////////////////////////////////////////////////
// RockyWorldTerrain
///////////////////////////////////////////////////////////
void RockyWorldTerrain::generate(Chunk &chunk)
{
    float caves, center_falloff, plateau_falloff, density;
    float xf, yf, zf;

    for (int y=0; y < CHUNKY; y++) {
        for (int x=0; x < CHUNKX; x++) {
            for (int z=0; z < CHUNKZ; z++) {
                xf = (float)x / (float)CHUNKX;
                yf = (float)y / (float)CHUNKY;
                zf = (float)z / (float)CHUNKZ;

                if(yf <= 0.8) {
                    plateau_falloff = 1.0;
                }
                else if(0.8 < yf && yf < 0.9){
                    plateau_falloff = 1.0-(yf-0.8)*10.0;
                }
                else{
                    plateau_falloff = 0.0;
                }

                center_falloff = 0.1/(
                    pow((xf-0.5)*1.5, 2) +
                    pow((yf-1.0)*0.8, 2) +
                    pow((zf-0.5)*1.5, 2)
                );
                caves = pow(simplex_noise(1, xf*5, yf*5, zf*5), 3);
                density = (
                    simplex_noise(5, xf, yf*0.5, zf) *
                    center_falloff *
                    plateau_falloff
                );
                density *= pow(
                    noise((xf+1)*3.0, (yf+1)*3.0, (zf+1)*3.0)+0.4, 1.8
                );
                if(caves<0.5){
                    density = 0;
                }
                chunk.setBlock(x, y, z, density>3.1 ? ROCK : 0);
            }
        }
    }
}


///////////////////////////////////////////////////////////
// SimpleTerrain
///////////////////////////////////////////////////////////

void SimpleTerrain::generate(Chunk &chunk)
{
    foreach_xz {
        float maxHeight = terrainNoise(
            (float)(x + chunk.getSeed() + (chunk.getWorldX() << 4)),
            (float)(z + chunk.getSeed() + (chunk.getWorldZ() << 4)),
            0.5f,
            0.5f) * 32 + WATER_LEVEL;

        assert(maxHeight <= CHUNKY && maxHeight >= 0);

		chunk.setBlock(x, (int)maxHeight, z, ROCK);
		chunk.setBlock(x, (int)maxHeight - 1, z,  ROCK);

    } endforeach;

    addDirt(chunk);
    addWaterLevel(chunk);
    addSand(chunk); // need to do this after adding water
    addBedrock(chunk);
    // addMarkersAtBoundaries();
}

void SimpleTerrain::addMarkersAtBoundaries(Chunk &chunk) {
    chunk.setBlock(0, 0, 0, RED);
    chunk.setBlock(0, 0, CHUNKZ-1, RED);
    chunk.setBlock(CHUNKX-1, 0, 0, RED);
    chunk.setBlock(CHUNKX-1, 0, CHUNKZ-1, RED);

    chunk.setBlock(0, CHUNKY-1, 0, RED);
    chunk.setBlock(0, CHUNKY-1, CHUNKZ-1, RED);
    chunk.setBlock(CHUNKX-1, CHUNKY-1, 0, RED);
    chunk.setBlock(CHUNKX-1, CHUNKY-1, CHUNKZ-1, RED);
}

void SimpleTerrain::addDirt(Chunk &chunk) {
	for (int y=0; y<CHUNKY; y++) {
		for (int z=0; z<CHUNKZ; z++) {
			for (int x=0; x<CHUNKX; x++) {
				if (chunk.getBlock(x,y,z) == ROCK && chunk.getBlock(x,y+1,z) == AIR) {
					chunk.setBlock(x,y,z, DIRT);
				}
			}
        }
    }
}

void SimpleTerrain::addBedrock(Chunk &chunk) {
    foreach_xz {
        chunk.setBlock(x,0,z, BEDROCK);
    } endforeach;
}

void SimpleTerrain::addSand(Chunk &chunk) {
    foreach_xz {
        GLubyte block = chunk.getBlock(x,WATER_LEVEL,z);
        if (block != WATER && block != AIR)
            chunk.setBlock(x, WATER_LEVEL, z, SAND);
    } endforeach;
}

void SimpleTerrain::addWaterLevel(Chunk &chunk) {
    foreach_xz {
        int y = CHUNKY - 1;
        while (y > 0 && chunk.getBlock(x,y,z) == AIR) {
            if (y <= WATER_LEVEL)
                chunk.setBlock(x,y,z, WATER);
            y--;
        }
    } endforeach;
}


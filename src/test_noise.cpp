#include "simplex.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>

#define X 4
#define Y 128
#define Z 4

int main()
{
    float density, caves;
    (void)density;
    (void)caves;

    for (int y=Y-1; y>=0; y--)
    {
        printf("%d: ", y);
        for (int x=0; x<X; x++) {
            for (int z=0; z<Z; z++) {
                float xf = (float)x / (float)X;
                float yf = (float)y / (float)Y;
                float zf = (float)z / (float)Z;

                // density = simplex_noise(5, xf, yf*0.5, zf);
                density = noise((xf+1.0f) * 3.0f, (yf+1.0f) * 3.0f, (zf+1.0f) * 3.0f);
                density = yf;
                caves = pow(simplex_noise(1, xf*5, yf*5, zf*5), 3);

                if (caves < 0)
                    density = 0.0f;

                printf ("%0.2f ", density);
            }
        }
        printf("\n");
    }

    return 0;
}

#ifndef MINECRAP_IMAGE_H
#define MINECRAP_IMAGE_H

#include "Vec.h"
#include <string>
#include <cstdio>
#include <IL/il.h>

class Image {
    char *buffer;
    Vec2 size;
    ILuint image;

    public:
        Image();
        Image(const char *filename);
        ~Image();

        void load(void *buffer, unsigned int size);
        void save(const char *filename);

        const char *data_ptr() const;

        int width() const;
        int height() const;
        void print() const;

    private:

        void loadImage(const char *filename);
};

void Image_Init();

inline int Image::width() const {
    return (int)size.x;
}

inline int Image::height() const {
    return (int)size.y;
}

inline void Image::print() const {
    size.print();
    printf( "buffer size: %d\n", (int)size.x * (int)size.y * 4);
}

#endif

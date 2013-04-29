#ifndef MINECRAP_TEXTURE_H
#define MINECRAP_TEXTURE_H

#include <string>
#include "minecrap.h"

class Image;
class Texture {
    GLenum target;
    GLuint texture;
    std::string filename;
    Image *image;

    public:
        Texture();
        ~Texture();

        bool load(GLenum target, Image *image);
        bool load();
        void bind(GLenum unit);
};

#endif

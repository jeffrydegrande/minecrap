#include "Texture.h"
#include "minecrap.h"
#include "Image.h"

Texture::Texture()
{
    target  = INVALID_OPENGL_VALUE;
    texture = INVALID_OPENGL_VALUE;
}

Texture::~Texture()
{
    if (image) {
        delete image;
        image = NULL;
    }
}

bool Texture::load()
{
    image = new Image(filename.c_str());
    if (image == NULL)
        return false;

    glGenTextures(1, &texture);
    glBindTexture(target, texture);

    glTexImage2D(target, 0, GL_RGB, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data_ptr());
    glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

bool Texture::load(GLenum target, Image *image)
{
    if (image == NULL)
        return false;

    this->target = target;

    glGenTextures(1, &texture);
    glBindTexture(target, texture);

    glTexImage2D(target, 0, GL_RGBA, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data_ptr());
    glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::bind(GLenum unit)
{
    assert(target != INVALID_OPENGL_VALUE);
    assert(texture != INVALID_OPENGL_VALUE);
    (void)unit;
    //glActiveTexture(unit);
    glBindTexture(target, texture);
}

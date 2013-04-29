#include "Image.h"
#include "minecrap.h"
#include <cerrno>

#include <IL/il.h>
#include <IL/ilu.h>

#define CHECK_IL_ERROR(func) \
    { \
        ILenum error; \
        while ((error = ilGetError()) != IL_NO_ERROR) { \
            printf("%s: error: %d\n", func, error); \
            exit(1); \
        } \
    }

void Image_Init()
{
    ilInit();
}

Image::Image()
    :buffer(NULL)
{
}

Image::Image(const char *filename)
    : buffer(NULL)
{
    loadImage(filename);
}

Image::~Image()
{
    if (buffer != NULL) {
        delete [] buffer;
    }
    ilDeleteImages(1, &image);
}

void Image::load(void *lump, unsigned int lumpSize)
{
    ILboolean ok;
    ilGenImages(1, &image);
    CHECK_IL_ERROR("ilGenImages")

    ilBindImage(image);
    CHECK_IL_ERROR("ilBindImage")

    ok = ilLoadL(IL_PNG, lump, lumpSize);
    if (!ok) {
        CHECK_IL_ERROR("ilLoadL")
    }

    this->size.x = ilGetInteger(IL_IMAGE_WIDTH);
    this->size.y = ilGetInteger(IL_IMAGE_HEIGHT);

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    buffer = new char[(int)this->size.x * (int)this->size.y * 4];
    ilCopyPixels(0, 0, 0, this->size.x, this->size.y, 1, IL_RGBA, IL_UNSIGNED_BYTE, buffer);
}

void Image::save(const char *filename)
{
    ilSaveImage(filename);
}

void Image::loadImage(const char *filename)
{
    ILboolean ok;
    ilGenImages(1, &image);
    CHECK_IL_ERROR("ilGenImages")

    ilBindImage(image);
    CHECK_IL_ERROR("ilBindImage")

    ok = ilLoadImage(filename);
    if (!ok) {
        CHECK_IL_ERROR("ilLoadImage")
    }

    size.x = ilGetInteger(IL_IMAGE_WIDTH);
    size.y = ilGetInteger(IL_IMAGE_HEIGHT);

    buffer = new char[(int)size.x * (int)size.y * 4];
    ilCopyPixels(0, 0, 0, size.x, size.y, 1, IL_RGBA, IL_UNSIGNED_BYTE, buffer);
}

const char *Image::data_ptr() const
{
    return buffer;
}

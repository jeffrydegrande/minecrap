#include "Image.h"
#include "minecrap.h"
#include <cerrno>

#include <IL/il.h>
#include <IL/ilu.h>


static bool ilInitialized = false;


Image::Image(const char *filename)
    : buffer(NULL)
{
    loadImage(filename);
}

Image::~Image()
{
    if (buffer != NULL) {
        delete buffer;
    }
}

void Image::loadImage(const char *filename)
{
    if (!ilInitialized) {
        ilInit();
        ilInitialized = true;
    }

    ILboolean ok;
    ILuint images;

    ilGenImages(1, &images);
    ilBindImage(images);
    ok = ilLoadImage(filename);
    if (!ok) {
        ILenum error;
        while ((error = ilGetError()) != IL_NO_ERROR) {
            printf("%d\n", error);
        }
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

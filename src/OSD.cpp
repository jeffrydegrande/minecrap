#include "OSD.h"
#include "minecrap.h"
#include "Text.h"

OSD *osd = NULL;

void OSD_Init()
{
    TextInit();
    osd = new OSD();
}

void OSD_Destroy()
{
    delete osd;
    osd = NULL;
}

OSD::OSD():
    currentLine(1),
    width(0)
{
}

OSD::~OSD() {}

void OSD::setWidth(int width) {
    this->width = width;
}

void OSD::write(const char *fmt, ...)
{
    char text[2048];
    va_list ap;
    if (fmt == NULL)
        return;

    va_start(ap, fmt);
    vsprintf(text, fmt, ap);
    va_end(ap);
    TextWrite(this->width / 2 - 40, currentLine * 13, text);
    currentLine++;
}

void OSD::reset() {
    currentLine = 1;
}

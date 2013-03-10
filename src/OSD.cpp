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

OSD::OSD(): width(0)
{ }

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
    lines.push_back(text);
}

void OSD::render() {
    int line = 1;
    for(size_t i=0; i<lines.size(); i++) {
        TextWrite(this->width / 2 - 40, line * 13, lines[i].c_str());
        line++;
    }
    lines.clear();
}

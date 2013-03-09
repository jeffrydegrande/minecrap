#ifndef MINECRAP_OSD_H
#define MINECRAP_OSD_H

#include <vector>
#include <string>

class OSD {
    int currentLine;
    int width;

    std::vector<std::string> lines;

    public:
        void setWidth(int width);
        void write(const char *fmt, ...);
        void update(float elapsed);
        void render();

        OSD();
        ~OSD();

};

void OSD_Init();
void OSD_Destroy();
extern OSD *osd;

#endif

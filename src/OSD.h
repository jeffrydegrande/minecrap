#ifndef MINECRAP_OSD_H
#define MINECRAP_OSD_H

class OSD {
    int currentLine;
    int width;

    public:
        void setWidth(int width);
        void write(const char *fmt, ...);
        void reset();

        OSD();
        ~OSD();

};

void OSD_Init();
void OSD_Destroy();
extern OSD *osd;

#endif

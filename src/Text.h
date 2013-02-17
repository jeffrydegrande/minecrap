#ifndef MINECRAP_TEXT_H
#define MINECRAP_TEXT_H

#include <string>

void TextInit();
void TextWrite(int x, int y, const char *txt);
void TextWrite(int x, int y, const std::string fmt, ...);
int TextCharHeight();
int TextCharWidth();

#endif

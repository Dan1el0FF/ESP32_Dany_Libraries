#ifndef _MINIGFX_H
#define _MINIGFX_H

#include "SSD1315.h"
#include "Font5x7.h"
#include "Icons.h"

class MiniGFX {
public:
    // Se vincula con un objeto de hardware SSD1315
    MiniGFX(SSD1315* display);

    void drawChar(int x, int y, char c, bool color);
    void drawText(int x, int y, const char* text, bool color);
    void drawRect(int x, int y, int w, int h, bool color);
    void drawBitmap(int x, int y, const uint8_t* bitmap, int w, int h, bool color);
    void drawLine(int x0, int y0, int x1, int y1, bool color);
private:
    SSD1315* _display;
};

#endif
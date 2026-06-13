#include "MiniGFX.h"

MiniGFX::MiniGFX(SSD1315* display) : _display(display) {}

void MiniGFX::drawChar(int x, int y, char c, bool color) {
    if (c < 32 || c > 126) return;
    
    uint16_t offset = (c - 32) * 5;
    for (int i = 0; i < 5; i++) {
        uint8_t line = font5x7[offset + i];
        for (int j = 0; j < 7; j++) {
            if (line & (1 << j)) {
                _display->drawPixel(x + i, y + j, color);
            }
        }
    }
}

void MiniGFX::drawText(int x, int y, const char* text, bool color) {
    int currX = x;
    while (*text) {
        drawChar(currX, y, *text++, color);
        currX += 6; // Ancho del caracter + espacio
        if (currX > 122) break; // Límite de la pantalla
    }
}

void MiniGFX::drawRect(int x, int y, int w, int h, bool color) {
    for (int i = x; i < x + w; i++) {
        _display->drawPixel(i, y, color);         // Arriba
        _display->drawPixel(i, y + h - 1, color); // Abajo
    }
    for (int j = y; j < y + h; j++) {
        _display->drawPixel(x, j, color);         // Izquierda
        _display->drawPixel(x + w - 1, j, color); // Derecha
    }
}

void MiniGFX::drawBitmap(int x, int y, const uint8_t* bitmap, int w, int h, bool color) {
    int byteWidth = (w + 7) / 8; // bytes por fila

    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            uint8_t byte = bitmap[j * byteWidth + (i / 8)];
            if (byte & (0x80 >> (i & 7))) {
                _display->drawPixel(x + i, y + j, color);
            }
        }
    }
}

void MiniGFX::drawLine(int x0, int y0, int x1, int y1, bool color) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        _display->drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}
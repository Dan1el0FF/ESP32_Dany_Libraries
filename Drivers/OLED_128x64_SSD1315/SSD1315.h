#ifndef _SSD1315_H
#define _SSD1315_H

#include "MultipleI2C.h"
#include <vector>
#include <cstring>

#define SSD1315_WIDTH  128
#define SSD1315_HEIGHT 64

class SSD1315 {
public:
    // Constructor requiere un puntero a una instancia de MultipleI2C ya configurada
    SSD1315(MultipleI2C* i2c_bus);

    bool init();
    void update();
    void clear();
    void drawPixel(int x, int y, bool color);

private:
    MultipleI2C* _i2c;
    uint8_t _buffer[SSD1315_WIDTH * SSD1315_HEIGHT / 8];

    void sendCommand(uint8_t command);
    void sendData(uint8_t* data, size_t size);
};

#endif
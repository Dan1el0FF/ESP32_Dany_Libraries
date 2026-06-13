#include "SSD1315.h"

SSD1315::SSD1315(MultipleI2C* i2c_bus) : _i2c(i2c_bus) {
    clear();
}

void SSD1315::sendCommand(uint8_t command) {
    uint8_t data[2] = {0x00, command}; // 0x00 indica que sigue un comando
    _i2c->write(data, 2);
}

void SSD1315::sendData(uint8_t* data, size_t size) {
    // Para SSD1315, el prefijo 0x40 indica que siguen datos de RAM
    uint8_t* buffer = new uint8_t[size + 1];
    buffer[0] = 0x40;
    memcpy(&buffer[1], data, size);
    _i2c->write(buffer, size + 1);
    delete[] buffer;
}

bool SSD1315::init() {
    // Secuencia de inicialización estándar para SSD1315/SSD1306
    sendCommand(0xAE); // Apagar pantalla
    sendCommand(0xD5); // Set Display Clock Divide Ratio
    sendCommand(0x80);
    sendCommand(0xA8); // Set Multiplex Ratio
    sendCommand(0x3F);
    sendCommand(0xD3); // Set Display Offset
    sendCommand(0x00);
    sendCommand(0x40); // Set Display Start Line
    sendCommand(0x8D); // Charge Pump
    sendCommand(0x14); // Enable Charge Pump
    sendCommand(0x20); // Memory Addressing Mode
    sendCommand(0x00); // Horizontal Addressing Mode
    sendCommand(0xA1); // Segment Re-map (Invertir horizontalmente)
    sendCommand(0xC8); // COM Output Scan Direction
    sendCommand(0xDA); // Set COM Pins Hardware Configuration
    sendCommand(0x12);
    sendCommand(0x81); // Set Contrast
    sendCommand(0xCF);
    sendCommand(0xA4); // Entire Display On
    sendCommand(0xA6); // Normal Display
    sendCommand(0xAF); // Encender pantalla
    
    update();
    return true;
}

void SSD1315::clear() {
    memset(_buffer, 0, sizeof(_buffer));
}

void SSD1315::drawPixel(int x, int y, bool color) {
    if (x >= SSD1315_WIDTH || y >= SSD1315_HEIGHT || x < 0 || y < 0) return;

    if (color)
        _buffer[x + (y / 8) * SSD1315_WIDTH] |= (1 << (y % 8));
    else
        _buffer[x + (y / 8) * SSD1315_WIDTH] &= ~(1 << (y % 8));
}

void SSD1315::update() {
    sendCommand(0x21); // Set Column Address
    sendCommand(0);
    sendCommand(127);
    sendCommand(0x22); // Set Page Address
    sendCommand(0);
    sendCommand(7);
    
    sendData(_buffer, sizeof(_buffer));
}

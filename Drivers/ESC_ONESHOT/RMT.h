#ifndef __ESCOSHOT_H__
#define __ESCOSHOT_H__


#include "driver/rmt.h"
#include "driver/gpio.h"


class ESCOShot {
private:
rmt_channel_t channel;
gpio_num_t pin;


// Parámetros OneShot125
uint16_t min_us = 125; // pulso mínimo
uint16_t max_us = 250; // pulso máximo
uint16_t period_us = 500; // periodo (2 kHz)


rmt_item32_t item[2]; // HIGH + LOW


void buildFrame(uint8_t power);


public:
ESCOShot();


// channel: RMT_CHANNEL_0 .. RMT_CHANNEL_7
// pin: GPIO de salida
void begin(rmt_channel_t channel, gpio_num_t pin);


// power: 0–100 (%)
void write(uint8_t power);
};


#endif

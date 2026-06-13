// =============================
// ESCOShot_LEDC.h
// OneShot125 usando LEDC (ESP32)
// Escala 0–100 (% potencia)
// NOTA: menos preciso que RMT, pero funcional
// =============================


#ifndef __ESCOSHOT_LEDC_H__
#define __ESCOSHOT_LEDC_H__


#include "driver/ledc.h"
#include "driver/gpio.h"


class ESCOShot_LEDC {
private:
int channel;
gpio_num_t pin;


// Parámetros OneShot125
uint32_t freq_hz = 2000; // 2 kHz
uint16_t min_us = 125;
uint16_t max_us = 250;


uint32_t duty_min;
uint32_t duty_max;


public:
ESCOShot_LEDC();


// channel: LEDC_CHANNEL_0 .. LEDC_CHANNEL_7
// pin: GPIO salida
void begin(int channel, gpio_num_t pin);


// power: 0–100
void write(uint8_t power);
};


#endif

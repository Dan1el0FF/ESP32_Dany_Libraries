// =============================
// ESCOShot_LEDC.cpp
// =============================


#include "ESCOShot_LEDC.h"


ESCOShot_LEDC::ESCOShot_LEDC() {}


void ESCOShot_LEDC::begin(int _channel, gpio_num_t _pin) {
channel = _channel;
pin = _pin;


// Resolución baja para poder subir frecuencia
ledc_timer_config_t timer = {};
timer.speed_mode = LEDC_HIGH_SPEED_MODE;
timer.timer_num = LEDC_TIMER_0;
timer.duty_resolution = LEDC_TIMER_10_BIT; // 0–1023
timer.freq_hz = freq_hz;
timer.clk_cfg = LEDC_AUTO_CLK;
ledc_timer_config(&timer);


ledc_channel_config_t ch = {};
ch.gpio_num = pin;
ch.speed_mode = LEDC_HIGH_SPEED_MODE;
ch.channel = (ledc_channel_t)channel;
ch.timer_sel = LEDC_TIMER_0;
ch.duty = 0;
ch.hpoint = 0;
ledc_channel_config(&ch);


// Calcular duty equivalente a 125–250 µs
// Periodo = 1/f = 500 µs
float period_us = 1e6 / freq_hz;


duty_min = (uint32_t)((min_us / period_us) * 1023.0f);
duty_max = (uint32_t)((max_us / period_us) * 1023.0f);


write(0);
}


void ESCOShot_LEDC::write(uint8_t power) {
if (power > 100) power = 100;


uint32_t duty = duty_min + (power * (duty_max - duty_min)) / 100;


ledc_set_duty(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)channel, duty);
ledc_update_duty(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)channel);
}

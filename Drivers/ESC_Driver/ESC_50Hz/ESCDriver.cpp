#include "ESCDriver.h"

ESCDriver::ESCDriver(){};

void ESCDriver::setup(int _channel, gpio_num_t _pin){

    channel = _channel;
    pin = _pin;

    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_12_BIT, //resolucion de 0–4095 
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false,
    };
    ledc_timer_config(&ledc_timer); 

    ledc_channel = {
        .gpio_num = pin,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = (ledc_channel_t)channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
        .flags = 0,
    };
    ledc_channel_config(&ledc_channel);
    
}

void ESCDriver::setDuty(float _duty){
    duty = _duty;
    if(duty<0)duty=0;
    if(duty>100)duty=100;
    output = (int)(205 + 2.05*duty);

    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, output);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);


}

float ESCDriver::getDuty(){
    return duty;
}
int ESCDriver::getOutput(){
    return output;
}
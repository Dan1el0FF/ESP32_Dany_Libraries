#ifndef __ESCDRIVER_H__
#define __ESCDRIVER_H__

#include "driver/ledc.h"

class ESCDriver {

    private:  //Atributos

    int channel,output;
    float duty;
    gpio_num_t pin;
    ledc_channel_config_t ledc_channel;

    public:  //Metodos

    ESCDriver(); //constructor
    void setup(int _channel, gpio_num_t _pin);
    void setDuty(float _duty); //numero entre 0-100 con una resolucion de 12 bits
    float getDuty();
    int getOutput();
};
#endif // __ESCDRIVER_H__
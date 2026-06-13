#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include <esp32c2/rom/ets_sys.h>

class UltraSonic
{
private: 
                               // atributos
    int Trig;
    int Echo;

public:                                 // metodos
    UltraSonic(int Trig, int Echo);     // constructor
    void configureUltrasonic();
    float getDistance();
};

#endif // ULTRASONIC_H

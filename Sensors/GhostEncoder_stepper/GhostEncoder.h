#ifndef __GHOSTENCODER_H__
#define __GHOSTENCODER_H__

#include <esp_timer.h>
#include <esp_attr.h>
#include <cmath>
#include <driver/gpio.h>

class GhostEncoder
{
private: //atributos


    int steps_per_mm;
    int steps_per_rev;
    float desired_pos; //mm
    volatile float current_pos; //mm
    int direction; //+1,0,-1
    volatile int counts;
    bool flag=false;
    bool moving= false;
    int steps_done;
    int steps_to_move;
    gpio_num_t step_pin;
    gpio_num_t dir_pin;
    int frqz;
    void IRAM_ATTR Timer_handler();
    esp_timer_handle_t   timerHandle;


public: //metodos

    GhostEncoder();
    void setup(gpio_num_t _step_pin, gpio_num_t _dir_pin, int _steps_per_mm=100,int _steps_per_rev=200);
    void setwaypoint(float _desired_pos,int _frqz);
    float getPos();
    void reset();
    float getspeed();
    void stop();
};

#endif // __GHOSTENCODER_H__

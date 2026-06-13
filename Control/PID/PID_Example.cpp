#include <stdio.h>
#include "PID.h"
extern "C" void app_main(void)
{
    esp_task_wdt_deinit(); //desactivamos el watchdog

    float gains[]={1.0f,0.0f,0.0f}; //arreglo de ganancias.

    uint64_t dt_us = 10000; 
    uint64_t current_time,prev_time;
    float ref = 0;

    PID control;

    control.setup(gains,dt_us/1000000.0f);

    prev_time = esp_timer_get_time(); 

    while(true){//metodo polling
        current_time = esp_timer_get_time();
        if(current_time-prev_time>= dt_us){
            prev_time = current_time;
            for(int i=0; i<4;i++){
            motor.setPower(control.calculate(ref,sensor.read()));
            }
        }
    }
}

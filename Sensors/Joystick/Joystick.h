#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <driver/gpio.h>
#include "driver/adc.h"
#include "stdio.h"
#include "cmath"
#include <Freertos/FreeRTOS.h>
#include <freertos/task.h>


#define ADC_WIDTH ADC_WIDTH_BIT_12
class Joystick{

    private: //atributos 

    adc1_channel_t VRX_Channel;
    adc1_channel_t VRY_Channel;
    gpio_num_t SW_Pin;
    void calibrateX(adc1_channel_t(Ch));
    void calibrateY(adc1_channel_t(Ch));  
    int invert_valueX,invert_valueY,center,dead,center1,dead1;
    public: //metodos

    Joystick(); //constructor
    void setupVRX(adc1_channel_t(_VRX_Channel),bool en_calibrate = false, int _invert_valueX = 4095); // Channel, enable automatic calibration, invert Axis
    void setupVRY(adc1_channel_t(_VRY_Channel),bool en_calibrate = false, int _invert_valueY = 4095); // Channel, enable automatic calibration, invert Axis
    void setupSW(gpio_num_t(_SW_Pin),bool enable_pullup = true); 
    int readVRX();
    int readVRY();
    bool readSW();
    void set_paramsX(int centerX,int deadzoneX);
    void set_paramsY(int centerY,int deadzoneY);
    

};


/*
-Al indicar en los setup el ultimo parametro, _invert_valueX o _invertvalueY, debemos colocar 4095 o 0 lo cual
permite poder colocar el joystick en diferentes posiciones.
-by Dany
*/
#endif // __JOYSTICK_H__
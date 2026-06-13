#include "Joystick.h"

Joystick::Joystick(){}

void Joystick::setupVRX(adc1_channel_t(_VRX_Channel),bool en_calibrate,int _invert_valueX){
    VRX_Channel = _VRX_Channel;
    if(_invert_valueX!=4095 && _invert_valueX != 0) _invert_valueX = 4095; //default
    invert_valueX = _invert_valueX;
    adc1_config_channel_atten(VRX_Channel, ADC_ATTEN_DB_12);
    adc1_config_width(adc_bits_width_t(12));
    if(en_calibrate)calibrateX(VRX_Channel);
}

void Joystick::setupVRY(adc1_channel_t(_VRY_Channel),bool en_calibrate,int _invert_valueY){
    VRY_Channel = _VRY_Channel;
    if(_invert_valueY != 4095 && _invert_valueY != 0) _invert_valueY = 4095; //default
    invert_valueY = _invert_valueY;
    adc1_config_channel_atten(VRY_Channel, ADC_ATTEN_DB_12);
    adc1_config_width(adc_bits_width_t(12));
    if(en_calibrate)calibrateY(VRY_Channel);
}

void Joystick::setupSW(gpio_num_t(_SW_Pin),bool enable_pullup){
    SW_Pin = _SW_Pin;
    gpio_set_direction(SW_Pin,GPIO_MODE_INPUT);
    if(enable_pullup)gpio_set_pull_mode(SW_Pin, GPIO_PULLUP_ONLY);
}

int Joystick::readVRX(){
    int value = abs(adc1_get_raw(VRX_Channel)-invert_valueX);
    if(value > 4095) value = 4095;
    int delta = value - center;
    if (abs(delta) <= dead) return 0;
    if (delta > 0) return (delta * 100) / (4095 - center);
    else return delta * 100 / center;    
}

int Joystick::readVRY(){
    int value = abs(adc1_get_raw(VRY_Channel)-invert_valueY);
    if(value > 4095) value = 4095;
    int delta = value - center1;
    if (abs(delta) <= dead1) return 0;
    if (delta > 0) return (delta * 100) / (4095 - center1);
    else return delta * 100 / center1; 
}

bool Joystick::readSW(){
    return gpio_get_level(SW_Pin);
}

void Joystick::calibrateX(adc1_channel_t(Ch)){
    printf("Calibrando Joystick_X no lo mueva...\n");
    int value = abs(adc1_get_raw(Ch)-invert_valueX);
    int min = value;
    int max = value;
    long sum = 0;
    int n = 1300;
    for(int i = 0; i < n; i++){
        value = abs(adc1_get_raw(Ch)-invert_valueX);
        sum += value;
        if(value>max)max = value;
        if(value<min)min = value;
        vTaskDelay(pdMS_TO_TICKS(6));
    }
    center = sum/n;
    int r1 = abs(max-center);
    int r2 = abs(center-min);
    if(r1>r2) dead = r1;
    else dead = r2;
    printf("Calibracion en X Completada: Center: %d DeadZone: %d\n",center,dead);
}

void Joystick::calibrateY(adc1_channel_t(Ch)){
    printf("Calibrando Joystick_Y no lo mueva...\n");
    int value = abs(adc1_get_raw(Ch)-invert_valueY);
    int min = value;
    int max = value;
    long sum = 0;
    int n = 1300;
    for(int i = 0; i < n; i++){
        value = abs(adc1_get_raw(Ch)-invert_valueY);
        sum += value;
        if(value>max)max = value;
        if(value<min)min = value;
        vTaskDelay(pdMS_TO_TICKS(6));
    }
    center1 = sum/n;
    int r1 = abs(max-center1);
    int r2 = abs(center1-min);
    if(r1>r2) dead1 = r1;
    else dead1 = r2;
    printf("Calibracion en Y Completada: Center: %d DeadZone: %d\n",center1,dead1);
}

void Joystick::set_paramsX(int centerX,int deadzoneX){
    center = centerX;
    dead = deadzoneX;
}

void Joystick::set_paramsY(int centerY,int deadzoneY){
    center1 = centerY;
    dead1 = deadzoneY;
}



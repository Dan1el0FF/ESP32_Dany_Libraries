//PWM_PIN_ACTUALIZADA_VERSION_SERVOZ_CONTROLLER
#ifndef ESP32SERVO_H
#define ESP32SERVO_H
#include "driver/ledc.h"

class ESP32Servo {
    private:
        int channel;
        int pin;
        int angle;
        ledc_channel_config_t ledc_channel;

    public:
        ESP32Servo(int channel, int pin);  // Constructor
        void configurePwm();
        void setAngle(int angle);
        int getAngle();

};

#endif

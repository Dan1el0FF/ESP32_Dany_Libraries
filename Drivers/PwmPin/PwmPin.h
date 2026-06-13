//PWM_PIN_ACTUALIZADA_VERSION_FRQZ_CHANGING
#ifndef PWMPIN_H
#define PWMPIN_H
#include "driver/ledc.h"

class PwmPin {
    private:
        int channel;
        int pin;
        int power;
        ledc_channel_config_t ledc_channel;
        float frqz;

    public:
        PwmPin(int channel, int pin,float frqz=5000);  // Constructor
        void configurePwm();
        void setPower(int power);
        int getPower();
        void setFrqz(float newFrqz);
        float getFrqz();

};

#endif

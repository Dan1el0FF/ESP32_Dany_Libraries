//PID version windup y reset integral
#ifndef PID_H
#define PID_H

#include <esp_timer.h>
#include <esp_task_wdt.h>

class PID {

    public:
    PID();
    void setup(float gains[3],float dt,float windup = 100.0f, bool circular = false);
    float calculate(float ref,float measurement);
    void resetIntegral();
    void update_dt(float dt); //en segundos
    void update_gains(float new_gains[3]);

    private:

    float _gains[3];
    float _dt;   
    float _prev_error=0; 
    float _integral=0;
    float _max_integral;
    bool _circular;

};



#endif // PID_H

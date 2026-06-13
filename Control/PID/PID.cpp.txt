//PID VERSION ANTI-WINDUP y RESET INTEGRAL,Circular, dynamic gains
#include "PID.h"

PID::PID(){
}

void PID::setup(float gains[3], float dt,float windup, bool circular){

    for (int i = 0; i < 3; i++)
        _gains[i] = gains[i];
    _dt = dt;
    _max_integral = windup;
    _circular = circular;
}

float PID::calculate(float ref, float measurement){

    float error = ref-measurement;
    if (_circular) {
        while (error > 180.0f)error -= 360.0f;
        while (error < -180.0f)error += 360.0f;
    }
    float u = _gains[0]*error;
    u+=_gains[2]*(error-_prev_error)/_dt;
    _integral += _dt*(error + _prev_error)/2.0f;
    if (_integral > _max_integral) _integral = _max_integral;
    else if (_integral < -_max_integral) _integral = -_max_integral;
    u+=_gains[1]*_integral;
    _prev_error=error;
    
    return u;

}

void PID::resetIntegral(){
    _integral = 0;
}

void PID::update_dt(float dt){
    _dt = dt;
}

void PID::update_gains(float new_gains[3]){
    for (int i = 0; i < 3; i++)
        _gains[i] = new_gains[i];
}
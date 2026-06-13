#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(){}

void KalmanFilter::setup(float _Q_angle, float _Q_bias, float _R_measure, bool _circular){
    Q_angle = _Q_angle;
    Q_bias = _Q_bias;
    R_measure = _R_measure;
    circular = _circular;
}

float KalmanFilter::calculate(float q_prev, float model_input, float q_meas, float dt)
{   
    model_input = model_input - bias;
    q_prev += dt*model_input;
    P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;

    // Update
    float S = P[0][0] + R_measure; // Estimate error
    float K[2];                    // Kalman gain
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;
 
    float y = q_meas - q_prev; // Angle difference
    if (circular) {
        while (y > 180.0f) y -= 360.0f;
        while (y < -180.0f) y += 360.0f;
    }
    q_prev += K[0] * y;
    bias += K[1] * y;
    
    double P00_temp = P[0][0];
    double P01_temp = P[0][1];
 
    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;

    if (circular) {
        while (q_prev > 180.0f) q_prev -= 360.0f;
        while (q_prev < -180.0f) q_prev += 360.0f;
    }
 
    return q_prev;
}

float KalmanFilter::tilt_compensation(float B_xyz[3],float Rot_xy[2]){
    float mx = B_xyz[0];
    float my = B_xyz[1];
    float mz = B_xyz[2];

    float roll = degToRad(Rot_xy[0]);
    float pitch = degToRad(Rot_xy[1]);

    float Bx = mx*cos(pitch) + mz*sin(pitch);
    float By = mx*sin(roll)*sin(pitch) + my*cos(roll) - mz*sin(roll)*cos(pitch);

    float heading_rad = atan2(By,Bx); 
    float heading_deg = radToDeg(heading_rad);
    heading_deg += declination;

    while (heading_deg > 180.0f) {
        heading_deg -= 360.0f;
    }
    while (heading_deg < -180.0f) {
        heading_deg += 360.0f;
    }
    return heading_deg;
}
float KalmanFilter::degToRad(float deg){
    return deg * M_PI / 180.0f;
}
float KalmanFilter::radToDeg(float rad){
    return rad * 180.0f / M_PI;
}
void KalmanFilter::set_declination(float deg_declination){
    declination = deg_declination;
}

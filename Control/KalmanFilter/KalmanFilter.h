#ifndef __KALMANFILTER_H__
#define __KALMANFILTER_H__

#include <cmath>

class KalmanFilter{

    private: //atributos
    float bias = 0;
    bool circular;
    float Q_angle,Q_bias,R_measure;
    float P[2][2] = {{0, 0}, // Error covariance matrix
                     {0, 0}
    };
    float declination = 3.97; //declinacion magnetica ciudad de mexico 2025 (en grados)
    public: //metodos

    KalmanFilter(); //contructor
    void setup(float _Q_angle, float _Q_bias, float _R_measure, bool _circular = false);
    float calculate(float q_prev, float model_input, float q_meas, float dt);
    float tilt_compensation(float B_xyz[3],float Rot_xy[2]); //x,y,z en gauss o lsb / roll,pitch en grados
    float degToRad(float deg);
    float radToDeg(float rad);
    void set_declination(float deg_declination); //en grados
};

#endif // __KALMANFILTER_H__

/* 

Descripcion de parametros constantes y dinamicos:

q_prev: Distancia o angulo previo (x,y,z,0x,0y,0z).
model_input: variable del sensor que se utiliza para el modelo matematico.
q_meas: variable variable del sensor que se compara con el del modelo matematico.
dt: unidad de tiempo en segundos que indica el periodo de tiempo para los calculos. 

Q_angle:
Q_bias:
R_measure: varianza de sensores no usados para modelo matematico

*/

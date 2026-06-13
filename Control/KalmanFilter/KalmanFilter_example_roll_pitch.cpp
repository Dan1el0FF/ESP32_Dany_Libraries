#include "BMI160.h"
#include "HMC5883L.h"
#include "KalmanFilter.h"
#define Q_angle 0.025
#define Q_bias 0.003
#define R_measure_1 0.0042  //PITCH
#define R_measure_2  0.0035 //ROLL
#define R_measure_3 0.0245  //YAW
extern "C" void app_main(){
    
    BMI160 bmi;
    KalmanFilter kalman1;
    KalmanFilter kalman2;

    bmi.init_BMI160();

    kalman1.setup(Q_angle,Q_bias,R_measure_1);
    kalman2.setup(Q_angle,Q_bias,R_measure_2);

    float Acc[3];
    float Gyro[3];
    float pitch = 0;
    float roll = 0;
    float dt = 0.01;
    while(true){
        bmi.readAcc(Acc);
        bmi.readGyro(Gyro);
        float acc_pitch = atan2(-Acc[0], sqrt(Acc[1] * Acc[1] + Acc[2] * Acc[2])) * 180.0 / M_PI;
        float acc_roll = atan2(Acc[1],Acc[2]) * 180.0 / M_PI;

        pitch = kalman1.calculate(pitch,Gyro[0],acc_pitch,dt);
        roll = kalman2.calculate(roll,Gyro[1],acc_roll,dt);
        
        printf(">kp:%.2f,Ap:%.2f,kr:%.2f,Ar:%.2f\r\n",pitch,acc_pitch,roll,acc_roll);
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

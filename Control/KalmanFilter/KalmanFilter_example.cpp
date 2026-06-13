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
    //HMC5883L hmc;
    KalmanFilter kalman1;
    KalmanFilter kalman2;
    KalmanFilter kalman3;
    bmi.init_BMI160();
    //hmc.init_HMC5883L(true);
    kalman1.setup(Q_angle,Q_bias,R_measure_1);
    kalman2.setup(Q_angle,Q_bias,R_measure_2);
    kalman3.setup(Q_angle,Q_bias,R_measure_3);

    float Acc[3];
    float hmc_data[5];
    float Gyro[3];
    float pitch_prev = 0;
    float dt = 0.01;
    while(true){
        bmi.readAcc(Acc);
        bmi.readGyro(Gyro);
        //hmc.read_hmc5883l(hmc_data);
        pitch_prev = kalman1.calculate(pitch_prev,Gyro[0],atan2(-Acc[0], sqrt(Acc[1] * Acc[1] + Acc[2] * Acc[2])) * 180.0 / M_PI,dt);
        float acc_pitch = atan2(-Acc[0], sqrt(Acc[1] * Acc[1] + Acc[2] * Acc[2])) * 180.0 / M_PI;
        printf(">kalman_pitch:%.2f,Acc_pitch:%.2f\r\n",pitch_prev,acc_pitch);
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

#include "BMI160.h"
#include "HMC5883L.h"
#include "KalmanFilter.h"
#define Q_angle 0.025
#define Q_angle_yaw 0.07
#define Q_bias 0.003
#define R_measure_1 0.0042  //PITCH
#define R_measure_2  0.0035 //ROLL
#define R_measure_3 0.0245  //YAW

extern "C" void app_main(){

    static MultipleI2C bus_bmi;
    static MultipleI2C bus_hmc;
    static BMI160 bmi(&bus_bmi);
    static HMC5883L hmc(&bus_hmc);

    KalmanFilter kalman_pitch;
    KalmanFilter kalman_roll;
    KalmanFilter kalman_yaw_1;
    KalmanFilter kalman_yaw_Wtilt;

    bmi.init_BMI160();
    hmc.init_HMC5883L(false);
    hmc.set_offset(0.065,-0.275,-0.272);
    kalman_pitch.setup(Q_angle,Q_bias,R_measure_1);
    kalman_roll.setup(Q_angle,Q_bias,R_measure_2);
    kalman_yaw_1.setup(Q_angle_yaw,Q_bias,R_measure_3);
    kalman_yaw_Wtilt.setup(Q_angle_yaw,Q_bias,R_measure_3);

    float hmc_data[5];
    float B_xyz[3];
    float Rot_xy[2];
    float Acc[3];
    float gyro[3];
    float yaw = 0;
    float Tyaw = 0;
    float pitch = 0;
    float roll = 0;
    float dt = 0.01;

    while(true){
        bmi.readAcc(Acc);
        bmi.readGyro(gyro);
        hmc.read_hmc5883l(hmc_data);
        B_xyz[0] = hmc_data[0];
        B_xyz[1] = hmc_data[1];
        B_xyz[2] = hmc_data[2];


        float mea_pitch = atan2(-Acc[0], sqrt(Acc[1] * Acc[1] + Acc[2] * Acc[2])) * 180.0 / M_PI;
        float mea_roll = atan2(Acc[1],Acc[2]) * 180.0 / M_PI;
        float mea_yaw = hmc_data[4];

        pitch = kalman_pitch.calculate(pitch,gyro[0],mea_pitch,dt);
        roll = kalman_roll.calculate(roll,gyro[1],mea_roll,dt);
        yaw = kalman_yaw_1.calculate(yaw,gyro[2],mea_yaw,dt);

        Rot_xy[0] = roll;
        Rot_xy[1] = pitch;

        float mea_Tyaw = kalman_yaw_Wtilt.tilt_compensation(B_xyz, Rot_xy);
        Tyaw = kalman_yaw_Wtilt.calculate(Tyaw,gyro[2],mea_Tyaw,dt);
        

        printf(">Kp:%.2f,p:%.2f,kr:%.2f,r:%.2f,ky:%.2f,y:%.2f\r\n",pitch,mea_pitch,roll,mea_roll,Tyaw,mea_Tyaw); 
        
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

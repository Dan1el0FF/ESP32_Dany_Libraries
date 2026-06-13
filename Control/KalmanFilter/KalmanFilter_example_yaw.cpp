#include "BMI160.h"
#include "HMC5883L.h"
#include "KalmanFilter.h"
#define Q_angle 0.025
#define Q_bias 0.003
#define R_measure_1 0.0042  //PITCH
#define R_measure_2  0.0035 //ROLL
#define R_measure_3 0.0245  //YAW
extern "C" void app_main(){
    
    HMC5883L hmc;
    hmc.init_HMC5883L(true);
    float hmc_data[5];
    float yaw = 0;
    while(true){
        hmc.read_hmc5883l(hmc_data);
        yaw = hmc_data[4];
        printf(">yaw:%.2f\r\n",yaw);
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

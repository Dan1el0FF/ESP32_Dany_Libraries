#include "BMI160.h"
#include "HMC5883L.h"
extern "C" void app_main(){

    static MultipleI2C bus_bmi;
    static MultipleI2C bus_hmc;

    static BMI160 bmi(&bus_bmi);
    static HMC5883L hmc(&bus_hmc);
    
    bmi.init_BMI160();
    hmc.init_HMC5883L(true);
    

    float hmc_data[5];
    float Acc[3];
    float gyro[3];
    float yaw = 0;
    float pitch = 0;
    float roll = 0;

    while(true){
        bmi.readAcc(Acc);
        bmi.readGyro(gyro);

        hmc.read_hmc5883l(hmc_data);


        pitch = atan2(-Acc[0], sqrt(Acc[1] * Acc[1] + Acc[2] * Acc[2])) * 180.0 / M_PI;
        roll = atan2(Acc[1],Acc[2]) * 180.0 / M_PI;
        yaw = hmc_data[4];
        
        

        printf(">pitch:%.2f,roll:%.2f,yaw:%.2f\r\n",pitch,roll,yaw);
        //printf("Gyro X:%.2f, Y:%.2f, Z:%.2f\r\n",gyro[0],gyro[1],gyro[2]);
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

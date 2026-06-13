#include "BMI160.h"
#include "MultipleI2C.h"
#include <stdio.h>
#include <Freertos/task.h>
#include <Freertos/FreeRTOS.h>
static MultipleI2C bus_bmi;
static BMI160 bmi(&bus_bmi);
float Acc[3], gyro[3];

extern "C" void app_main() {
    bmi.init_BMI160(3,3,0x68,false);
    bmi.set_or_reset_offsets(0.595402,0.575763,-0.003832,-0.319084,-0.125191,-0.013740);
    while(1){
        bmi.readGyro(gyro);
        bmi.readAcc(Acc);
        printf("gyro:%f\n",Acc[0]);
    }
}
*/

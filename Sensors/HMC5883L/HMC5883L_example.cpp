#include "HMC5883L.h"
#include "MultipleI2C.h"
#include <stdio.h>
#include <Freertos/task.h>
#include <Freertos/FreeRTOS.h>
static MultipleI2C bus_hmc;
HMC5883L mag(&bus_hmc);
float hmc_data[5];

extern "C" void app_main() {
    mag.init_HMC5883L(false);
    mag.set_offset(0.046, -0.115, -0.189); //offset probado  0.065,-0.275,-0.272

    while (true)
    {
        mag.read_hmc5883l(hmc_data);
        printf("hmc:%f\n",hmc_data[4]);
    }
    
}

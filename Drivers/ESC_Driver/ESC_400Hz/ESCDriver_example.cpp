#include "ESCDriver.h"
#include <Freertos/FreeRTOS.h>
extern "C" void app_main(){

    ESCDriver esc;

    esc.setup(0,GPIO_NUM_27);
    esc.setDuty(0);
    vTaskDelay(5000/portTICK_PERIOD_MS);
    esc.setDuty(30);
    vTaskDelay(3000/portTICK_PERIOD_MS);
    esc.setDuty(100);
    vTaskDelay(3000/portTICK_PERIOD_MS);
    esc.setDuty(0);


}

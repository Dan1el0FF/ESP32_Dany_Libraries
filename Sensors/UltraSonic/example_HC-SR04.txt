#include <driver/gpio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp32c2/rom/ets_sys.h>
#include "UltraSonic.h"

using namespace std;



extern "C" void app_main(void){
    UltraSonic ultra1(17,16);
    float distance;
    while(true){
    distance=ultra1.getDistance(); 
    cout<<"La distancia es: "<<distance<<"\n";
    vTaskDelay(500/portTICK_PERIOD_MS); //velocidad a la que se actualiza la distancia en ms
    
    }
}


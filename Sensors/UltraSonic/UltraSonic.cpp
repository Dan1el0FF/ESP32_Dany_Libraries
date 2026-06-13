//nueva ultrasonic anti-watchdog
#include "UltraSonic.h"

UltraSonic::UltraSonic(int Trig_,int Echo_){
    Trig=Trig_;
    Echo=Echo_;
    configureUltrasonic();
}

void UltraSonic::configureUltrasonic(){
    gpio_set_direction(gpio_num_t(Trig), GPIO_MODE_OUTPUT);
    gpio_set_direction(gpio_num_t(Echo), GPIO_MODE_INPUT);
    gpio_set_level(gpio_num_t(Trig),0);
}

float UltraSonic::getDistance(){

        gpio_set_level(gpio_num_t(Trig),1);
        ets_delay_us(10); // Espera 10 microsegundos
        gpio_set_level(gpio_num_t(Trig),0);

        int timeout=10000;

        // Esperar a que Echo pase a HIGH
        while (gpio_get_level(gpio_num_t(Echo))==0 && timeout>0) {
            // Esperar a que el eco suba
            timeout--;
        }
        if(timeout==0){
            return -1;
        }
        
        // Medir el tiempo en microsegundos mientras el Echo está en HIGH
        int64_t startTime = esp_timer_get_time();
        while (gpio_get_level(gpio_num_t(Echo))==1) {
            // Esperar a que el eco baje
        }
        int64_t endTime = esp_timer_get_time();
        
        // Calcular el tiempo de ida y vuelta en microsegundos
        int64_t duration = endTime - startTime;

        // Convertir el tiempo a distancia (en cm)
        float distance = (duration / 2.0) * 0.0343; // Velocidad del sonido es ~343 m/s

        return distance;

}

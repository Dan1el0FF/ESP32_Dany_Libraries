#include "stdio.h"
#include "SimpleI2C.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "cmath"
#include "HMC5883L.h"
const int N = 200;
float pitch_samples[N];
float sum = 0.0;
float R_measure;
float roll = 66666;
float pitch = 6666;
float yaw = 6666;
extern "C" void app_main(void){
    HMC5883L sensor;
    float gauss[5];
    sensor.init_HMC5883L(false);
    sensor.set_offset(-0.182,-0.273,-0.082);//0.101707
    printf("comenzando muestras acomode el sensor en posicione estatica\n");
    vTaskDelay(4000/portTICK_PERIOD_MS); // espera entre lecturas
    printf("comenzando muestras no mueva el sensor\n");
    // 1. Recolectar muestras
    for (int i = 0; i < N; i++) {
        sensor.read_hmc5883l(gauss);
        yaw = gauss[4];
        pitch_samples[i] = yaw;
        sum += yaw;
        vTaskDelay(30/portTICK_PERIOD_MS); // espera entre lecturas
    }

    // 2. Calcular promedio
    float mean = sum / N;

    // 3. Calcular varianza
    float variance = 0.0;
    for (int i = 0; i < N; i++) {
        variance += pow(pitch_samples[i] - mean, 2);
    }
    variance /= N;

    R_measure = variance;
    printf("R_measure = %f\n",variance);
    printf("yaw: %f\n",yaw);
}

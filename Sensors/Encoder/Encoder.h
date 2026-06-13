#ifndef ENCODER_H
#define ENCODER_H

#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Encoder {
private: // Atributos
    int isr_pin_A; // Pin del encoder A
    int isr_pin_B; // Pin del encoder B
    int ppr; // Pulsos por revolución del motor
    int interval_MS; // Intervalo en milisegundos para medir la velocidad
    volatile int totalPulsesA = 0;
    volatile unsigned long long totalPulsesB = 0;
    int total;

    static void IRAM_ATTR isr_A(void *arg); // ISR estática para el pin A
    static void IRAM_ATTR isr_B(void *arg); // ISR estática para el pin B

public: // Métodos
    Encoder(int isr_pin_A,int isr_pin_B, int ppr, int interval_MS=50); // Constructor
    void configureEncoder();
    float getRpm();
    int getPulsesA();
    int getPulsesB();
    int getDirection();
    void resetPulsesB();
};


#endif // ENCODER_H

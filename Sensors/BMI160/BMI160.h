#ifndef __BMI160_H__
#define __BMI160_H__

#include "MultipleI2C.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"

class BMI160 {

private:  //atributos

MultipleI2C* I2C;
//Tablas de sensibilidades
//                              ±2g   ±4g   ±8g   ±16g
//                               1     2     3     4      5     
float ACCEL_SENSITIVITY[4] = { 16384, 8192, 4096, 2048         };
//                             ±2000  ±1000 ±500  ±250   ±125
float GYRO_SENSITIVITY[5] =  { 16.4,  32.8, 65.5, 131.0, 262.0 };

//Tablas de rangos
    uint8_t acc_range[4][2] = {{0x41, 0x03}, // ±2g  sens: 16384 (LSB/g)      selection 1
                               {0x41, 0x05}, // ±4g  sens: 8192  (LSB/g)      selection 2
                               {0x41, 0x08}, // ±8g  sens: 4096  (LSB/g)      selection 3
                               {0x41, 0x0C}, // ±16g sens: 2048  (LSB/g)      selection 4
    };         
    uint8_t gyro_range[5][2] = {{0x43, 0x00}, // ±2000  sens: 16.4 (LSB/°/s)  selection 1
                                {0x43, 0x01}, // ±1000  sens: 32.8 (LSB/°/s)  selection 2
                                {0x43, 0x02}, // ±500  sens: 65.5  (LSB/°/s)  selection 3
                                {0x43, 0x03}, // ±250 sens: 131.0  (LSB/°/s)  selection 4
                                {0x43, 0x04}, // ±125 sens: 262.0  (LSB/°/s)  selection 5
    };

bool EN_AUTO_OFFSET;

float ACC_OFFSET_X = 0;
float ACC_OFFSET_Y = 0;
float ACC_OFFSET_Z = 0;

float GYRO_OFFSET_X = 0;
float GYRO_OFFSET_Y = 0;
float GYRO_OFFSET_Z = 0;

uint8_t SELECT_ACC;
uint8_t SELECT_GYRO;

public: //metodos

BMI160(MultipleI2C* i2c_ptr); //Constructor
void init_BMI160(uint8_t _acc_range = 3, uint8_t _gyro_range = 3, uint8_t _BMI160_I2C_ADDRESS = 0x68,bool _EN_AUTO_OFFSET = true); // accelerometer range, gyro range, BMI160 ADRESS, true/false
void readAcc(float Acc[3]); //lee valores crudos de aceleracion del sensor y los convierte a valores legibles en m/s
void readGyro(float Gyro[3]);//lee los valores crudos de velocidad angular del sensor y los convierte en valores legibles en °/s
void calculate_offsets();
void set_or_reset_offsets(float ax_offset = 0,float ay_offset = 0,float az_offset = 0,
                          float gx_offset = 0,float gy_offset = 0,float gz_offset = 0); // set or reset acc/gyro offsets
void new_acc_range(uint8_t _new_acc_range); // selecciona numero entre el 1 y el 4 para seleccionar un rango (se pueden checar los rangos en las tablas de los atributos)
void new_gyro_range(uint8_t _new_gyro_range); // selecciona numero entre el 1 y el 5 para seleccionar un rango (se pueden checar los rangos en las tablas de los atributos)
float read_temperature(); //lee la temperatura del chip
};

#endif // __BMI160_H__
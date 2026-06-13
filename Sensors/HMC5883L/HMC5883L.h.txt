#ifndef __HMC5883L_H__
#define __HMC5883L_H__

#include "MultipleI2C.h"
#include "cmath"
#include "esp_log.h"
#include <esp_timer.h>
#include <esp_task_wdt.h>

class HMC5883L {

    private:  //atributos

    MultipleI2C* I2C_HMC5883L;
    float x_offset = 0;
    float y_offset = 0;
    float z_offset = 0;
    float declination = 3.97; //declinacion magnetica ciudad de mexico 2025 (en grados)
    public:  //metodos

    HMC5883L(MultipleI2C* i2c_ptr); //constructor
    void init_HMC5883L(bool EN_CALIBRATION = false);
    void read_hmc5883l(float hmc_data[5]); //x,y,z,field,heading
    void calibrate();
    void set_offset(float _x_offset,float _y_offset, float _z_offset);
    void get_offset(float offsets[3]); //x,y,z
    void set_declination(float deg_declination); //en grados
};
#endif // __HMC5883L_H__


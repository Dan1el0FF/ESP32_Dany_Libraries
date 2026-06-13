//version regresa valores de -180 a 180
#include "HMC5883L.h"

static const char* TAG = "HMC5883L";

HMC5883L::HMC5883L(MultipleI2C* i2c_ptr) {
    I2C_HMC5883L = i2c_ptr;
}

void HMC5883L::init_HMC5883L(bool EN_CALIBRATION){
    uint8_t sample_rate[2] = {0x00, 0x18};   // 8 muestras, 75Hz, modo normal
    uint8_t mag_range[2]   = {0x01, 0x20};   // ±1.3 Gauss (1090 LSB/G)
    uint8_t mag_mode[2]    = {0x02, 0x00};   // Modo continuo

    I2C_HMC5883L->setup(0X1E);
    if(!I2C_HMC5883L->write(sample_rate, sizeof(sample_rate))) ESP_LOGE(TAG,"[BugFinder] Fallo al escribir (sample rate)");
    vTaskDelay(100 / portTICK_PERIOD_MS);
    if(!I2C_HMC5883L->write(mag_range, sizeof(mag_range))) ESP_LOGE(TAG, "[BugFinder] Fallo al escribir (mag range)");
    vTaskDelay(100 / portTICK_PERIOD_MS);
    if(!I2C_HMC5883L->write(mag_mode, sizeof(mag_mode))) ESP_LOGE(TAG, "[BugFinder] Fallo al escribir (mag mode)");
    vTaskDelay(100 / portTICK_PERIOD_MS);
    if(EN_CALIBRATION) calibrate();
}
void HMC5883L::read_hmc5883l(float hmc_data[5]){
    uint8_t mag_reg = 0x03;
    uint8_t mag_data[6] = {0};
    if(!I2C_HMC5883L->read(&mag_reg, 1, mag_data, 6)){
        ESP_LOGE(TAG, "[BugFinder] Fallo al leer (read hmc5883l)");
        return;
    }
    int16_t x_raw = (int16_t)((mag_data[0] << 8) | mag_data[1]);
    int16_t z_raw = (int16_t)((mag_data[2] << 8) | mag_data[3]);
    int16_t y_raw = (int16_t)((mag_data[4] << 8) | mag_data[5]);
    float x_gauss = (x_raw / 1090.0f) - x_offset;
    float y_gauss = (y_raw / 1090.0f) - y_offset;
    float z_gauss = (z_raw / 1090.0f) - z_offset;
    float field = sqrt(x_gauss * x_gauss + y_gauss * y_gauss + z_gauss * z_gauss);
    float heading_rad = atan2(y_gauss, x_gauss);
    float heading_deg = heading_rad * 180.0f / M_PI;
    heading_deg += declination;
    while (heading_deg > 180.0f) heading_deg -= 360.0f;
    while (heading_deg < -180.0f) heading_deg += 360.0f;
    hmc_data[0] = x_gauss;
    hmc_data[1] = y_gauss;
    hmc_data[2] = z_gauss;
    hmc_data[3] = field;
    hmc_data[4] = heading_deg;
}
void HMC5883L::calibrate(){
    uint64_t dt_us = 20000000;  // 20 segundos
    int64_t prev_time = esp_timer_get_time();
    int64_t current_time;

    float x_min = 1000, x_max = -1000;
    float y_min = 1000, y_max = -1000;
    float z_min = 1000, z_max = -1000;
    ESP_LOGI(TAG,"Iniciando calibración, mueva el sensor en los 3 ejes...");

    while ((current_time = esp_timer_get_time()) - prev_time < dt_us) {
        uint8_t mag_reg = 0x03;
        uint8_t mag_data[6] = {0};
        if(!I2C_HMC5883L->read(&mag_reg, 1, mag_data, 6)) ESP_LOGE(TAG, "[BugFinder] Fallo al leer datos para calibracion (volver a calibrar)");
        int16_t x_raw = (int16_t)((mag_data[0] << 8) | mag_data[1]);
        int16_t z_raw = (int16_t)((mag_data[2] << 8) | mag_data[3]);
        int16_t y_raw = (int16_t)((mag_data[4] << 8) | mag_data[5]);
        float x_gauss = x_raw / 1090.0f;
        float y_gauss = y_raw / 1090.0f;
        float z_gauss = z_raw / 1090.0f;
        if (x_gauss < x_min) x_min = x_gauss;
        if (x_gauss > x_max) x_max = x_gauss;
        if (y_gauss < y_min) y_min = y_gauss;
        if (y_gauss > y_max) y_max = y_gauss;
        if (z_gauss < z_min) z_min = z_gauss;
        if (z_gauss > z_max) z_max = z_gauss;
        x_offset = (x_max + x_min) / 2.0f;
        y_offset = (y_max + y_min) / 2.0f;
        z_offset = (z_max + z_min) / 2.0f;
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    prev_time = current_time;    
    ESP_LOGI(TAG,"Calibración completada...");
    ESP_LOGI(TAG,"Los offsets son: X: %.3f Y: %.3f Z: %.3f",x_offset,y_offset,z_offset);
}

void HMC5883L::set_offset(float _x_offset,float _y_offset, float _z_offset){
    x_offset = _x_offset;
    y_offset = _y_offset;
    z_offset = _z_offset;
}
void HMC5883L::get_offset(float offsets[3]){
    offsets[0] = x_offset;
    offsets[1] = y_offset;
    offsets[2] = z_offset;
    ESP_LOGI(TAG,"Los offsets son: X: %.3f Y: %.3f Z: %.3f",x_offset,y_offset,z_offset);
}

void HMC5883L::set_declination(float deg_declination){
    declination = deg_declination;
}



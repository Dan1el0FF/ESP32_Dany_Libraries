#include "BMI160.h"

static const char* TAG = "BMI160";

BMI160::BMI160(MultipleI2C* i2c_ptr) {
    I2C = i2c_ptr;
}

void BMI160::init_BMI160(uint8_t _acc_range, uint8_t _gyro_range, uint8_t BMI160_I2C_ADDRESS, bool _EN_AUTO_OFFSET){
    EN_AUTO_OFFSET = _EN_AUTO_OFFSET;
    SELECT_ACC = (_acc_range >= 1 && _acc_range <= 4) ? _acc_range - 1 : 2; // default a ±8g
    SELECT_GYRO = (_gyro_range >= 1 && _gyro_range <= 5) ? _gyro_range - 1 : 2; // default a ±500 °/s
    I2C->setup(BMI160_I2C_ADDRESS);
    uint8_t reg_chip = 0x00;
    uint8_t chip_id = 0;
    if (!I2C->read(&reg_chip, 1, &chip_id, 1))ESP_LOGE(TAG, "[BugFinder] No se pudo leer el CHIP_ID");
    if (chip_id == 0xD1)ESP_LOGI(TAG, "[BugFinder] BMI160 detectado correctamente. CHIP_ID: 0x%02X", chip_id);
    else ESP_LOGW(TAG, "[BugFinder] CHIP_ID incorrecto: 0x%02X (se esperaba 0xD1)", chip_id);

    uint8_t acc_suspend[]  = {0x7E, 0x10};
    uint8_t gyro_suspend[] = {0x7E, 0x14};
    if(!I2C->write(acc_suspend,2))ESP_LOGE(TAG, "[BugFinder] Fallo al suspender acc");
    vTaskDelay(50 / portTICK_PERIOD_MS);
    if(!I2C->write(gyro_suspend,2))ESP_LOGE(TAG, "[BugFinder] Fallo al suspender gyro");
    vTaskDelay(50 / portTICK_PERIOD_MS);
    uint8_t acc_conf[] = {0x40, 0x0A};// ACC: 400 Hz, BW normal - 0x0A filtro fuerte
    uint8_t gyro_conf[] = {0x42, 0x1C}; // GYRO: 1600 Hz - 0x1C filtro medio
    if (!I2C->write(acc_conf,2))ESP_LOGE(TAG, "[BugFinder] Fallo al configurar frecuencia del acelerometro");
    vTaskDelay(50 / portTICK_PERIOD_MS);
    if (!I2C->write(gyro_conf,2))ESP_LOGE(TAG, "[BugFinder] Fallo al configurar frecuencia del gyroscopio");
    vTaskDelay(50 / portTICK_PERIOD_MS);
    // Configuraciones de inicialización
    uint8_t acc_normal_mode[] = {0x7E, 0x11};     // Activar ACC
    uint8_t gyro_normal_mode[] = {0x7E, 0x15};    // Activar GYR
    uint8_t calibrate_acc[] = {0x7E,0x37};        // Calibrar ACC
    if (!I2C->write(acc_normal_mode, sizeof(acc_normal_mode)))ESP_LOGE(TAG, "[BugFinder] Fallo al configurar acc_normal_mode");
    vTaskDelay(50/portTICK_PERIOD_MS);
    if (!I2C->write(acc_range[SELECT_ACC],2))ESP_LOGE(TAG, "[BugFinder] Fallo al configurar el rango de aceleraciones"); 
    vTaskDelay(50/portTICK_PERIOD_MS);
    if (!I2C->write(calibrate_acc,sizeof(calibrate_acc)))ESP_LOGE(TAG, "[BugFinder] Fallo al calibrar acelerometro");
    vTaskDelay(50/portTICK_PERIOD_MS);
    if (!I2C->write(gyro_normal_mode, sizeof(gyro_normal_mode)))ESP_LOGE(TAG, "[BugFinder] Fallo al configurar gyro_normal_mode");
    vTaskDelay(100/portTICK_PERIOD_MS);
    if (!I2C->write(gyro_range[SELECT_GYRO],2))ESP_LOGE(TAG, "[BugFinder] Fallo al configurar el rango de velocidades angulares");
    vTaskDelay(100/portTICK_PERIOD_MS);
    calculate_offsets();
}



void BMI160::readAcc(float Acc[3]){
    uint8_t reg_acc = 0x12;
    uint8_t acc_data[6] = {0};
    if (!I2C->read(&reg_acc, 1, acc_data, 6)){
        ESP_LOGE(TAG, "[BugFinder] Fallo al leer los datos del acelerometro");
        return;
    }
    int16_t ax = (int16_t)((acc_data[1] << 8) | acc_data[0]);
    int16_t ay = (int16_t)((acc_data[3] << 8) | acc_data[2]);
    int16_t az = (int16_t)((acc_data[5] << 8) | acc_data[4]);
    Acc[0] = ax * (9.81 / ACCEL_SENSITIVITY[SELECT_ACC]) + ACC_OFFSET_X;
    Acc[1] = ay * (9.81 / ACCEL_SENSITIVITY[SELECT_ACC]) + ACC_OFFSET_Y;
    Acc[2] = az * (9.81 / ACCEL_SENSITIVITY[SELECT_ACC]) + ACC_OFFSET_Z;
}

void BMI160::readGyro(float Gyro[3]){
    uint8_t reg_gyro = 0x0C;
    uint8_t gyr_data[6] = {0};
    if (!I2C->read(&reg_gyro, 1, gyr_data, 6)){
        ESP_LOGE(TAG, "[BugFinder] Fallo al leer los datos del gyroscopio");
        return;
    }
    int16_t gx = (int16_t)((gyr_data[1] << 8) | gyr_data[0]);
    int16_t gy = (int16_t)((gyr_data[3] << 8) | gyr_data[2]);
    int16_t gz = (int16_t)((gyr_data[5] << 8) | gyr_data[4]);
    Gyro[0] = gx / GYRO_SENSITIVITY[SELECT_GYRO] + GYRO_OFFSET_X;
    Gyro[1] = gy / GYRO_SENSITIVITY[SELECT_GYRO] + GYRO_OFFSET_Y;
    Gyro[2] = gz / GYRO_SENSITIVITY[SELECT_GYRO] + GYRO_OFFSET_Z;
}

void BMI160::calculate_offsets(){
    float acc_offsets[3]; //ax,ay,az
    float gyro_offsets[3]; //gx,gy,gz
    float Acc_sample_list[5][3];
    float Gyro_sample_list[5][3];
    float acc_current_sample[3];
    float gyro_current_sample[3];
    if(EN_AUTO_OFFSET){
        ESP_LOGI(TAG,"Iniciando calibración, no mueva el sensor");
        for(int i=0;i<5;i++){
            readAcc(acc_current_sample);
            readGyro(gyro_current_sample);
            for(int j=0;j<3;j++){ 
                Acc_sample_list[i][j]=acc_current_sample[j];
                Gyro_sample_list[i][j]=gyro_current_sample[j];
            }
            vTaskDelay(50 / portTICK_PERIOD_MS);
        }
        for(int i=0;i<3;i++){
            float Asum = 0;
            float Bsum = 0;
            for(int j=0;j<5;j++){
                Asum+=Acc_sample_list[j][i];
                Bsum+=Gyro_sample_list[j][i];
            }
            acc_offsets[i] = Asum / 5.0f;
            gyro_offsets[i] = Bsum / 5.0f;
        }
        ACC_OFFSET_X = -acc_offsets[0];
        ACC_OFFSET_Y = -acc_offsets[1];
        ACC_OFFSET_Z = 9.81 - acc_offsets[2];

        GYRO_OFFSET_X = -gyro_offsets[0];
        GYRO_OFFSET_Y = -gyro_offsets[1];
        GYRO_OFFSET_Z = -gyro_offsets[2];
        ESP_LOGI(TAG,"Calibracion terminada Ax: %f Ay: %f Az:%f Gx: %f Gy: %f Gz: %f",ACC_OFFSET_X,ACC_OFFSET_Y,ACC_OFFSET_Z,GYRO_OFFSET_X,GYRO_OFFSET_Y,GYRO_OFFSET_Z);
    } 
}

void BMI160::set_or_reset_offsets(float ax_offset,float ay_offset,float az_offset,float gx_offset,float gy_offset,float gz_offset){
    ACC_OFFSET_X = ax_offset;
    ACC_OFFSET_Y = ay_offset;
    ACC_OFFSET_Z = az_offset;  
    GYRO_OFFSET_X = gx_offset;
    GYRO_OFFSET_Y = gy_offset;
    GYRO_OFFSET_Z = gz_offset;   
}

void BMI160::new_acc_range(uint8_t _new_acc_range){
    SELECT_ACC = (_new_acc_range >= 1 && _new_acc_range <= 4) ? _new_acc_range - 1 : 2; // default a ±8g
    if (!I2C->write(acc_range[SELECT_ACC],2))ESP_LOGE(TAG, "[BugFinder] Fallo al configurar nuevo rango de aceleraciones");
    vTaskDelay(100/portTICK_PERIOD_MS);
}

void BMI160::new_gyro_range(uint8_t _new_gyro_range){
    SELECT_GYRO = (_new_gyro_range >= 1 && _new_gyro_range <= 5) ? _new_gyro_range - 1 : 2; // default a ±500 °/s
    if (!I2C->write(gyro_range[SELECT_GYRO],2))ESP_LOGE(TAG, "[BugFinder] Fallo al configurar nuevo rango de velocidades angulares");
    vTaskDelay(100/portTICK_PERIOD_MS);
}
float BMI160::read_temperature(){
    uint8_t reg_temp = 0x20;
    uint8_t temp_data[2] = {0};
    if (!I2C->read(&reg_temp, 1, temp_data, 2))ESP_LOGE(TAG, "[BugFinder] Fallo al leer los datos de temperatura");
    int16_t temp_raw = (int16_t)((temp_data[1] << 8) | temp_data[0]);
    float temperature = (temp_raw / 512.0f) + 23.0f;
    if(temperature > 85.0f && temperature < 100.0f) ESP_LOGW(TAG, "[BugFinder] Advertencia: la temperatura actual (%.2f °C) puede afectar la presicion de las mediciones",temperature);
    if(temperature >= 100.0f && temperature <= 125.0f)ESP_LOGE(TAG, "[BugFinder] Peligro: la temperatura actual (%.2f °C) puede averiar el sensor permanentemente",temperature);
    if (temperature > 125.0f)ESP_LOGE(TAG, "[BugFinder] Temperatura crítica: %.2f °C. Se ha excedido el límite máximo absoluto.",temperature);
    return temperature;
}




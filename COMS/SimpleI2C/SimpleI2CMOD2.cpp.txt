#include "SimpleI2C.h"

SimpleI2C::SimpleI2C()
{
}

void SimpleI2C::setup(uint8_t device_address, uint32_t freq, uint8_t sda_pin, uint8_t scl_pin, i2c_port_t i2c_num)
{
    _device_address = device_address;
    _i2c_port = i2c_num;

    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)sda_pin;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)scl_pin;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = freq;
    conf.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(_i2c_port, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(_i2c_port, I2C_MODE_MASTER, 0, 0, 0));

    ESP_LOGI("I2C", "Driver clásico instalado en I2C_NUM_%d con dirección 0x%02X", _i2c_port, _device_address);
}

bool SimpleI2C::read(uint8_t data_in[], size_t size_in, uint8_t data_out[], size_t size_out)
{
    esp_err_t ret = i2c_master_write_read_device(
        _i2c_port,
        _device_address,
        data_in, size_in,
        data_out, size_out,
        pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS)
    );

    if (ret != ESP_OK) {
        ESP_LOGE("I2C", "Read error: %s", esp_err_to_name(ret));
        return false;
    }
    return true;
}

bool SimpleI2C::write(uint8_t data_in[], size_t size_in)
{
    esp_err_t ret = i2c_master_write_to_device(
        _i2c_port,
        _device_address,
        data_in,
        size_in,
        pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS)
    );

    if (ret != ESP_OK) {
        ESP_LOGE("I2C", "Write error: %s", esp_err_to_name(ret));
        return false;
    }
    return true;
}


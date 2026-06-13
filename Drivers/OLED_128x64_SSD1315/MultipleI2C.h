#ifndef _MULTIPLEI2C_H
#define _MULTIPLEI2C_H

#include "driver/i2c_master.h"
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_TIMEOUT_MS 10

class MultipleI2C
{
public:
    MultipleI2C();
    void setup(uint8_t device_address, uint32_t freq = 400000, uint8_t sda_pin = 21, uint8_t scl_pin = 22, i2c_port_t i2c_num = I2C_NUM_0);
    bool read(uint8_t data_in[], size_t size_in, uint8_t data_out[], size_t size_out);
    bool write(uint8_t data_in[], size_t size_in);

private:
    uint8_t _device_address;
    i2c_master_dev_handle_t _dev_handle;
    static i2c_master_bus_handle_t _bus_handle;
    static bool _i2c_initialized;
};

#endif // _MULTIPLEI2C_H

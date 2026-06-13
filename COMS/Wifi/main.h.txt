#pragma once

#include <iostream>
#include <ctime>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_sntp.h"

#include "Wifi.h"

class Main final
{
public:
    void setup(void);
    void run(void);

private:
    WIFI::Wifi wifi;
    WIFI::Wifi::state_e wifiState { WIFI::Wifi::state_e::NOT_INITIALIZED };
    bool ntpStarted { false };

    void initNtp(void);
    void printTime(void);
};



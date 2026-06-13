#include "main.h"

static Main app;

/* ------------------ NTP ------------------ */

void Main::initNtp(void)
{
    std::cout << "Inicializando NTP...\n";

    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();
}

void Main::printTime(void)
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    // Aun no sincronizado
    if (timeinfo.tm_year < (2016 - 1900))
    {
        std::cout << "Esperando hora NTP...\n";
        return;
    }

    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::cout << "Hora actual: " << buffer << "\n";
}

/* ------------------ MAIN LOGIC ------------------ */

void Main::setup(void)
{
    // Inicializacion base ESP-IDF
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(nvs_flash_init());

    // Zona horaria (México ejemplo)
    setenv("TZ", "CST6CDT,M3.2.0,M11.1.0", 1);
    tzset();

    // Credenciales WiFi
    wifi.SetCredentials("IZZI-EFC1", "3C046109EFC1");

    // Inicializa WiFi
    ESP_ERROR_CHECK(wifi.Init());

    std::cout << "Setup completo\n";
}

void Main::run(void)
{
    wifiState = wifi.GetState();

    switch (wifiState)
    {
    case WIFI::Wifi::state_e::READY_TO_CONNECT:
        std::cout << "WiFi: READY_TO_CONNECT\n";
        wifi.Begin();
        break;

    case WIFI::Wifi::state_e::CONNECTED:
        if (!ntpStarted)
        {
            std::cout << "WiFi conectado\n";
            initNtp();
            ntpStarted = true;
        }
        printTime();
        break;

    case WIFI::Wifi::state_e::DISCONNECTED:
        std::cout << "WiFi: DISCONNECTED\n";
        wifi.Begin();
        break;

    default:
        break;
    }
}

/* ------------------ app_main ------------------ */

extern "C" void app_main(void)
{
    app.setup();

    while (true)
    {
        app.run();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

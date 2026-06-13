#ifndef ESPNOW_H
#define ESPNOW_H

#include "esp_now.h"
#include <vector>
#include <array>
#include <stdint.h>
#include "nvs_flash.h"
#include "esp_log.h"

#define MESSAGE_MAX_LEN 250  // Ajusta el tamaño máximo del mensaje según tus necesidades

// Definición de la estructura para los datos de ESP-NOW
typedef struct {
    char message[MESSAGE_MAX_LEN];
} espnow_data_t;

class ESPNow {
public:
    ESPNow();
    ~ESPNow();
    void setup(const std::vector<std::array<uint8_t, 6>> &peer_macs);
    void addPeer(const uint8_t mac[6]);
    void send_data(const char* message, const uint8_t mac[6]);
    bool is_data_received();
    espnow_data_t get_received_data();
    void get_esp_mac();

private:
    bool data_received;
    espnow_data_t received_data;

    static ESPNow* instance;

    void init_nvs();
    void wifi_init();
    void espnow_init();

    static void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void on_data_recv(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len);
};

#endif // ESPNOW_H
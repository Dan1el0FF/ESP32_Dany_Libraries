#include "ESPNow.h"
#include <string.h>
#include <stdio.h>
#include "esp_wifi.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"

#ifndef MACSTR
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

#ifndef MAC2STR
#define MAC2STR(mac) (mac)[0], (mac)[1], (mac)[2], (mac)[3], (mac)[4], (mac)[5]
#endif

static const char *TAG = "ESP-NOW";
ESPNow* ESPNow::instance = nullptr;

ESPNow::ESPNow() : data_received(false) {
    instance = this;  // Almacena la instancia actual
}

ESPNow::~ESPNow(){
}

void ESPNow::setup(const std::vector<std::array<uint8_t, 6>> &peer_macs) {
    init_nvs();
    wifi_init();
    espnow_init();

    for (const auto &mac : peer_macs) {
        addPeer(mac.data());
    }
}

void ESPNow::addPeer(const uint8_t mac[6]) {
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, mac, 6);
    peerInfo.channel = 1;  // Configuración común para todos los peers
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        ESP_LOGE(TAG, "Error al añadir el peer");
    }
}

void ESPNow::send_data(const char* message, const uint8_t mac[6]) {
    espnow_data_t data_to_send;
    strncpy(data_to_send.message, message, sizeof(data_to_send.message) - 1);
    data_to_send.message[sizeof(data_to_send.message) - 1] = '\0';

    esp_err_t result = esp_now_send(mac, (const uint8_t *)&data_to_send, sizeof(data_to_send));
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Fallo al enviar datos, intentando de nuevo");
        esp_now_send(mac, (const uint8_t *)&data_to_send, sizeof(data_to_send));  // Reintento
    }
}

bool ESPNow::is_data_received() {
    return data_received;
}

espnow_data_t ESPNow::get_received_data() {
    data_received = false;
    return received_data;
}

void ESPNow::get_esp_mac() {
    uint8_t mac[6];
    esp_err_t mac_result = esp_wifi_get_mac(WIFI_IF_STA, mac);
    if (mac_result == ESP_OK) {
        ESP_LOGI(TAG, "ESP MAC Address: %02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else {
        ESP_LOGE(TAG, "Error getting MAC address: %d", mac_result);
    }
}

void ESPNow::init_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void ESPNow::wifi_init() {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));  // Desactiva el modo de ahorro de energía para respuestas rápidas
    ESP_ERROR_CHECK(esp_wifi_start());
}

void ESPNow::espnow_init() {
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(on_data_sent));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(on_data_recv));
}

void ESPNow::on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    // Puedes agregar lógica para manejar el resultado de envíos si lo necesitas
}

// Callback para manejar los eventos de recepción
void ESPNow::on_data_recv(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len) {

    // Usamos el puntero 'instance' para acceder a los miembros no estáticos
    if (instance != nullptr) {
        memcpy(instance->received_data.message, data, data_len);  // Copiar los datos recibidos
        instance->data_received = true;  // Indicar que se recibieron datos
    }
}
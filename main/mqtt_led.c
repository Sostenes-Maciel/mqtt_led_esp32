#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "driver/gpio.h"

#define LED_GPIO GPIO_NUM_2
static const char *TAG = "MQTT_LED";

// Configure seu Wi-Fi aqui:
#define WIFI_SSID "AGT_NALVA_2.4"
#define WIFI_PASS "16122001"

// Evento Wi-Fi
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "WiFi conectado com IP!");
    }
}

// Inicializa Wi-Fi no modo estação
void wifi_init_sta(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id, instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta terminou.");
}

// Evento MQTT
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Conectado ao broker MQTT!");
            esp_mqtt_client_subscribe(client, "/ifpe/ads/embarcados/esp32/led", 0);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Tópico: %.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "Mensagem: %.*s", event->data_len, event->data);

            if (strncmp(event->data, "1", event->data_len) == 0) {
                gpio_set_level(LED_GPIO, 1);
                ESP_LOGI(TAG, "LED ACESO");
            } else if (strncmp(event->data, "0", event->data_len) == 0) {
                gpio_set_level(LED_GPIO, 0);
                ESP_LOGI(TAG, "LED APAGADO");
            }
            break;
        default:
            break;
    }
}

void app_main(void) {
    // Configura GPIO do LED
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    // Inicializa NVS
    ESP_ERROR_CHECK(nvs_flash_init());

    // Inicializa Wi-Fi
    wifi_init_sta();

    // Configura MQTT para broker público HiveMQ
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://broker.hivemq.com:1883"
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

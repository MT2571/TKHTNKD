#include "app_mqtt.h"
#include "app_ble.h"
void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    app_mqtt_start();
    app_ble_start();

}

#include "global_data.h"

dados_t dados;
dsm501a_t dsm_receive;
bme280_t bme_receive;
inmp441_t inmp_receive;
esp_mqtt_client_handle_t client;

void send_data_task ( void *pvParameters )
{
    mqtt_app_start();
    esp_task_wdt_add(NULL);
    while (true) {
        if (xQueueReceive(inmp_queue, &inmp_receive, portMAX_DELAY))
            dados.ruido = inmp_receive.ruido;

        if (xQueueReceive(dsm_queue, &dsm_receive, portMAX_DELAY))
        {
            dados.poeira_pm_10 = dsm_receive.poeira_pm_10;
            dados.poeira_pm_25 = dsm_receive.poeira_pm_25;
        }

        if (xQueueReceive(bme_queue, &bme_receive, portMAX_DELAY))
        {
            dados.temperatura = bme_receive.temperatura;
            dados.umidade = bme_receive.umidade;
            dados.pressao = bme_receive.pressao;
            xTaskNotifyGive(xTaskGetCurrentTaskHandle());
        }

        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY)) {
            ESP_LOGI(__func__, "BME280(%.2f*C, %.2f%%, %dPa), DSM501a(%.2fpcs/0.01cf, %.2fpcs/0.01cf), INMP441(%.2fdB).", 
                dados.temperatura, dados.umidade, dados.pressao, dados.poeira_pm_10, dados.poeira_pm_25, dados.ruido);
            
            char tmp[21];
            sprintf(tmp, "%.2f", dados.temperatura);
            esp_mqtt_client_publish(client, "/bme280/temperatura", tmp, 0, 0, 0);
            sprintf(tmp, "%.2f", dados.umidade);
            esp_mqtt_client_publish(client, "/bme280/umidade", tmp, 0, 0, 0);
            sprintf(tmp, "%d", dados.pressao);
            esp_mqtt_client_publish(client, "/bme280/pressao", tmp, 0, 0, 0);
            sprintf(tmp, "%.2f", dados.poeira_pm_10);
            esp_mqtt_client_publish(client, "/bme280/poeira_pm_10", tmp, 0, 0, 0);
            sprintf(tmp, "%.2f", dados.poeira_pm_25);
            esp_mqtt_client_publish(client, "/bme280/poeira_pm_25", tmp, 0, 0, 0);
            sprintf(tmp, "%.2f", dados.ruido);
            esp_mqtt_client_publish(client, "/bme280/ruido", tmp, 0, 0, 0);

            esp_task_wdt_reset();
        }
    }
}

void mqtt_app_start(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, esp_mqtt_event_id_t(ESP_EVENT_ANY_ID), mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

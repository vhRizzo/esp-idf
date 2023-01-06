#include "global_data.h"

dados_t dados;

#ifdef INMP441_SENSOR
inmp441_t inmp_receive;
#endif
#ifdef DSM501A_SENSOR
dsm501a_t dsm_receive;
#endif
#ifdef BME280_SENSOR
bme280_t bme_receive;
#endif
#ifdef NEO6M_SENSOR
neo6m_t neo_receive;
#endif

void rcv_data_task ( void *pvParameters )
{
    mqtt_app_start();

    dados.poeira_pm_10 = -1;
    dados.poeira_pm_25 = -1;
    dados.temperatura = -1;
    dados.umidade = -1;
    dados.pressao = -1;
    dados.ruido = -1;
    dados.coord[0] = -1;
    dados.coord[1] = -1;

    esp_timer_create_args_t data_timer_args = {
        .callback = &send_data_timer,
        .name = "data_timer"
    };

    esp_timer_handle_t data_timer;
    ESP_ERROR_CHECK(esp_timer_create(&data_timer_args, &data_timer));

    esp_timer_create_args_t startup_timer_args = {
        .callback = &startup_timer_func,
        .arg = (void *) data_timer,
        .name = "startup_timer"
    };

    esp_timer_handle_t startup_timer;
    ESP_ERROR_CHECK(esp_timer_create(&startup_timer_args, &startup_timer));
    ESP_ERROR_CHECK(esp_timer_start_once(startup_timer, TEMPO_STARTUP * 1000000));

    while (true) {
        #ifdef INMP441_SENSOR
        if (xQueueReceive(inmp_queue, &inmp_receive, portMAX_DELAY)) 
        {
            dados.ruido = inmp_receive.ruido;
        }
        #endif

        #ifdef DSM501A_SENSOR
        if (xQueueReceive(dsm_queue, &dsm_receive, portMAX_DELAY))
        {
            dados.poeira_pm_10 = dsm_receive.poeira_pm_10;
            dados.poeira_pm_25 = dsm_receive.poeira_pm_25;
        }
        #endif

        #ifdef BME280_SENSOR
        if (xQueueReceive(bme_queue, &bme_receive, portMAX_DELAY))
        {
            dados.temperatura = bme_receive.temperatura;
            dados.umidade = bme_receive.umidade;
            dados.pressao = bme_receive.pressao;
        }
        #endif

        #ifdef NEO6M_SENSOR
        if (xQueueReceive(neo_queue, &neo_receive, portMAX_DELAY))
        {
            dados.coord[0] = neo_receive.coord[0];
            dados.coord[1] = neo_receive.coord[1];
        }
        #endif
    }
}

void startup_timer_func (void *arg)
{
    ESP_ERROR_CHECK(esp_timer_start_periodic((esp_timer_handle_t)arg, TEMPO_ANALISE * 1000000));
}

void send_data_timer (void *arg)
{
    ESP_LOGI(__func__, "BME280(%.2f*C, %.2f%%, %dPa), DSM501a(%.2fpcs/0.01cf, %.2fpcs/0.01cf), INMP441(%.2fdB), NEO-6M(%.6f, %.6f).", 
                dados.temperatura, dados.umidade, dados.pressao, 
                dados.poeira_pm_10, dados.poeira_pm_25, 
                dados.ruido, 
                dados.coord[0], dados.coord[1]);
            
    char topic[51];
    char data[51];
    sprintf(topic, "%s/bme280", CLIENT_ID);
    sprintf(data, "{temperatura:%.2f}", dados.temperatura);
    esp_mqtt_client_publish(client, topic, data, 0, 0, 0);
    sprintf(data, "{umidade:%.2f}", dados.umidade);
    esp_mqtt_client_publish(client, topic, data, 0, 0, 0);
    sprintf(data, "{pressao:%d}", dados.pressao);
    esp_mqtt_client_publish(client, topic, data, 0, 0, 0);

    sprintf(topic, "%s/dsm501a", CLIENT_ID);
    sprintf(data, "{poeira_pm_10:%.2f}", dados.poeira_pm_10);
    esp_mqtt_client_publish(client, topic, data, 0, 0, 0);
    sprintf(data, "{poeira_pm_25:%.2f}", dados.poeira_pm_25);
    esp_mqtt_client_publish(client, topic, data, 0, 0, 0);
    
    sprintf(topic, "%s/inmp441", CLIENT_ID);
    sprintf(data, "{ruido:%.2f}", dados.ruido);
    esp_mqtt_client_publish(client, topic, data, 0, 0, 0);
    
    sprintf(topic, "%s/neo6m", CLIENT_ID);
    if (abs(dados.coord[0]) <= 90 && dados.coord[0] != 0) {
        sprintf(data, "{gps_latitude:%.6f}", dados.coord[0]);
        esp_mqtt_client_publish(client, topic, data, 0, 0, 0);
    }
    if (abs(dados.coord[1]) <= 180 && dados.coord[1] != 0) {
        sprintf(data, "{gps_longitude:%.6f}", dados.coord[1]);
        esp_mqtt_client_publish(client, topic, data, 0, 0, 0);
    }
}

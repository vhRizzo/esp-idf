#include "global_data.h"
#include "Modem_SmartModular.hpp"

dados_t dados;
char tmp[100];
char rcv[1000];
char rcv_gps[1000];
uint16_t porta = 20;      //aqui deve-se escolhar >=1 , sendo útil para indicar qual parte da aplicação pertence o dado obtido!!!!
uint32_t rcv_size = 0;
uint32_t send_size = 0;
uint16_t porta_gps = 19;
uint32_t rcv_gps_size = 0;
Modem_SmartModular teste(UART_NUM_2);
int cont_gps = (int)floor(CONTAGEM_GPS*(2./3.));

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
    dados.poeira_pm_10 = -1;
    dados.poeira_pm_25 = -1;
    dados.temperatura = -1;
    dados.umidade = -1;
    dados.pressao = -1;
    dados.coord[0] = -1;
    dados.coord[1] = -1;
    memset(rcv,0,1000);
    memset(rcv_gps,0,1000);
    
    if(strcmp(teste.auto_join(),"0")==0){
        ESP_LOGI("APP", "AutoJoin ativado: %s", teste.auto_join(1));
    }
    else{
        ESP_LOGI("APP", "AutoJoin já ativado...");
    }

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
    // ESP_LOGI(__func__, "BME280(%.2f*C, %.2f%%, %dPa), DSM501a(%.2fpcs/0.01cf, %.2fpcs/0.01cf), NEO-6M(%.6f, %.6f).", 
    //             dados.temperatura, dados.umidade, dados.pressao, 
    //             dados.poeira_pm_10, dados.poeira_pm_25, 
    //             dados.coord[0], dados.coord[1]);

    char tmp[100];
    sprintf(tmp, "%.2f,%.2f,%d;%.2f,%.2f", 
            dados.temperatura, dados.umidade, dados.pressao, 
            dados.poeira_pm_10, dados.poeira_pm_25);
    send_size = strlen(tmp);
    ESP_LOGI(__func__, "ENVIOU = DADOS: %s | TAM: %i", tmp_38, send_size);
    // teste.enviar_receber(porta, tmp, send_size, rcv, &rcv_size);
    ESP_LOGI(__func__, "RECEBEU = DADOS: %s | TAM: %i", rcv, (int)rcv_size);
    cont_gps++;
    if (cont_gps >= CONTAGEM_GPS) {
        char tmp_gps[100];
        sprintf(tmp_gps, "%.6f,%.6f", dados.coord[0], dados.coord[1]);
        send_size = strlen(tmp_gps);
        ESP_LOGI(__func__, "ENVIOU = DADOS: %s | TAM: %i", tmp_gps, send_size);
        // teste.enviar_receber(porta_gps, tmp_gps, send_size, rcv_gps, &rcv_gps_size);
        ESP_LOGI(__func__, "RECEBEU = DADOS: %s | TAM: %i", rcv_gps, (int)rcv_gps_size);
        cont_gps = 0;
    }
}

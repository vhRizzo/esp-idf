#include "global_data.h"
#include "Modem_SmartModular.hpp"

dados_t dados = {
    .temperatura = -1,
    .umidade = -1,
    .pressao = (uint32_t)-1,
    .dallas_temp = -1,
    .irrad = -1,
    .poeira_pm_10 = -1,
    .poeira_pm_25 = -1,
    .coord = {-1,-1}
};
char tmp[100];
char rcv[1000];
char rcv_gps[1000];
uint16_t porta = 20;      //aqui deve-se escolher >=1 , sendo útil para indicar qual parte da aplicação pertence o dado obtido!!!!
uint32_t rcv_size = 0;
uint32_t send_size = 0;
uint16_t porta_gps = 19;
uint16_t porta_sol = 18;
uint32_t rcv_gps_size = 0;
Modem_SmartModular teste(LOR_UART_PORT);
int cont_gps = (int)floor(CONTAGEM_GPS*(2./3.));

#ifdef DSM501A_SENSOR
dsm501a_t dsm_rcv;
#endif
#ifdef BME280_SENSOR
bme280_t bme_rcv;
#endif
#ifdef NEO6M_SENSOR
neo6m_t neo_rcv;
#endif
#ifdef DS18B20_SENSOR
ds18b20_t dallas_rcv;
#endif
#ifdef BPW34_SENSOR
bpw34_t bpw_rcv;
#endif

void rcv_data_task ( void *pvParameters )
{
    while (1)
    {
        #ifdef DSM501A_SENSOR
        if (xQueueReceive(dsm_queue, &dsm_rcv, portMAX_DELAY))
        {
            dados.poeira_pm_10 = dsm_rcv.poeira_pm_10;
            dados.poeira_pm_25 = dsm_rcv.poeira_pm_25;
        }
        #endif

        #ifdef BME280_SENSOR
        if (xQueueReceive(bme_queue, &bme_rcv, portMAX_DELAY))
        {
            dados.temperatura = bme_rcv.temperatura;
            dados.umidade = bme_rcv.umidade;
            dados.pressao = bme_rcv.pressao;
        }
        #endif

        #ifdef NEO6M_SENSOR
        if (xQueueReceive(neo_queue, &neo_rcv, portMAX_DELAY))
        {
            if (fabs(neo_rcv.coord[0]) > 0 && fabs(neo_rcv.coord[0]) <= 90)
                dados.coord[0] = neo_rcv.coord[0];
            if (fabs(neo_rcv.coord[1]) > 0 && fabs(neo_rcv.coord[1]) <= 180)
                dados.coord[1] = neo_rcv.coord[1];
        }
        #endif

        #ifdef DS18B20_SENSOR
        if (xQueueReceive(ds18b20_queue, &dallas_rcv, portMAX_DELAY))
        {
            dados.dallas_temp = dallas_rcv.dallas_temp;
        }
        #endif
        
        #ifdef BPW34_SENSOR
        if (xQueueReceive(bpw34_queue, &bpw_rcv, portMAX_DELAY))
        {
            dados.irrad = bpw_rcv.irrad;
        }
        #endif
    }
}

void snd_data_task ( void *pvParameters )
{
    memset(rcv,0,1000);
    memset(rcv_gps,0,1000);
    
    teste.OTAA_DEUI();
    #ifdef SERIAL_DEBUG
    ESP_LOGI(__func__, "%s", teste.OTAA_APPKEY());
    #endif

    if(strcmp(teste.auto_join(),"0")==0){
        #ifdef SERIAL_DEBUG
        ESP_LOGI("APP", "AutoJoin ativado: %s", teste.auto_join(1));
        #endif
    }
    else{
        #ifdef SERIAL_DEBUG
        ESP_LOGI("APP", "AutoJoin já ativado...");
        #endif
    }

    vTaskDelay( (TEMPO_STARTUP*1000)/portTICK_PERIOD_MS );
    while(1)
    {
        vTaskDelay( (TEMPO_ANALISE*1000)/portTICK_PERIOD_MS );
        char tmp[100];
        sprintf(tmp, "%.2f,%.2f,%d;%.2f,%.2f", 
                dados.temperatura, dados.umidade, (int)dados.pressao, 
                dados.poeira_pm_10, dados.poeira_pm_25);
        send_size = strlen(tmp);
        #ifdef SERIAL_DEBUG
        ESP_LOGI(__func__, "ENVIOU = DADOS: %s | TAM: %i | PORTA: %d", tmp, (int)send_size, porta);
        #endif
        teste.enviar_receber(porta, tmp, send_size, rcv, &rcv_size);
        #ifdef SERIAL_DEBUG
        ESP_LOGI(__func__, "RECEBEU = DADOS: %s | TAM: %i", rcv, (int)rcv_size);
        #endif

        sprintf(tmp, "%.2f,%.2f", 
                dados.dallas_temp, dados.irrad);
        send_size = strlen(tmp);
        #ifdef SERIAL_DEBUG
        ESP_LOGI(__func__, "ENVIOU = DADOS: %s | TAM: %i | PORTA: %d", tmp, (int)send_size, porta_sol);
        #endif
        teste.enviar_receber(porta_sol, tmp, send_size, rcv, &rcv_size);
        #ifdef SERIAL_DEBUG
        ESP_LOGI(__func__, "RECEBEU = DADOS: %s | TAM: %i", rcv, (int)rcv_size);
        #endif

        #ifdef NEO6M_SENSOR
        cont_gps++;
        if (cont_gps >= CONTAGEM_GPS) {
            char tmp_gps[100];
            sprintf(tmp_gps, "%.6f,%.6f", dados.coord[0], dados.coord[1]);
            send_size = strlen(tmp_gps);
            #ifdef SERIAL_DEBUG
            ESP_LOGI(__func__, "ENVIOU = DADOS: %s | TAM: %i | PORTA: %d", tmp_gps, (int)send_size, porta_gps);
            #endif
            teste.enviar_receber(porta_gps, tmp_gps, send_size, rcv_gps, &rcv_gps_size);
            #ifdef SERIAL_DEBUG
            ESP_LOGI(__func__, "RECEBEU = DADOS: %s | TAM: %i", rcv_gps, (int)rcv_gps_size);
            #endif
            cont_gps = 0;
        }
        #endif
    }
}

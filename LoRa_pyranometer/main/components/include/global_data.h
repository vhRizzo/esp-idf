#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#ifdef __cplusplus
extern "C" {
#endif /* C++ Guard */

/* Geral */
#define TEMPO_ANALISE   30  // segundos
#define TEMPO_STARTUP   5   // segundos
#define CONTAGEM_GPS    3  // a cada quantas coletas de dados serão enviadas as coordenadas GPS.
#define DSM501A_SENSOR
#define BME280_SENSOR
#define NEO6M_SENSOR
#define DS18B20_SENSOR
#define BPW34_SENSOR
#define SERIAL_DEBUG

/* Pinout */
#define BME_SDA_PIN     GPIO_NUM_21 // (D21)
#define BME_SCL_PIN     GPIO_NUM_22 // (D22)
#define DSM_P10_PIN     GPIO_NUM_34 // (D34)
#define DSM_P25_PIN     GPIO_NUM_35 // (D35)
#define NEO_UTX_PIN     GPIO_NUM_32 // (D32)
#define NEO_URX_PIN     GPIO_NUM_33 // (D33)
#define LORA_TX_PIN     GPIO_NUM_12 // (D12)
#define LORA_RX_PIN     GPIO_NUM_13 // (D13)
#define DS18B20_PIN     GPIO_NUM_27 // (D27)
#define BPW_ADC_PIN     GPIO_NUM_36 // (VP)
/*  
 *  O ESP32 tem 2 unidades de ADC, a unidade 2 possui algumas limitacoes, portanto e preferivel
 *  a utilizacao da unidade 1.
 *  A unidade 1 possui 7 canais disponiveis, presentes nos pinos 32 a 39.
 */

/* Portas UART */
#define NEO_UART_PORT   UART_NUM_1
#define LOR_UART_PORT   UART_NUM_2

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp32/rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "esp_adc/adc_cali.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_task_wdt.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_now.h"

void rcv_data_task ( void *pvParameters );
void snd_data_task ( void *pvParameters );

#ifdef DSM501A_SENSOR
typedef struct __attribute__((__packed__))
{
    float poeira_pm_10;
    float poeira_pm_25;
} dsm501a_t;

extern QueueHandle_t dsm_queue;
#endif

#ifdef BME280_SENSOR
typedef struct __attribute__((__packed__))
{
    float temperatura;
    float umidade;
    uint32_t pressao;
} bme280_t;

extern QueueHandle_t bme_queue;
#endif

#ifdef NEO6M_SENSOR
typedef struct __attribute__((__packed__))
{
    float coord[2]; //[0] Lat , [1] Lng
} neo6m_t;

extern QueueHandle_t neo_queue;
#endif

#ifdef DS18B20_SENSOR
typedef struct __attribute__((__packed__))
{
    float dallas_temp;
} ds18b20_t;

extern QueueHandle_t ds18b20_queue;
#endif

#ifdef BPW34_SENSOR
typedef struct __attribute__((__packed__))
{
    float irrad;
} bpw34_t;

extern QueueHandle_t bpw34_queue;
#endif

typedef struct __attribute__((__packed__)) //esse atributo informa ao compilador para utilizar o mínimo de memória para tipos dentro da struct ou da union
{
    float temperatura;     // 4 bytes
    float umidade;         // 4 bytes
    uint32_t pressao;       // 2 bytes
    float dallas_temp;      // 4 bytes
    float irrad;            // 4 bytes
    float poeira_pm_10;     // 4 bytes
    float poeira_pm_25;     // 4 bytes
    float coord[2];         // 8 bytes
} dados_t;

#ifdef __cplusplus
}
#endif /* C++ Guard */

#endif /* GLOBAL_DATA_H */

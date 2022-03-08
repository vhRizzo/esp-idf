#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/freeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"
#include "driver/i2s.h"
#include "driver/i2c.h"
#include "mqtt_setup.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_now.h"

#define TEMPO_ANALISE 5

void send_data_task ( void *pvParameters );
void mqtt_app_start(void);

typedef struct __attribute__((__packed__))
{
    float poeira_pm_10;
    float poeira_pm_25;
} dsm501a_t;

typedef struct __attribute__((__packed__))
{
    float temperatura;
    float umidade;
    uint32_t pressao;
} bme280_t;

typedef struct __attribute__((__packed__))
{
    double ruido;
} inmp441_t;

typedef struct __attribute__((__packed__)) //esse atributo informa ao compilador para utilizar o mínimo de memória para tipos dentro da struct ou da union
{
    float  temperatura;      //4 bytes
    float  umidade;          //4 bytes
    uint32_t pressao;        //2 bytes
    float poeira_pm_10;      //4 bytes
    float poeira_pm_25;      //4 bytes
    double ruido;
    float coord[2];          //8 bytes 
} dados_t;

extern QueueHandle_t dsm_queue;
extern QueueHandle_t bme_queue;
extern QueueHandle_t inmp_queue;

#endif /* GLOBAL_DATA_H */

#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/i2s.h"
#include "driver/i2c.h"
#include "mqtt_setup.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_now.h"

/* Geral */
#define TEMPO_ANALISE   15 //s
#define TEMPO_STARTUP   5 //s 
// #define DSM501A_SENSOR
// #define INMP441_SENSOR
#define BME280_SENSOR
#define NEO6M_SENSOR

/* WiFi */
#define WIFI_SSID       "Herbert Richers"
#define WIFI_PASS       "kinodertoten"
#define MAXIMUM_RETRY   10

/* MQTT */
#define BROKER_URL      "mqtt://user1:gabriel@iot.coenc.ap.utfpr.edu.br"
#define BME_TEMP_TOPIC  "/bme280/temp"
#define BME_UMID_TOPIC  "/bme280/umid"
#define BME_PRES_TOPIC  "/bme280/pres"
#define DSM_PM10_TOPIC  "/dsm501a/pm10"
#define DSM_PM25_TOPIC  "/dsm501a/pm25"
#define _INMP_DB_TOPIC  "/inmp441/dB"
#define _NEO_LAT_TOPIC  "/neo6m/lat"
#define _NEO_LNG_TOPIC  "/neo6m/lng"

/* OTA */
#define OTA_FIRM_URL    "https://192.168.0.102:8070/school_monitor.bin"

void send_data_task ( void *pvParameters );
void mqtt_app_start(void);

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

#ifdef INMP441_SENSOR
typedef struct __attribute__((__packed__))
{
    double ruido;
} inmp441_t;

extern QueueHandle_t inmp_queue;
#endif

#ifdef NEO6M_SENSOR
typedef struct __attribute__((__packed__))
{
    float coord[2]; //[0] Lat , [1] Lng
} neo6m_t;

extern QueueHandle_t neo_queue;
#endif

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

#endif /* GLOBAL_DATA_H */

/*
 * Implementation of tasks to send read data to server via MQTT.
 * 
 * (c)2022 Victor Moura
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *    
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

/* Geral */
#define TEMPO_ANALISE   60 //s
#define TEMPO_STARTUP   60 //s 
#define DSM501A_SENSOR
#define INMP441_SENSOR
#define BME280_SENSOR
#define NEO6M_SENSOR

/* WiFi */
#define WIFI_SSID       "UTFPR-IOT"
#define WIFI_PASS       "c*98MzkFy"
#define MAXIMUM_RETRY   10

/* MQTT */
#define BROKER_URL      "mqtt://user1:gabriel@iot.coenc.ap.utfpr.edu.br"
#define CLIENT_ID       "utfpr_ap"

/* OTA */
#define OTA_FIRM_URL    "http://iot.coenc.ap.utfpr.edu.br/atualizar/utfpr_ap/school_monitor.bin"
#define TEMPO_OTA_CHECK 300

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

void rcv_data_task ( void *pvParameters );
void startup_timer_func (void *arg);
void send_data_timer (void *arg);
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

extern esp_mqtt_client_handle_t client;

#endif /* GLOBAL_DATA_H */

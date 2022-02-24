#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/freeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_now.h"

#define TEMPO_ANALISE 5

void print_task( void *pvParameters );

typedef struct __attribute__((__packed__))
{
    float poeira_pm_10;
    float poeira_pm_25;
} dsm501a_t;

extern QueueHandle_t dsm_queue;

#endif /* GLOBAL_DATA_H */
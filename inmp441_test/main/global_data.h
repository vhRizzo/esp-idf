#ifndef GLOBAL_DATA_H_
#define GLOBAL_DATA_H_

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/freeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_now.h"

#define TEMPO_ANALISE   5

void print_task( void *pvParameters );

typedef struct __attribute__((__packed__)) {
    double ruido;
} inmp441_t;

extern QueueHandle_t inmp_queue;

#endif /* GLOBAL_DATA_H_ */
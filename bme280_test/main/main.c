#include <stdio.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "nvs_flash.h"
#include "esp_err.h"

#include "global_data.h"
#include "bme280_setup.h"

void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());

    xTaskCreatePinnedToCore(bme280_task, "BME280_Task", 3072, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(print_task, "Print_Task", 2048, NULL, 2, NULL, 0);
}
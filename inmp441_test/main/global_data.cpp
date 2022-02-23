#include "global_data.h"

void print_task ( void *pvParameters )
{
    inmp441_t inmp;
    while (true) {
        if (xQueueReceive(inmp_queue, &inmp, portMAX_DELAY)) {
            ESP_LOGI(__func__, "%.2f", inmp.ruido);
        }
    }
}

#include "global_data.h"

void print_task ( void *pvParameters )
{
    dsm501a_t dsm_receive;
    while (true) {
        if (xQueueReceive(dsm_queue, &dsm_receive, portMAX_DELAY))
        {
            ESP_LOGI(__func__, "DSM501a(%.2fpcs/0.01cf, %.2fpcs/0.01cf).", 
                dsm_receive.poeira_pm_10, dsm_receive.poeira_pm_25);
        }
    }
}

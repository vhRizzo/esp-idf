#include "dsm501a_setup.h"

extern "C" {
    void app_main(void);
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    dsm_queue = xQueueCreate(1, sizeof(dsm501a_t));

    xTaskCreatePinnedToCore(dsm501a_task, "DSM501a_Task", 3072, NULL, 7, NULL, 0);
    xTaskCreatePinnedToCore(print_task, "Print_Task", 2048, NULL, 1, NULL, 1);
}

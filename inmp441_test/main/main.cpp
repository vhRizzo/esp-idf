#include "inmp441_setup.h"

extern "C" {
    void app_main(void);
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    xTaskCreatePinnedToCore(print_task, "Print_Task", 1024, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(mic_i2s_reader_task, "INMP441_Task", 2048, NULL, 4, NULL, 0);
    xTaskCreatePinnedToCore(mic_i2s_filter_task, "INMP441_Task", 2048, NULL, 5, NULL, 0);
}

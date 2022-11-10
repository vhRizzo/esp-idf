extern "C" {

#include "global_data.h"
#include "Modem_SmartModular.hpp"

#ifdef DSM501A_SENSOR
#include "dsm501a_setup.h"
#endif
#ifdef BME280_SENSOR
#include "bme280_setup.h"
#endif
#ifdef NEO6M_SENSOR
#include "neo6m_setup.h"
#endif

void app_main(void);
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    gpio_pad_select_gpio(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_2, 0);
    vTaskDelay( 1000/portTICK_PERIOD_MS );
    gpio_set_level(GPIO_NUM_2, 1);

    #ifdef BME280_SENSOR
    bme_queue = xQueueCreate(1, sizeof(bme280_t));
    xTaskCreatePinnedToCore(bme280_task, "BME280_Task", 4 * 1024, NULL, configMAX_PRIORITIES - 6, NULL, 0);
    #endif

    #ifdef DSM501A_SENSOR
    dsm_queue = xQueueCreate(1, sizeof(dsm501a_t));
    xTaskCreatePinnedToCore(dsm501a_task, "DSM501a_Task", 4 * 1024, NULL, configMAX_PRIORITIES - 7, NULL, 0);
    #endif

    #ifdef NEO6M_SENSOR
    neo_queue = xQueueCreate(1, sizeof(neo6m_t));
    xTaskCreatePinnedToCore(neo6m_task, "NEO-6M_Task", 4 * 1024, NULL, configMAX_PRIORITIES - 8, NULL, 1);
    #endif

    //***** Fazer apenas uma vez!!!!
    //teste.OTAA_APPEUI("00:00:00:00:00:00:00:00");
    //teste.OTAA_APPKEY("D4:42:C3:A0:40:02:97:E4:BD:AC:CD:86:F4:6A:36:D2");
    //teste.salvar_config(); 
    //***** As configs ficaram salvas para o auto_join ser ativado !

    vTaskDelay( 1000/portTICK_PERIOD_MS );
    xTaskCreatePinnedToCore(rcv_data_task, "Data_Receive_Task", 4 * 1024, NULL, configMAX_PRIORITIES - 2, NULL, 1);
}
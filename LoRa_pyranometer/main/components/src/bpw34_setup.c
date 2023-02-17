#include "global_data.h"

#ifdef BPW34_SENSOR

#include "bpw34_setup.h"

bpw34_t bpw = {0};
static int adc_raw;
QueueHandle_t bpw34_queue;

void bpw34_task ( void *pvParameters )
{
    ESP_LOGI(__func__, "ADC Pin: %i, ADC Channel: %i", (int)BPW_ADC_PIN, (int)ADC_CHAN);
    //-------------ADC1 Init---------------//
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHAN, &config));

    uint64_t now = esp_timer_get_time();
    unsigned long new_raw = 0;
    uint32_t mean_count = 0;

    while (true)
    {
        mean_count++;
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHAN, &adc_raw));
        new_raw += adc_raw;
        if (esp_timer_get_time() - now >= TEMPO_ANALISE * 1000000)
        {
            int res_raw = new_raw / mean_count;
            float voltage = (res_raw * 3.3 / 4095);
            bpw.irrad = voltage / (RF * (PD_AREA*1e-6) * PD_SENS);
            xQueueOverwrite(bpw34_queue, &bpw);
            // ESP_LOGI(__func__, "RAW: %d | Voltage: %.2f mV | Irrad: %.2f W/m2", res_raw, voltage*1e3, bpw.irrad);
            new_raw = 0;
            mean_count = 0;
            now = esp_timer_get_time();
        }
        vTaskDelay(1);
    }

    //Tear Down
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc_handle));
}

#endif

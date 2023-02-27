/* Codigo adaptado da documentacao da propria espressif na categoria de conversor analogico-digital "oneshot".
 * Disponivel em: https://github.com/espressif/esp-idf/tree/57b6be22a77/examples/peripherals/adc/oneshot_read */
#include "global_data.h"

#ifdef BPW34_SENSOR

#include "bpw34_setup.h"

bpw34_t bpw = {0};          // Inicializa o item do fotodiodo com o dado de irradiancia em 0
static int adc_raw;         // Variavel para armazenar o valor "cru" lido pelo ADC do ESP
QueueHandle_t bpw34_queue;  // Cria o handler da queue onde sera enviado o item

void bpw34_task ( void *pvParameters )
{
    /* Inicializacao toda baseada no codigo da espressif, apenas removendo porcoes que nao sao necessarias para o projeto. */
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

    uint64_t now = esp_timer_get_time();    // Inicializa o temporizador
    unsigned long new_raw = 0;              // Variavel para acumular as leituras do ESP
    uint32_t mean_count = 0;                // Contador para indicar quantas vezes o valor foi acumulado para calcular a media ao final da leitura

    while (true)
    {
        mean_count++;                       // Incrementa o contador de leituras
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHAN, &adc_raw));  // Realiza a leitura verificando por erros
        new_raw += adc_raw;                 // Acumula o valor lido na variavel de acumulacao
        if (esp_timer_get_time() - now >= TEMPO_ANALISE * 1000000)  // Se tiver passado o tempo definido para o intervalo de medidas...
        {
            int res_raw = new_raw / mean_count; // Calcula a media das leituras
            /* O ESP32 suporta tensao maxima de ate 3.3 V de entrada, E as leituras do ADC conseguem ler ate 12 bits, portanto
             * o valor cru lido pelo ESP tem, idealmente, uma relacao linear de 0 a 4095 quando ler de 0 a 3.3 V.
             * Porem na pratica a relacao nao e totalmente linear, como visto em https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
             * podendo ser necessaria a utilizacao de alguma forma de calibracao, ou ate mesmo a utilizacao de um modulo ADC dedicado.
             * Ate que os testes sejam realizados, faremos uma conversao supondo que a relacao seja linear. */
            float voltage = (res_raw * 3.3 / 4095); // Converte o valor cru medido para um valor de tensao
            // TODO - Verificar se utilizar o recurso de calibracao da esp-idf retorna resultados mais consistentes
            bpw.irrad = voltage / (RF * (PD_AREA*1e-6) * PD_SENS);  // Calcula a irradiacao
            xQueueOverwrite(bpw34_queue, &bpw); // Envia o valor calculado para a fila
            // ESP_LOGI(__func__, "RAW: %d | Voltage: %.2f mV | Irrad: %.2f W/m2", res_raw, voltage*1e3, bpw.irrad);
            new_raw = 0;                        // Reinicia o acumulador de medidas
            mean_count = 0;                     // E o contador
            now = esp_timer_get_time();         // E o tempo de ser comparacao ser o tempo atual
        }
        vTaskDelay(1);                      // Pequeno delay para evitar stack overflow
    }

    //Tear Down
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc_handle));  // Teoricamente nunca chegara aqui, mas se chegar, remove a unidade ADC utilizada
}

#endif

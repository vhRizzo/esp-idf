#ifndef GLOBAL_DATA_H_
#define GLOBAL_DATA_H_

#define TEMPO_ANALISE 5

void print_task (void *pvParameters);

typedef struct __attribute__((__packed__)) {
    float temperatura;
    float umidade;
    float pressao;
} TDados;

TDados tdados;

void print_task (void *pvParameters)
{
    vTaskDelay( ((TEMPO_ANALISE + 1) * 1000)/portTICK_PERIOD_MS );
    while (1) {
        printf("BME280(%.2f, %.2f, %.2f)\r\n", tdados.temperatura, tdados.umidade, tdados.pressao);
        vTaskDelay( (TEMPO_ANALISE*1000)/portTICK_PERIOD_MS );
    }
}

#endif /* GLOBAL_DATA_H_ */
#ifndef INMP441_SETUP_H
#define INMP441_SETUP_H

#include "global_data.h"

/* Configuration */
#define WEIGHTING         A_weighting // Also avaliable: 'None' (Z_weighting)

// NOTE: Some microphones require at least DC-Blocker filter
#define MIC_EQUALIZER     INMP441     // See below for defined IIR filters or set to 'None' to disable
#define MIC_OFFSET_DB     3.0103      // Default offset (sine-wave RMS vs. dBFS). Modify this value for linear calibration

// Valores modificados de acordo com o datasheet do INMP441
#define MIC_SENSITIVITY   -26         // dBFS value expected at MIC_REF_DB (Sensitivity value from datasheet)
#define MIC_REF_DB        94.0        // Value at which point sensitivity is specified in datasheet (dB)
#define MIC_OVERLOAD_DB   116.0       // dB - Acoustic overload point
#define MIC_NOISE_DB      33          // dB - Noise floor
#define MIC_BITS          24          // valid number of bits in I2S data
#define MIC_CONVERT(s)    (s >> (SAMPLE_BITS - MIC_BITS))
#define MIC_TIMING_SHIFT  0           // Set to one to fix MSB timing for some microphones, i.e. SPH0645LM4H-x

#define I2S_WS            5
#define I2S_SCK           4
#define I2S_SD            15
#define I2S_PORT          I2S_NUM_0 // I2S peripheral to use (0 or 1)

/* Sampling */
#define SAMPLE_RATE       48000 // Hz, fixed to design of IIR filters
#define SAMPLE_BITS       32    // bits
#define SAMPLE_T          int32_t 
#define SAMPLES_SHORT     (SAMPLE_RATE / 8) // ~125ms
#define DMA_BANK_SIZE     (SAMPLES_SHORT / 16)
#define DMA_BANKS         32

struct sum_queue_t {
  float sum_sqr_SPL;        // Sum of squares of mic samples, after Equalizer filter
  float sum_sqr_weighted;   // Sum of squares of weighted mic samples
};

void mic_i2s_init();
void mic_i2s_reader_task( void *pvParameters );
void mic_i2s_filter_task( void *pvParameters );

#endif /* INMP441_SETUP_H */
#ifndef BPW34_SETUP_H
#define BPW34_SETUP_H

#include "global_data.h"

//-------------Set ADC channel to use based on pin---------------//
#if   (BPW_ADC_PIN == GPIO_NUM_36)
#define ADC_CHAN            ADC_CHANNEL_0
#elif (BPW_ADC_PIN == GPIO_NUM_37)
#define ADC_CHAN            ADC_CHANNEL_1
#elif (BPW_ADC_PIN == GPIO_NUM_38)
#define ADC_CHAN            ADC_CHANNEL_2
#elif (BPW_ADC_PIN == GPIO_NUM_39)
#define ADC_CHAN            ADC_CHANNEL_3
#elif (BPW_ADC_PIN == GPIO_NUM_32)
#define ADC_CHAN            ADC_CHANNEL_4
#elif (BPW_ADC_PIN == GPIO_NUM_33)
#define ADC_CHAN            ADC_CHANNEL_5
#elif (BPW_ADC_PIN == GPIO_NUM_34)
#define ADC_CHAN            ADC_CHANNEL_6
#elif (BPW_ADC_PIN == GPIO_NUM_35)
#define ADC_CHAN            ADC_CHANNEL_7
#endif

#define ADC_UNIT            ADC_UNIT_1
#define ADC_ATTEN           ADC_ATTEN_DB_11
#define PD_AREA             7.02    // (mm2)
#define PD_SENS             0.62    // (A/W)
#define RF                  680     // (Ω)
/*  
 *  Ip    = Irrad * Area * Sens
 *  Ip    = Vo / Rf
 *  Irrad = Ip / (Area * Sens)
 *  Irrad = Vo / (Rf * Area * Sens)
 * 
 *  Ip    = Photodiode output current.
 *  Irrad = Irradiance.
 *  Area  = Radiant sensitive area.
 *  Sens  = Spectral sensitivity of the chip.
 *  Rf    = Feedback resistor.
 *  Vo    = Output voltage.
 * 
 *  Vo = Ip * Rf - ESP32 supports at max 3.3V as input.
 *  At STC (1000 W/m2 irradiance) Ip will be:
 *  Ip = 1000 * 7.02*1e-6 * 0.62 = 4.35 mA
 *  So Rf has to be such as Vo = 4.35 mA * Rf (Vo < 3.3V)
 *  Being 680 Ω the highest comercial value where this is true.
 */
// https://www.osram.com/ecat/com/en/class_pim_web_catalog_103489/prd_pim_device_2219534/

void bpw34_task ( void *pvParameters );

#endif /* BPW34_SETUP_H */
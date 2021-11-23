#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/adc_common.h"
#include "led_strip.h"

int value;
int * valueADC2 = &value;

void app_main(void)
{
    adc2_config_channel_atten(4, ADC_ATTEN_DB_11);
    adc2_get_raw(4, ADC_WIDTH_BIT_12, valueADC2);
    printf("%d,  %d\n", *valueADC2, value);
}
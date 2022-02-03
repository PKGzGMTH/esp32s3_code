#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led_strip.h"

#define NeoPixel_GPIO 48
static led_strip_t *NeoPixel;

void app_main(void)
{
    NeoPixel = led_strip_init(0, NeoPixel_GPIO, 1);
    NeoPixel -> clear(NeoPixel, 50);
    gpio_reset_pin(1);
    gpio_reset_pin(2);
    gpio_set_direction(1,GPIO_MODE_OUTPUT);
    gpio_set_direction(2,GPIO_MODE_OUTPUT);
    gpio_set_level(1, 0);
    gpio_set_level(2, 1);
    while (1){
        gpio_set_level(1, 1);
        NeoPixel -> set_pixel(NeoPixel, 0, 6, 0, 0);
        NeoPixel -> refresh(NeoPixel, 100);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        gpio_set_level(1, 0);
        NeoPixel -> set_pixel(NeoPixel, 0, 6, 6, 0);
        NeoPixel -> refresh(NeoPixel, 100);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        gpio_set_level(2, 0);
        NeoPixel -> set_pixel(NeoPixel, 0, 0, 0, 6);
        NeoPixel -> refresh(NeoPixel, 100);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        
        gpio_set_level(2, 1);
        NeoPixel -> set_pixel(NeoPixel, 0, 0, 6, 0);
        NeoPixel -> refresh(NeoPixel, 100);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        NeoPixel -> clear(NeoPixel, 50);
    }

}
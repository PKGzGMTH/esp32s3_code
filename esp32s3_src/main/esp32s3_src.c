// H2install library (.h file)  https://vcpkg.io/en/getting-started.html
// use 921600 byte speed baud rate
//delay microsecond use
//ets_delay_us(value)
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led_strip.h"

#define NeoPixel_GPIO 48
#define row 32
#define col 32

static led_strip_t *NeoPixel;
uint8_t R,G,B,data[row][col];
int start = 0;

void app_main(void)
{
    NeoPixel = led_strip_init(0, NeoPixel_GPIO, 1);
    NeoPixel -> clear(NeoPixel, 50);

    while (1){
        printf("%c", start);
        //set_pixel(NeoPixel, location, R, G, B);
        NeoPixel -> set_pixel(NeoPixel, 0, 0, 6, 0);
        NeoPixel -> refresh(NeoPixel, 100);
        for (int j = 0; j < 1024; j++){
            int value = (random()%99)+1;
            printf("%c",value);
        }
        NeoPixel -> clear(NeoPixel, 50);
        vTaskDelay(512 / portTICK_PERIOD_MS);
    }
}

// H2install library (.h file)  https://vcpkg.io/en/getting-started.html
// use 921600 byte speed baud rate
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led_strip.h"

#define BLINK_GPIO 48
#define row 32
#define col 32

static led_strip_t *pStrip_a;
uint8_t R,G,B,data[row][col];
int start = 0;

void app_main(void)
{
    char *ourTaskName = pcTaskGetName(NULL);
    pStrip_a = led_strip_init(0, BLINK_GPIO, 1);
    pStrip_a->clear(pStrip_a, 50);
    ESP_LOGI(ourTaskName, "Starting UP...\n");
    printf("this is prinf() function!\n\n");
    pStrip_a->set_pixel(pStrip_a, 0, 0, 6, 6);
    pStrip_a->refresh(pStrip_a, 100);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    pStrip_a->clear(pStrip_a, 50);
    while (1){
        printf("%c", start);
        pStrip_a->set_pixel(pStrip_a, 0, 0, 6, 0);
        pStrip_a->refresh(pStrip_a, 100);
        for (int j = 0; j < 64; j++){
            int value = (random()%99)+1;
            printf("%c",value);
        }
        pStrip_a->clear(pStrip_a, 50);
        vTaskDelay(256 / portTICK_PERIOD_MS);
    }
}

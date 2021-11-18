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

int channel[16][4] = {
    {0,0,0,0},
    {0,0,0,1},
    {0,0,1,0},
    {0,0,1,1},
    {0,1,0,0},
    {0,1,0,1},
    {0,1,1,0},
    {0,1,1,1},
    {1,0,0,0},
    {1,0,0,1},
    {1,0,1,0},
    {1,0,1,1},
    {1,1,0,0},
    {1,1,0,1},
    {1,1,1,0},
    {1,1,1,1},
};

void init_mux(int S0, int S1, int S2, int S3, int EN){
    gpio_reset_pin(S0);
    gpio_reset_pin(S1);
    gpio_reset_pin(S2);
    gpio_reset_pin(S3);
    gpio_reset_pin(EN);
    gpio_set_direction(S0,GPIO_MODE_OUTPUT);
    gpio_set_direction(S1,GPIO_MODE_OUTPUT);
    gpio_set_direction(S2,GPIO_MODE_OUTPUT);
    gpio_set_direction(S3,GPIO_MODE_OUTPUT);
    gpio_set_direction(EN,GPIO_MODE_OUTPUT);
    gpio_set_level(EN, 1);
}

void select_mux(int S0, int S1, int S2, int S3, int EN, int select_channel){
    gpio_set_level(EN, 0);
    gpio_set_level(S0,channel[select_channel][3]);
    gpio_set_level(S1,channel[select_channel][2]);
    gpio_set_level(S2,channel[select_channel][1]);
    gpio_set_level(S3,channel[select_channel][0]);
}

void app_main(void){
    printf("starting up...\n");
    gpio_reset_pin(4);
    gpio_set_direction(9,GPIO_MODE_OUTPUT);
    gpio_set_level(4,0);
    init_mux(18, 17, 16, 15, 8);
    //gpio_set_level(14,0);
    printf("init complete.\n");
    vTaskDelay(4000 / portTICK_PERIOD_MS);
    while (1){
    for(int i = 0; i<16; i++){
        printf("now set level at pin C%d as high\n",i);
        select_mux(18, 17, 16, 15, 8, i);
        gpio_set_level(4,1);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        gpio_set_level(4,0);
    }}
    printf("Finish Program...\n");
}
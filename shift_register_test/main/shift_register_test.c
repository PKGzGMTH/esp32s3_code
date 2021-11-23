#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led_strip.h"

#define ser 42       // data in
#define srclk 41     // shift register clock
#define rclk 40      // latch clock
#define srclr 39     // shift register reset

int ShiftRegisterPin[4] = {ser, srclk, rclk, srclr};

void init_sr(int pin[4]){
    gpio_reset_pin(pin[0]);
    gpio_reset_pin(pin[1]);
    gpio_reset_pin(pin[2]);
    gpio_reset_pin(pin[3]);

    gpio_set_direction(pin[0],GPIO_MODE_OUTPUT);
    gpio_set_direction(pin[1],GPIO_MODE_OUTPUT);
    gpio_set_direction(pin[2],GPIO_MODE_OUTPUT);
    gpio_set_direction(pin[3],GPIO_MODE_OUTPUT);

    gpio_set_level(pin[0], 0);
    gpio_set_level(pin[1], 0);
    gpio_set_level(pin[2], 0);
    gpio_set_level(pin[3], 0);

    //reset
    gpio_set_level(pin[3], 0);
    gpio_set_level(pin[3], 1);
}

void app_main(void)
{
    // init shift register
    init_sr(ShiftRegisterPin);
    while (1){
        //send high
        gpio_set_level(ser, 1);

        //start clock storage in shift register
        gpio_set_level(srclk, 1);
        gpio_set_level(srclk, 0);

        //send low
        gpio_set_level(ser, 0);
        for (int i = 0; i < 16; i++){
            //start clock output
            gpio_set_level(rclk, 1);
            gpio_set_level(rclk, 0);

            printf("send data in pin %d\n", i);

            //shift data
            gpio_set_level(srclk, 1);
            gpio_set_level(srclk, 0);
        }
        //reset
        gpio_set_level(srclr, 0);
        gpio_set_level(srclr, 1);
    }
}
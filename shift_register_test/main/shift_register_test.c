#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led_strip.h"

#define ser 4       // data in
#define srclk 5     // shift register clock
#define rclk 6      // latch clock
#define srclr 7     // shift register reset

int ShiftRegisterPin[4] = {4, 5, 6, 7};

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
    //enable output
    gpio_set_level(pin[3], 1);
}

void app_main(void)
{
    while (1){
        init_sr(ShiftRegisterPin);
        //send high
        gpio_set_level(ser, 1);

        //start clock storage in shift register
        gpio_set_level(srclk, 1);
        gpio_set_level(srclk, 0);

        //send low
        gpio_set_level(ser, 0);
        for (int i = 0; i < 8; i++){
            //start clock output
            gpio_set_level(rclk, 1);
            gpio_set_level(rclk, 0);

            printf("send data in pin %d\n", i);

            //shift data
            gpio_set_level(srclk, 1);
            gpio_set_level(srclk, 0);
            vTaskDelay(1000/ portTICK_PERIOD_MS);
        }
    }

}

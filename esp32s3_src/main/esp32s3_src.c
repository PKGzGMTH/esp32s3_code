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
#include "driver/adc_common.h"
#include "led_strip.h"

#define NeoPixel_GPIO 48

#define ser 42       // data in
#define srclk 41     // shift register clock
#define rclk 40      // latch clock
#define srclr 39     // shift register reset

static led_strip_t *NeoPixel;
uint8_t R,G,B;

int value, start = 0;
int *valueADC2 = &value;
int ShiftRegisterPin[4] = {ser, srclk, rclk, srclr};
int adc_pin[16] = {4, 5, 6, 7, 15, 16, 17, 18, 8, 1, 2, 9, 10, 11, 12, 13};

void init_sr(int pin[4])
{
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

void readAnalog(void)
{
    printf("%c", value = (adc1_get_raw(3) * 254 / 4095) + 1);
    printf("%c", value = (adc1_get_raw(4) * 254 / 4095) + 1);
    printf("%c", value = (adc1_get_raw(5) * 254 / 4095) + 1);
    printf("%c", value = (adc1_get_raw(6) * 254 / 4095) + 1);

    adc2_get_raw(4, ADC_WIDTH_BIT_12, valueADC2);
    printf("%c", (value * 254 / 4095) + 1);
    adc2_get_raw(5, ADC_WIDTH_BIT_12, valueADC2);
    printf("%c", (value * 254 / 4095) + 1);
    adc2_get_raw(6, ADC_WIDTH_BIT_12, valueADC2);
    printf("%c", (value * 254 / 4095) + 1);
    adc2_get_raw(7, ADC_WIDTH_BIT_12, valueADC2);
    printf("%c", (value * 254 / 4095) + 1);

    printf("%c", value = (adc1_get_raw(7) * 254 / 4095) + 1);
    printf("%c", value = (adc1_get_raw(0) * 254 / 4095) + 1);
    printf("%c", value = (adc1_get_raw(1) * 254 / 4095) + 1);

    adc2_get_raw(8, ADC_WIDTH_BIT_12, valueADC2);
    printf("%c", (value * 254 / 4095) + 1);
    adc2_get_raw(9, ADC_WIDTH_BIT_12, valueADC2);
    printf("%c", (value * 254 / 4095) + 1);
    adc2_get_raw(0, ADC_WIDTH_BIT_12, valueADC2);
    printf("%c", (value * 254 / 4095) + 1);
    adc2_get_raw(1, ADC_WIDTH_BIT_12, valueADC2);
    printf("%c", (value * 254 / 4095) + 1);
    adc2_get_raw(2, ADC_WIDTH_BIT_12, valueADC2);
    printf("%c", (value * 254 / 4095) + 1);
}

void init_ADC(int pin[16])
{
    for (int i = 0; i < 16; i++){
        gpio_reset_pin(pin[i]);
        gpio_pulldown_en(pin[i]);
    }

    for (int i = 0; i < 10; i++){
        adc1_config_channel_atten(i, ADC_ATTEN_DB_11);
        adc2_config_channel_atten(i, ADC_ATTEN_DB_11);
    }
    adc1_config_width(ADC_WIDTH_BIT_12);

}

void app_main(void)
{
    //init Analog to Digital Converter (ADC)
    init_ADC(adc_pin);

    //init shift register
    init_sr(ShiftRegisterPin);

    NeoPixel = led_strip_init(0, NeoPixel_GPIO, 1);
    NeoPixel -> clear(NeoPixel, 50);
    
    while (1){
        printf("%c", start);
        //set_pixel(NeoPixel, location, R, G, B);
        NeoPixel -> set_pixel(NeoPixel, 0, 0, 4, 1);
        NeoPixel -> refresh(NeoPixel, 100);

        //send high
        gpio_set_level(ser, 1);

        //start clock storage in shift register
        gpio_set_level(srclk, 1);
        gpio_set_level(srclk, 0);

        //send low
        gpio_set_level(ser, 0);

        //send output channel 0 -> 16
        for (int i = 0; i < 16; i++){
            //start clock output
            gpio_set_level(rclk, 1);
            gpio_set_level(rclk, 0);
            
            //read analog and send data in string [0-255]
            readAnalog();

            //shift data
            gpio_set_level(srclk, 1);
            gpio_set_level(srclk, 0);
            vTaskDelay(32 / portTICK_PERIOD_MS);
        }

        NeoPixel -> clear(NeoPixel, 50);
        //reset
        gpio_set_level(srclr, 0);
        gpio_set_level(srclr, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);

    }
}
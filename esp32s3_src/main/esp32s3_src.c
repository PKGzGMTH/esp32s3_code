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

#define S0 13
#define S1 12
#define S2 11
#define S3 10
#define EN 14

static led_strip_t *NeoPixel;
uint8_t R,G,B;

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

int value, start = 0;
int *valueADC2 = &value;
int ShiftRegisterPin[4] = {ser, srclk, rclk, srclr};
int AnalogMuxPin[5] = {S0, S1, S2, S3, EN};
int size = 200, startValue = 11;
char data[32][32];

void init_mux(int pin[5]) {
    gpio_reset_pin(pin[0]);
    gpio_reset_pin(pin[1]);
    gpio_reset_pin(pin[2]);
    gpio_reset_pin(pin[3]);
    gpio_reset_pin(pin[4]);
    gpio_set_direction(pin[0],GPIO_MODE_OUTPUT);
    gpio_set_direction(pin[1],GPIO_MODE_OUTPUT);
    gpio_set_direction(pin[2],GPIO_MODE_OUTPUT);
    gpio_set_direction(pin[3],GPIO_MODE_OUTPUT);
    gpio_set_direction(pin[4],GPIO_MODE_OUTPUT);
    gpio_set_level(pin[4], 1);
}

void select_mux(int pin[5], int select_channel) {
    gpio_set_level(pin[4], 0);
    gpio_set_level(pin[0],channel[select_channel][3]);
    gpio_set_level(pin[1],channel[select_channel][2]);
    gpio_set_level(pin[2],channel[select_channel][1]);
    gpio_set_level(pin[3],channel[select_channel][0]);
}

void init_sr(int pin[4]) {
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

void init_ADC(void) {
    gpio_reset_pin(4);
    gpio_reset_pin(5);
    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_set_direction(5, GPIO_MODE_INPUT);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(3, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(4, ADC_ATTEN_DB_11);
}

void app_main(void) {
    //init Analog Mutiplexer
    init_mux(AnalogMuxPin);

    //init Analog to Digital Converter (ADC)
    init_ADC();

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

        //read analog pressure value
        for (int x = 15; x >= 0; x--){
            //start clock output
            gpio_set_level(rclk, 1);
            gpio_set_level(rclk, 0);
            
            //read analog and save in data[32][32] in string [11-200]
            for (int y = 0; y < 16; y++){
                select_mux(AnalogMuxPin, y); 
                data[x][y] = ((adc1_get_raw(3) * size / 4095) + startValue);
                data[x+16][y+16] = ((adc1_get_raw(4) * size / 4095) + startValue);
            }

            //shift data
            gpio_set_level(srclk, 1);
            gpio_set_level(srclk, 0);

            //delay
            //vTaskDelay(32 / portTICK_PERIOD_MS);
        }

        //print value
        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 32; x++) {
                printf("%c", data[x][y]);
            }
        }

        //print \n to send all char data
        printf("\n");

        //clear RGB Neo-pixel
        NeoPixel -> clear(NeoPixel, 50);

        //reset
        gpio_set_level(srclr, 0);
        gpio_set_level(srclr, 1);
        vTaskDelay(360 / portTICK_PERIOD_MS);
    }
}
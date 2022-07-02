#include "LED_W2812.h"

TIM_HandleTypeDef* LED_htim;

uint8_t LED_numberOfLed;
uint8_t LED_frequency;
uint8_t LED_mode;

void LED_init(TIM_HandleTypeDef* htim, int numberOfLed);
void LED_setColor(int index, uint8_t brightness, uint8_t R, uint8_t G, uint8_t B);
void LED_setMode(uint8_t LED_MODE);
void LED_setFrequency();

void LED_process();
void LED_loop();

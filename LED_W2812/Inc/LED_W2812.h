/*
 * LED_W2812.h
 *
 *  Created on: Jul 2, 2022
 *      Author: yp7
 */

#ifndef LED_W2812_INC_LED_W2812_H_
#define LED_W2812_INC_LED_W2812_H_

#include "main.h"
#include "stdint.h"

enum
{
	LED_MODE_OFF,
	LED_MODE_STATIC,
	LED_MODE_BREATHING,
	LED_MODE_RAINBOW
};

void LED_init(TIM_HandleTypeDef* htim, int numberOfLed);
void LED_setColor(int index, uint8_t brightness, uint8_t R, uint8_t G, uint8_t B);
void LED_setMode(uint8_t LED_MODE);
void LED_setFrequency();

void LED_process();
void LED_loop();

#endif /* LED_W2812_INC_LED_W2812_H_ */

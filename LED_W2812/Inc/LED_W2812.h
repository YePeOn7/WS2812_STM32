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
	LED_MODE_RAINBOW,
	LED_MODE_BLINKING
};

extern uint16_t LED_numberOfData;
extern uint16_t LED_pwmData[66];
extern uint8_t LED_data[1][3];				//use for saving user input
extern uint8_t LED_processOutput[1][3];
//extern uint16_t* LED_pwmData;
//extern uint8_t** LED_data;				//use for saving user input
//extern uint8_t** LED_processOutput;	//use for saving processed data

extern uint32_t delTime;
extern int blinkingState;

void LED_init(TIM_HandleTypeDef* htim, uint32_t pwmChannel, int numberOfLed);
void LED_setColor(int index, uint8_t brightness, uint8_t R, uint8_t G, uint8_t B);
void LED_setMode(uint8_t LED_MODE);
void LED_setPeriode(int periode);

void LED_process();
void LED_loop();

#endif /* LED_W2812_INC_LED_W2812_H_ */

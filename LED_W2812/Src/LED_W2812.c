#include "LED_W2812.h"
#include "stdlib.h"
#include "math.h"

#define getTick 	HAL_GetTick
#define PI 			3.141592653589793

TIM_HandleTypeDef* LED_htim;

uint8_t LED_numberOfLed;
uint8_t LED_periode;
uint8_t LED_brigtness;

uint8_t LED_mode;
uint8_t LED_modePrevious;
uint32_t LED_modeStartTime;

uint16_t LED_numberOfData;
//uint16_t LED_pwmData[53];
//uint8_t LED_data[1][3];				//use for saving user input
//uint8_t LED_processOutput[1][3];
uint16_t* LED_pwmData;
uint8_t** LED_data;				//use for saving user input
uint8_t** LED_processOutput;	//use for saving processed data

void LED_init(TIM_HandleTypeDef* htim, uint32_t pwmChannel, int numberOfLed)
{
	LED_htim = htim;
	LED_numberOfLed = numberOfLed;
	LED_numberOfData = numberOfLed * 24 + 42;

	LED_pwmData = (uint16_t*) malloc(LED_numberOfData * sizeof(uint16_t));
	for(int i = 0; i < LED_numberOfData; i++)
	{
		LED_pwmData[i] = 0;
	}

	LED_data = (uint8_t**) malloc(numberOfLed * sizeof(uint8_t*));
	LED_processOutput = (uint8_t**) malloc(numberOfLed * sizeof(uint8_t*));
	for(int i = 0; i < numberOfLed; i++)
	{
		LED_data[i] = malloc(3 * sizeof(uint8_t));
		LED_processOutput[i] = malloc(3 * sizeof(uint8_t));
        for(int j =0; j<3; j++)
        {
            LED_data[i][j] = 0;
            LED_processOutput[i][j] = 0;
        }
	}

	HAL_TIM_PWM_Start_DMA(LED_htim, pwmChannel, (uint32_t*)LED_pwmData, LED_numberOfData);
}

void LED_setColor(int index, uint8_t brightness, uint8_t R, uint8_t G, uint8_t B)
{
	LED_brigtness = brightness;
	LED_data[index][0] = R;
	LED_data[index][1] = G;
	LED_data[index][2] = B;
}

void LED_setMode(uint8_t LED_MODE)
{
	LED_mode = LED_MODE;
	if(LED_mode != LED_modePrevious)
	{
		LED_modeStartTime = getTick();
	}
	LED_modePrevious = LED_mode;
}

void LED_setPeriode(int periode)
{
	LED_periode = periode;
}

void LED_process()
{
	int blinkingState;
	uint32_t t;
	switch(LED_mode)
	{
		case LED_MODE_OFF:
			for(int i =0; i < LED_numberOfLed; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					LED_processOutput[i][j] = 0;
				}
			}
			break;

		case LED_MODE_STATIC:
			for(int i =0; i < LED_numberOfLed; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					LED_processOutput[i][j] = LED_data[i][j] * LED_brigtness / 255;
				}
			}
			break;

		case LED_MODE_BLINKING:


			if((getTick() - LED_modeStartTime) > LED_periode * 1000 / 2) blinkingState = 0;
			else blinkingState = 1;

			for(int i =0; i < LED_numberOfLed; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					LED_processOutput[i][j] = LED_data[i][j] * blinkingState;
				}
			}
			break;

		case LED_MODE_BREATHING:
			t = getTick() - LED_modeStartTime;
			for(int i =0; i < LED_numberOfLed; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					LED_processOutput[i][j] = LED_data[i][j] * (1 + cosf(2 * PI * t / LED_periode)) / 2;
				}
			}
			break;

		case LED_MODE_RAINBOW:
			t = getTick() - LED_modeStartTime;
			for(int i =0; i < LED_numberOfLed; i++)
			{

				LED_processOutput[i][0] = LED_brigtness * (1 + sinf(2 * PI * t / LED_periode)) / 2;
				LED_processOutput[i][1] = LED_brigtness * (1 + sinf(2 * PI * t / LED_periode) + PI * 2 / 3) / 2;
				LED_processOutput[i][2] = LED_brigtness * (1 + sinf(2 * PI * t / LED_periode) - PI * 2 / 3) / 2;
			}
			break;
	}
}

void LED_loop()
{
	uint32_t color;

	LED_process();

	for(int i = 0; i < LED_numberOfLed; i++)
	{
		//the order is GRB (Based on datasheet
		color = LED_processOutput[i][1] << 16 | LED_processOutput[i][0] << 8 | LED_processOutput[i][2];

		for(int j = 0; j < 24; j++)
		{
			if(color >> (23-j) & 1) LED_pwmData[i * 24 + j] = 2 * LED_htim->Init.Period / 3;
			else LED_pwmData[i * 24 + j] = LED_htim->Init.Period / 3;
		}
	}
}

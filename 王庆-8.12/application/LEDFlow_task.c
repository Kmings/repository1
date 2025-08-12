/**
  *********************************(C) 2025 bayek******************************
  * @file       LEDFlow_task.c/h
  * @brief      LED流水灯显示任务
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Aug-05-2025     bayek           1. done
  *
  @verbatim
  ==============================================================================
  *             TODO
  ==============================================================================
  @endverbatim
  */

#include "LEDFlow_task.h"
#include "string.h"
#include "cmsis_os.h"
#include "main.h"

#include "bsp_LED.h"

void LEDFlowFunc()
{
	static int timesss=0;
	while(1)
	{
		timesss++;
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		osDelay(100);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		osDelay(100);
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		osDelay(100);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		osDelay(100);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		osDelay(100);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
		osDelay(100);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		osDelay(100);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
		osDelay(100);
	}
}

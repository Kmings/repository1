#ifndef _CYLINDER_H
#define _CYLINDER_H

//#include "stm32f4xx.h"
//#include "stm32f4xx_hal_gpio.h"
//#include "mathfunc.h"
//// #define Set_CYL8CLK() 		GPIO_SetBits(GPIOA, GPIO_Pin_1)
//// #define Reset_CYL8CLK() 	GPIO_ResetBits(GPIOA, GPIO_Pin_1)
//// #define Set_CYLData() 		GPIO_SetBits(GPIOA, GPIO_Pin_0)
//// #define Reset_CYLData() 	GPIO_ResetBits(GPIOA, GPIO_Pin_0)

//#define Set_CYL8CLK()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
//#define Reset_CYL8CLK()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
//#define Set_CYLData()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
//#define Reset_CYLData() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

//typedef struct
//{
//    void (*CYL_Ctrl)(u8 IO, u8 status);

//} CyLinderStruct;

//void CYL_Configuration(void);
//void CYL_Ctrl(u8 IO, u8 status);
//void Valve_Ctrl(u8 Data);

//extern CyLinderStruct Cyl;

//#define Set_CYLDATA() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
//#define Reset_CYLDATA() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
//#define Set_CYLCLK()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
//#define Reset_CYLCLK()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

//#endif




#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "mathfunc.h"



extern int cylinderenable;

void CLK_set(void);
void reset_SI(void);
void set_SI(void);
void CLK_reset(void);

void ctrl(uint8_t cmd);

#endif

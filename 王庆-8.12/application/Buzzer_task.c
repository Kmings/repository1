/**
  *********************************(C) 2025 bayek******************************
  * @file       Buzzer_task.c/h
  * @brief      ��Դ��������������
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

#include "Buzzer_task.h"
#include "string.h"
#include "cmsis_os.h"
#include "main.h"
#include "bsp_buzzer.h"
#include <stdio.h>
#include "EventRecorder.h"

void buzzerFunc()
{
	timess = 2;
	while(1)
	{
		if(timess)
		{
			EventStartA(0);
			printf("���������%d��\r\n", timess);
			buzzer_play(3000,100);
			timess--;
		}
		osDelay(100);
		EventStopA(0);
//		printf("hello");
		
	}
}

#include "bsp_buzzer.h"
#include "main.h"



extern TIM_HandleTypeDef htim1;
void buzzer_TIM_init()
{
		//start tim
    //������ʱ��
    HAL_TIM_Base_Start(&htim1);
    //start pwm channel
    //����PWMͨ��
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}
void buzzer_play(uint16_t freq_hz, uint32_t duration_ms)
{
    // 1. ������Ч�Լ��
    if(freq_hz == 0 || duration_ms == 0) {
        buzzer_off();
        return;
    }
    
    // 2. ��ȡ��ʱ��ʵ��ʱ��Ƶ��
    uint32_t timer_clock = HAL_RCC_GetPCLK1Freq(); // ����ʵ�ʶ�ʱ��ʱ��Դ����
    uint16_t prescaler = htim1.Init.Prescaler + 1;
    uint32_t timer_freq = timer_clock / prescaler;
    
    // 3. ����ARRֵ (��ֹ����)
    uint32_t arr_val = (timer_freq / freq_hz) - 1;
    if(arr_val > UINT16_MAX || arr_val == 0) {
        // Ƶ�ʳ�����Χ����
        buzzer_off();
        return;
    }
    
    // 4. ����PWM (50%ռ�ձ�)
    uint16_t compare_val = (uint16_t)(arr_val / 2);
    
    // 5. ��ȫ���üĴ���
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SET_AUTORELOAD(&htim1, (uint16_t)arr_val);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, compare_val);
    __HAL_TIM_SET_COUNTER(&htim1, 0);

    // 6. ����PWM
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    
    // 7. ��������ʱ���������״̬����
    // ʵ����Ŀ��Ӧʹ��RTOS�����ʱ���ص�
    // �˴���ʵ��
    HAL_Delay(duration_ms);
    buzzer_off();
}
void buzzer_off(void)
{
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
}

#include "bsp_buzzer.h"
#include "main.h"



extern TIM_HandleTypeDef htim1;
void buzzer_TIM_init()
{
		//start tim
    //开启定时器
    HAL_TIM_Base_Start(&htim1);
    //start pwm channel
    //开启PWM通道
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}
void buzzer_play(uint16_t freq_hz, uint32_t duration_ms)
{
    // 1. 参数有效性检查
    if(freq_hz == 0 || duration_ms == 0) {
        buzzer_off();
        return;
    }
    
    // 2. 获取定时器实际时钟频率
    uint32_t timer_clock = HAL_RCC_GetPCLK1Freq(); // 根据实际定时器时钟源调整
    uint16_t prescaler = htim1.Init.Prescaler + 1;
    uint32_t timer_freq = timer_clock / prescaler;
    
    // 3. 计算ARR值 (防止除零)
    uint32_t arr_val = (timer_freq / freq_hz) - 1;
    if(arr_val > UINT16_MAX || arr_val == 0) {
        // 频率超出范围处理
        buzzer_off();
        return;
    }
    
    // 4. 配置PWM (50%占空比)
    uint16_t compare_val = (uint16_t)(arr_val / 2);
    
    // 5. 安全设置寄存器
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SET_AUTORELOAD(&htim1, (uint16_t)arr_val);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, compare_val);
    __HAL_TIM_SET_COUNTER(&htim1, 0);

    // 6. 启动PWM
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    
    // 7. 非阻塞延时处理（需配合状态机）
    // 实际项目中应使用RTOS任务或定时器回调
    // 此处简化实现
    HAL_Delay(duration_ms);
    buzzer_off();
}
void buzzer_off(void)
{
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
}

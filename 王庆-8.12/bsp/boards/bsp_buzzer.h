#ifndef BSP_BUZZER_H
#define BSP_BUZZER_H

#include "struct_typedef.h"

extern void buzzer_play(uint16_t freq_hz, uint32_t duration_ms);
extern void buzzer_off(void);
extern void buzzer_TIM_init(void);

#endif

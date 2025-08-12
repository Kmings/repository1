#ifndef BSP_LED_H
#define BSP_LED_H
#include "struct_typedef.h"

#define RGB_FLOW_COLOR_CHANGE_TIME  500
#define RGB_FLOW_COLOR_LENGHT   3

/**
  * @brief          ��ʾRGB
  * @param[in]      aRGB:0xaaRRGGBB,'aa' ��͸����,'RR'�Ǻ�ɫ,'GG'����ɫ,'BB'����ɫ
  * @retval         none
  */
extern void led_show(int i, int tim);

#endif

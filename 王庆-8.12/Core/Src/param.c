#include "param.h"

FlagType Flag;
BeepStructType Beep;
LEDStructType Led;
Usart_StructType usart;

void param_Init(void)
{
    Led.Normal = true;
    Led.Stuck = false;
    Led.Timeout = false;
    Beep.error = false;
    Beep.error_cnt = 0;

    // 自己的协议 提前将帧头和帧尾设置好

}

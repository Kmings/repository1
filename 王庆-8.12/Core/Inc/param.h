#ifndef _PARAM_H
#define _PARAM_H
#include "stm32f4xx.h"
#include "stdbool.h"
#include "DJmotor.h"

#define USART1_TX_BUFFER_SIZE 256
#define USART1_RX_BUFFER_SIZE 32
#define USART2_TX_BUFFER_SIZE 256
#define USART2_RX_BUFFER_SIZE 32
#define USART3_TX_BUFFER_SIZE 50 // 蓝牙数据包
#define USART3_RX_BUFFER_SIZE 50

typedef struct
{
    uint8_t Tx_Buffer_USART1[USART1_TX_BUFFER_SIZE]; // 串口1发送数据数组
    uint8_t Rx_Buffer_USART1[USART1_RX_BUFFER_SIZE]; // 串口1接收数据

    uint8_t Tx_Buffer_USART2[USART2_TX_BUFFER_SIZE];
    uint8_t Rx_Buffer_USART2[USART2_RX_BUFFER_SIZE];

    uint8_t Tx_Buffer_USART3[USART3_TX_BUFFER_SIZE];
    uint8_t Rx_Biffer_USART3[USART3_RX_BUFFER_SIZE];
} Usart_StructType;

typedef struct
{
    volatile bool FireringFlag; // 发射标志位
    volatile bool FireSpeedCheckFlag;
    volatile bool FireSpeedArrviedFlag;
    volatile bool TakeRingFlag;     // 取环标志位
    volatile bool TakeRingOverFlag; // 取环完成标志位 视情况看是否需要
    volatile bool ScopeStartFlag; // 示波器开启标志位
    volatile bool BTStartFlag; // 蓝牙串口发送标志位

    volatile bool CAN1_ControlList_Enable;
    volatile bool CAN2_ControlList_Enable;
    volatile bool CAN1SendQueueFULL;
    volatile bool CAN2SendQueueFULL;
    volatile bool VESCSendQueueFULL;
    volatile bool CANSendQueueEMPTY;
    volatile bool Usart1DMA_SendFinish;
    volatile bool Usart2DMA_SendFinish;
    volatile bool Usart3DMA_SendFinish;
    // TODO: 根据实际需要添加标志位
} FlagType;

typedef struct
{
    volatile bool AllowFlag;
    uint16_t BeepOnNum;
    volatile bool error;
    uint8_t error_cnt;
} BeepStructType;

typedef struct
{
    volatile bool Normal;
    volatile bool Stuck;
    volatile bool CANErr;
    volatile bool Timeout;
    volatile bool CloseAll;
} LEDStructType;

extern FlagType Flag;
extern BeepStructType Beep;
extern LEDStructType Led;
extern Usart_StructType usart;

void param_Init(void);

#endif

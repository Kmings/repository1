#ifndef _ZDRIVE_H
#define _ZDRIVE_H

#include <stdbool.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal_can.h"

#include "pid.h"
#include "myqueue.h"
#include "param.h"
#include "mathfunc.h"

// mode
typedef enum
{
    Zdrive_Disable,                  // 0失能
    Zdrive_Current,                  // 1电流模式
    Zdrive_Speed,                    // 2速度模式
    Zdrive_Postion,                  // 3位置模式
    Zdrive_Test,                     // 4测试模式
    Zdrive_RVCalibration,            // 5电阻电感校准
    Zdrive_EncoderLineCalibration,   // 6编码器线性补偿
    Zdrive_EncoudeOffsetCalibration, // 7编码器偏移校准
    Zdrive_VKCalibration,            // 8VK校准
    Zdrive_SaveSetting,              // 9保存配置
    Zdrive_EraseSetting,             // 10擦除配置
    Zdrive_ClearErr,                 // 11擦除错误
    Zdrive_Brake                     // 12刹车
} ZdriveMode;

// error
typedef enum
{
    Zdrive_Well,                // 0无
    Zdrive_InsufficientVoltage, // 1低电压
    Zdrive_OverVoltage,         // 2过电压
    Zdrive_InstabilityCurrent,  // 3电流不稳
    Zdrive_OverCurrent,         // 4过电流
    Zdrive_OverSpeed,           // 5超速
    Zdrive_ExcessiveR,          // 6电阻过大
    Zdrive_ExcessiveInductence, // 7电感过大
    Zdrive_LoseEncoder1,        // 8编码器错误
    Zdrive_PolesErr,            // 9极对数不匹配
    Zdrive_VKCalibrationErr,    // 10 KV校准失败
    Zdrive_ModeErr,             // 11模式不合法
    Zdrive_ParameterErr,        // 12参数错误
    Zdrive_Hot                  // 13过热
} ZdriveErr;

typedef struct
{
    float current;
    float speed;
    float angle;
	float pin;
} ZdriveValue;

typedef struct
{
    uint32_t lastRxTime;
    uint32_t timeoutTicks;
    uint32_t stuckCnt;
    float lockAngle;
} ZdriveArgum;

typedef struct
{
    float gearRatio;
    float reductionRatio;
} ZdriveParam;

typedef struct
{
    bool timeout;
    bool stuck;
    ZdriveErr err;
} ZdriveStatus;

typedef struct
{
    bool PVTModeFlag;
    u8 PVTStage, PVTArrivedCnt;
    float reachTime[3];
    float targetSpeed[3];
    float targetAngle[3];
} ZdrivePVTMode;

typedef struct
{
    bool enable;
    bool begin;
    bool brake;
    bool errClearFlag;
    ZdriveMode mode, modeRead;
    ZdriveParam param;
    ZdriveArgum argum;
    ZdriveValue valueRealLast, valueSetLast, valueRealNow, valueSetNow;
    ZdriveStatus status;
    ZdrivePVTMode PVTMode;
} Zdrive;

void zdriveInit(void);

void zdriveSetMode(float mode, u8 id);
void zdriveSetSpeed(float speed, u8 id);
void zdriveSetPosition(float angle_dgree, u8 id);
void zdriveSetPresentPosition(float angle_degree, u8 id); 
void ZdriveSavePresentPos(u8 id);

void zdriveReceiveHandler(CAN_RxHeaderTypeDef rx_msg, uint8_t *RXmessage);
void zdriveFunc(void);

extern Zdrive Zmotor[4];
extern int Poscnt;
#endif

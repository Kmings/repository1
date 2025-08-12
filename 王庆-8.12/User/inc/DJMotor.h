#ifndef _DJMOTOR_H
#define _DJMOTOR_H
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "stm32f4xx_hal_can.h"
#include "pid.h"
#include "mathfunc.h"
#include "can.h"
#include "param.h"

#define USE_M2006_NUM 4
#define USE_M3508_NUM 0
#define USE_DJ_NUM (USE_M2006_NUM + USE_M3508_NUM)

#define M2006_RATIO 36
#define M3508_RATIO 19.20320855f

#define ZERO_DISTANCE_THRESHOLD 15
#define SETLOCK_MAXDIS_THRESHOLD 10

enum MotorMode
{
    DJ_RPM,      // 速度模式
    DJ_POSITION, // 位置模式
    DJ_ZERO,     // 寻零模式
    DJ_CURRENT   // 力矩模式
};

typedef struct
{
    uint16_t pulsePerRound; // 电机每转一圈的脉冲数
    float reductionRatio;   // 减速比
    float GearRatio;        // 齿轮传动比
    uint32_t StdID;         // ID号
    uint32_t IDE;           // 标准帧or扩展帧
    uint32_t RTR;           // 数据帧or广播帧
    uint8_t DLC;            // 数据长度
    int16_t currentLimit;   // 电流限制，写在这里因为2006和3508的范围不同
} DJMotorParam;

typedef struct
{
    volatile int16_t current;       // 电流大小
    volatile float angle;           // 电机角度大小，°位单位
    volatile int16_t speed;         // 电机转速，单位rpm
    volatile float currentA;        // 解算后的电流大小
    volatile float realSpeed;       // 电机真实速度
    volatile int16_t pulseRead;     // 电调读取的脉冲数
    volatile int16_t pulseDistanse; // 当前脉冲与上次脉冲数之差
    volatile int32_t pulseTotal;    // 累计脉冲数
    volatile int8_t temperature;    // 大疆C620反馈温度，C610不会反馈
} DJMotorValue;

typedef struct
{
		volatile int32_t pulseLock;
		volatile uint32_t lastRxTime;
    uint16_t zeroCnt; // 寻零计数器
    uint16_t pawLockCnt;
    uint32_t maxDistance;
    uint16_t counter;
} DJMotorArgum;

typedef struct
{
		bool RPMLimitFlag;
		bool posAngleLimitFlag; // 位置模式下的角度限制
		bool posRPMFlag;        // 位置模式下的速度环pid计算限制标志位
    bool currentLimitFlag;  // 电流限制
    // 数值限制
    float maxAngle; // 最大角度
    float minAngle; // 最小角度

    int32_t posRPMLimit; // 位置模式下的最大限制角度，这里实际上是队pid计算的中间量进行限制
    int16_t zeroSpeed;   // 寻零最大速度限制
    int16_t zeroCurrent; // 寻零最大电流
    bool isLooseStuck;   // 是否堵转
} DJMotorLimit;	

typedef struct
{
    bool releaseWhenStuckFlag; // 判断堵转后是否释放
    bool StuckDetectFlag;      // 堵转监控
    bool TimeoutDetectFlag;    // 超时监控

    uint16_t stuckCount;   // 堵转计数
    uint16_t timeoutCount; // 超时计数
} DJMotorError;   //电机异常反应

// 电机状态执行情况
typedef struct
{
	volatile bool arrivedFlag;
	volatile bool zeroFlag;
	volatile bool overtimeFlag;
	volatile bool stuckFlag;
	volatile bool isSetZero;
} DJMotorStatus;


typedef struct
{
	uint8_t ID;
	volatile bool enable;
	volatile bool begin;
	uint8_t mode;
	DJMotorParam param;
	DJMotorValue valueSet, valueNow, valuePre;
	DJMotorArgum argum;
	DJMotorLimit limit;
	DJMotorError error;
	DJMotorStatus statusFlag;	
	
	PIDType posPID, rpmPID;
} DJMotor, *DJMotorPointer;

extern DJMotor DJmotor[USE_DJ_NUM];   //extern

//电机初始化
void DJMotorInit(void);
//电机位置置零
void DJSetZero(DJMotorPointer motor);
//接收电调反馈
void DJReceiveData_Can2(CAN_RxHeaderTypeDef Rxmsg0,uint8_t *data);
//电机角度解算
void DJAngleCulculate(DJMotorPointer motor);
//锁点
void DJLockPosition(DJMotorPointer motor);
//速度模式
void DJSpeedMode(DJMotorPointer motor);
//位置模式
void DJPositionMode(DJMotorPointer motor);
//寻零模式
void DJZeroMode(DJMotorPointer motor);
//发送控制电流
void DJCurrentTransmit(DJMotorPointer motor, uint8_t id);
//电机状态监测
void motorMonitor(DJMotorPointer motor);
//总结，放在定时器中
void DJFunc(void);

#endif

#ifndef _VESC_H
#define _VESC_H

#include "myqueue.h"
#include "mathFunc.h"
#include "pid.h"
#include <string.h>


#define USE_VESC_NUM 8

enum vescMoterMode
{
	VESC_RPM = 0,
	VESC_POSITION,
	VESC_CURRENT,
	VESC_DUTY,
	VESC_HANDBRAKE
};

enum canPacketID // 大概只用前四个吧
{
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,		  // 1
	CAN_PACKET_SET_CURRENT_BRAKE, // 2
	CAN_PACKET_SET_RPM,			  // 3
	CAN_PACKET_SET_POS,			  // 4
	CAN_PACKET_FILL_RX_BUFFER,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,
	CAN_PACKET_STATUS,
	CAN_PACKET_SET_CURRENT_REL,
	CAN_PACKET_SET_CURRENT_BRAKE_REL,
	CAN_PACKET_SET_CURRENT_HANDBRAKE,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
	CAN_PACKET_STATUS_2,
	CAN_PACKET_STATUS_3,
	CAN_PACKET_STATUS_4,
	CAN_PACKET_PING,
	CAN_PACKET_PONG,
	CAN_PACKET_DETECT_APPLY_ALL_FOC,
	CAN_PACKET_DETECT_APPLY_ALL_FOC_RES,
	CAN_PACKET_CONF_CURRENT_LIMITS,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS,
	CAN_PACKET_CONF_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_FOC_ERPMS,
	CAN_PACKET_CONF_STORE_FOC_ERPMS,
	CAN_PACKET_STATUS_5
};

typedef struct
{
	uint8_t PolePairs;
} vescParam;

typedef struct
{
	float current;
	float speed;
	float angle;
	float duty;
	volatile s16 handbrakeCurrent;
	volatile s32 brakeCurrent;
	float angleABS;
} vescValue;

typedef struct
{
	bool timeoutFlag;
	bool stuckFlag;
} vescStatus;

typedef struct
{
	bool timeoutDect;
	bool stuckDect;
	bool releaseWhenStuck;
	uint8_t currentLimit;
} vescLimit;

typedef struct
{
	uint32_t lastRxTime;
	uint32_t timeoutTicks;
	uint32_t stuckCount;

	volatile u16 angleNow;
	volatile u16 anglePre;
	volatile s16 distance;
	volatile s32 position;
} vescArgum;

typedef struct
{
	volatile bool enable;
	volatile bool begin;
	volatile bool brake;
	uint8_t mode;
	vescParam param;
	vescValue valueSet, valueReal;
	vescStatus status;
	vescLimit limit;
	vescArgum argum;
} vescMotor;

extern vescMotor VESCmotor[USE_VESC_NUM];

void vescInit(void);
void vescReceiveData_CAN2(CAN_RxHeaderTypeDef Rx_message1, uint8_t *RXmessage);
void vescSetPos(u8 controlID, float pos, bool InConGrpFlag);
void vescSetSpeed(u8 controlID, float speed, bool InConGrpFlag);
void vescSetCurrent(u8 controlID, float current, bool InConGrpFlag);
void vescSetDuty(u8 controlID, float duty, bool InConGrpFlag);
void vescSetHandbrakeCurrent(u8 controlID, float duty, bool InConGrpFlag);
void vescIfTimeout(u8 id);
void vescIfStuck(u8 id);
void vescFunc(void);

#endif

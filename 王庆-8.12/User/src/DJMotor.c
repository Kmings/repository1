#include "DJmotor.h"

DJMotor DJmotor[USE_DJ_NUM];
uint8_t TXmessage[8] ;
/**
 * @brief 电机参数初始化
 *
 */
void DJMotorInit()
{
    DJMotorParam Motor2006Param, Motor3508Param;
    DJMotorArgum argum;
    DJMotorLimit limit;
    DJMotorStatus status;
    DJMotorError error;

    Motor2006Param.StdID = 0x200; // todo：改
    Motor2006Param.IDE = CAN_ID_STD;
    Motor2006Param.RTR = CAN_RTR_DATA;
    Motor2006Param.DLC = 8;
    Motor2006Param.reductionRatio = M2006_RATIO;
	/*--------------------------齿轮传动比-----------------------*/
    Motor2006Param.GearRatio = 5.0f;        // todo：改
	
    Motor2006Param.pulsePerRound = 8191; // 电机转过一圈输出的脉冲数，8191与360°对应
    Motor2006Param.currentLimit = 4500;

    Motor3508Param.StdID = 0x200; // todo:改
    Motor3508Param.IDE = CAN_ID_STD;
    Motor3508Param.RTR = CAN_RTR_DATA;
    Motor3508Param.DLC = 8;
    Motor3508Param.reductionRatio = M3508_RATIO;
    Motor3508Param.GearRatio = 1; // todo：改
    Motor3508Param.pulsePerRound = 8191;
    Motor3508Param.currentLimit = 10000;

    argum.counter = 0;
    argum.lastRxTime = 0;
    argum.pawLockCnt = 0;
    argum.pulseLock = 0;
    argum.zeroCnt = 0;
    argum.maxDistance = 1500;

    limit.currentLimitFlag = true;
    limit.isLooseStuck = false;
    // 位置模式
    limit.maxAngle = 270;
    limit.minAngle = -270;
    limit.posAngleLimitFlag = false;
    limit.posRPMFlag = true;
    limit.posRPMLimit = 2000;
    // 速度模式
    limit.RPMLimitFlag = false;
    // 寻零模式
    limit.zeroCurrent = 3000;
    limit.zeroSpeed = 500;

    status.arrivedFlag = false;
    status.isSetZero = true;
    status.overtimeFlag = false;
    status.zeroFlag = false;
    status.stuckFlag = false;

    error.releaseWhenStuckFlag = false;
    error.stuckCount = 0;
    error.StuckDetectFlag = true;
    error.timeoutCount = 0;
    error.TimeoutDetectFlag = true;

    for (int i = 0; i < USE_DJ_NUM; i++)
    {
        DJmotor[i].mode = DJ_POSITION;
        DJmotor[i].begin = false;
        DJmotor[i].enable = false;
        DJmotor[i].argum = argum;
        DJmotor[i].limit = limit;
        DJmotor[i].statusFlag = status;
        DJmotor[i].error = error;
        DJmotor[i].valueNow.pulseTotal = 0;
        DJmotor[i].valuePre.pulseRead = 0;
    }
    for (uint8_t i = 0; i < USE_M2006_NUM; i++)
    {
        DJmotor[i].ID = i + 1;
        DJmotor[i].mode = DJ_POSITION;
        DJmotor[i].param = Motor2006Param;
    }
    // for (uint8_t i = 0; i < USE_M3508_NUM; i++)
    // {
    //     DJmotor[i].ID = USE_M2006_NUM + i + 1;
    //     DJmotor[i].mode = DJ_POSITION;
    //     DJmotor[i].param = Motor3508Param;
    // }

    // todo: 根据实际情况修改
    PIDInit(&DJmotor[0].posPID, 1.3, 0.2, 0, 0); // pid 2006为 1.3,0.3,0   3508为 2,0.08,0
    PIDInit(&DJmotor[0].rpmPID, 8, 0.5, 0, 0);

    PIDInit(&DJmotor[1].posPID, 1.3, 0.2, 0, 0);
    PIDInit(&DJmotor[1].rpmPID, 8, 0.5, 0, 0);

    PIDInit(&DJmotor[2].posPID, 1.3, 0.2, 0, 0);
    PIDInit(&DJmotor[2].rpmPID, 8, 0.5, 0, 0);

    PIDInit(&DJmotor[3].posPID, 1.3, 0.3, 0, 0);
    PIDInit(&DJmotor[3].rpmPID, 8, 0.5, 0, 0);

//    PIDInit(&DJmotor[4].posPID, 1.3, 0.3, 0, 0);
//    PIDInit(&DJmotor[4].rpmPID, 8, 0.5, 0, 0);

//    PIDInit(&DJmotor[5].posPID, 1.3, 0.3, 0, 0);
//    PIDInit(&DJmotor[5].rpmPID, 8, 0.5, 0, 0);

//    PIDInit(&DJmotor[6].posPID, 1.3, 0.3, 0, 0);
//    PIDInit(&DJmotor[6].rpmPID, 8, 0.5, 0, 0);
//		
//    PIDInit(&DJmotor[7].posPID, 1.3, 0.3, 0, 0);
//    PIDInit(&DJmotor[7].rpmPID, 8, 0.5, 0, 0);
}

/**
 * @brief 设置零点函数
 *
 * @param motor
 */
void DJSetZero(DJMotorPointer motor)
{
    motor->statusFlag.isSetZero = false;
    motor->valueNow.angle = 0;
    motor->valueNow.pulseTotal = 0;
    motor->argum.pulseLock = 0;
}

/**
 * @brief 接收电调的反馈数据
 *
 * @param Rxmsg0 接受的数据
 */	

void DJReceiveData_Can2(CAN_RxHeaderTypeDef Rxmsg0, uint8_t *data)
{
    if (Rxmsg0.StdId >= 0x201 && Rxmsg0.StdId <= 0x208)
    {
        uint8_t ID = Rxmsg0.StdId - 0x200;
        for (int k = 0; k < USE_DJ_NUM; k++)
        {
            if (ID == DJmotor[k].ID)
            {
                DJmotor[k].valueNow.pulseRead = ((data[0] << 8) | data[1]);
                DJmotor[k].valueNow.speed = ((data[2] << 8) | data[3]); // 电调反馈的速度是轴前的速度
                DJmotor[k].valueNow.current = ((data[4] << 8) | data[5]);
                if (DJmotor[k].param.reductionRatio == M3508_RATIO)
                {
                    DJmotor[k].valueNow.realSpeed = (float)DJmotor[k].valueNow.speed; //??  // 电机转过一圈输出的脉冲数，8191与360°对应 * 0.0263157
                    DJmotor[k].valueNow.temperature = data[6];
                    DJmotor[k].valueNow.currentA = (float)DJmotor[k].valueNow.current * 0.0012207f; // 这里应该是 读取的电流/16384（电调每最大输出电流）*20（电调最大实际电流）           }
                }
                else
                {
                    DJmotor[k].valueNow.currentA = (float)DJmotor[k].valueNow.current * 0.001f; // 这里应该是 读取的电流/10000*10
                }
                // 角度解算
                DJAngleCulculate(&DJmotor[k]);
                DJmotor[k].argum.lastRxTime = 0; // 接收到报文就将其置零，用来判断是否超时
            }
        }
    }
}

/**
 * @brief 进行角度解算
 *
 * @param motor 电机指针变量
 */
void DJAngleCulculate(DJMotorPointer motor)
{
    motor->valueNow.pulseDistanse = motor->valueNow.pulseRead - motor->valuePre.pulseRead; // 计算每次的脉冲数之差，对应每次转过的角度
    motor->valuePre = motor->valueNow;

    if (ABS(motor->valueNow.pulseDistanse) > 4096) // 用来判断电机应该正转还是反转  
    {
        motor->valueNow.pulseDistanse = motor->valueNow.pulseDistanse - PlusOrMinus(motor->valueNow.pulseDistanse) * motor->param.pulsePerRound;
    } // pulseDistance = 5000
    motor->valueNow.pulseTotal += motor->valueNow.pulseDistanse;
    motor->valueNow.angle = motor->valueNow.pulseTotal * 360 / (motor->param.pulsePerRound * motor->param.GearRatio * motor->param.reductionRatio);

    if (motor->begin)
    {
        motor->argum.pulseLock = motor->valueNow.pulseTotal;
    }

    if (motor->statusFlag.isSetZero)
    {
        DJSetZero(motor); // 归零函数
    }
}

/**
 * @brief 锁位置
 *
 * @param motor
 */
void DJLockPosition(DJMotorPointer motor)
{
    motor->posPID.SetVal = motor->argum.pulseLock;
    motor->posPID.CurVal = motor->valueNow.pulseTotal;

    motor->rpmPID.SetVal = PIDCaculateDelta(&(motor->posPID)); // 位置环的输出作为速度环的输入
    motor->rpmPID.CurVal = motor->valueNow.speed;

    motor->valueSet.current += PIDCaculateDelta(&(motor->rpmPID)); // 速度环的输出作为电流环的输入
    PEAK(motor->valueSet.current, 3000);                           // 自锁的电流限制，如果大于3000可能会堵转
}

/**
 * @brief 速度模式
 *
 * @param motor
 */
void DJSpeedMode(DJMotorPointer motor)
{
    motor->rpmPID.SetVal = motor->valueSet.speed;
    motor->rpmPID.CurVal = motor->valueNow.speed;
    motor->valueSet.current += PIDCaculateDelta(&(motor->rpmPID));
}

/**
 * @brief 位置模式，将设定角度转换为脉冲数进行计算，设定的角度单位为degree
 *
 * @param motor
 */
void DJPositionMode(DJMotorPointer motor)
{
    motor->valueSet.pulseTotal = motor->valueSet.angle * motor->param.GearRatio * motor->param.pulsePerRound * motor->param.reductionRatio * 0.0027777f; // 将目标角度转换为电机脉冲数 0.0027777 = 1/360

    if (motor->limit.posAngleLimitFlag == true) // 首先看位置限制标志位是否启用，若启用，则将设定位置与限制位置进行判断
    {
        int32_t maxPulse = motor->limit.maxAngle * motor->param.GearRatio * motor->param.pulsePerRound * motor->param.reductionRatio / 360; // 最大脉冲数限制
        int32_t minPulse = motor->limit.minAngle * motor->param.GearRatio * motor->param.pulsePerRound * motor->param.reductionRatio / 360; // 最小脉冲数限制
        // motor->posPID.SetVal = (motor->valueNow.pulseTotal > maxPulse) ? maxPulse : (motor->valueSet.pulseTotal > minPulse ? motor->valueSet.pulseTotal : minPulse);
        if (motor->valueSet.pulseTotal > maxPulse) // 位置模式下的角度限制
        {
            motor->posPID.SetVal = maxPulse;
        }
        else if (motor->valueSet.pulseTotal < minPulse)
        {
            motor->posPID.SetVal = minPulse;
        }
        else
        {
            motor->posPID.SetVal = motor->valueSet.pulseTotal;
        }
    }
    else
    {
        motor->posPID.SetVal = motor->valueSet.pulseTotal;
    }
    motor->posPID.CurVal = motor->valueNow.pulseTotal;

    motor->rpmPID.SetVal = PIDCaculateDelta(&(motor->posPID)); // 位置环的pid输出作为速度环的输入，若速度有限制，则对设定速度和限制速度进行判断
    if (motor->limit.posRPMFlag)                               // 位置模式下的速度限制
    {
        PEAK(motor->rpmPID.SetVal, motor->limit.posRPMLimit);
    }
    motor->rpmPID.CurVal = motor->valueNow.speed;

    motor->valueSet.current += PIDCaculateDelta(&(motor->rpmPID)); // 速度环的pid输出作为电流环的pid输入
}

/**
 * @brief 寻零模式
 *
 * @param motor 
 */
void DJZeroMode(DJMotorPointer motor)
{
    motor->rpmPID.SetVal = motor->limit.zeroSpeed; // 寻零模式限制速度
    motor->rpmPID.CurVal = motor->valueNow.speed;
    motor->valueSet.current += PIDCaculateDelta(&(motor->rpmPID)); // 速度环的输出作电流环的输入
    PEAK(motor->valueSet.current, motor->limit.zeroCurrent);       // 寻零模式限制电流

    if (ABS(motor->valueNow.pulseDistanse) < ZERO_DISTANCE_THRESHOLD) // 电机受到阻力（相当于相邻两次检测到的脉冲数小于某个阈值）就将其置为电机的零点
    {
        if (motor->argum.zeroCnt++ > 100)
        {
            motor->argum.zeroCnt = 0;

            motor->statusFlag.arrivedFlag = false; // 没什么用可以去掉
            motor->statusFlag.zeroFlag = true;     // 寻零模式结束后，zeroflag会被置一，需要手动置零
            motor->begin = false;
            DJSetZero(motor);
        }
    }
    else
    {
        motor->argum.zeroCnt = 0;
    }
}

/**
 * @brief 电流发送控制
 *
 * @param motor
 * @param id 电机id，不过前面设过了，好像可以不用
 */
void DJCurrentTransmit(DJMotorPointer motor, uint8_t i)
{
    uint8_t t;
    static CAN_TxHeaderTypeDef txMessage;
		uint32_t pTxMailbox = 0;
    PEAK(motor->valueSet.current, motor->param.currentLimit);
    if (motor->enable != ENABLE)
    {
        motor->valueSet.current = 0;
    }

    txMessage.IDE = CAN_ID_STD; ////千万不要忘记设置！！！不然很有可能疯转
    txMessage.RTR = CAN_RTR_DATA;
    txMessage.DLC = 8;

    if (i <= 4)
        txMessage.StdId = 0x200; // 电调ID为1~4时，控制ID为0x200
    else
        txMessage.StdId = 0x1FF; // 电调ID为5~8时，控制ID为0x1FF

    if (motor->ID <= 4)
    {
        t = ((motor->ID - 1) * 2);
    }
    else
    {
        t = ((motor->ID - 5) * 2);
    }
		EncodeS16Data(&motor->valueSet.current, &TXmessage[t]);
		ChangeDataByte(&TXmessage[t], &TXmessage[t + 1]); // 换位因为C620手册中高8位为DATA[0],低8位为DATA[1]...以此类推

    // if (USE_DJ_NUM <= 4 && i == USE_DJ_NUM)
    //     CAN_Transmit(CAN2, &txMessage); // 当使用dj电机数量小于等于4时，当计数器与使用电机数量相等时，can发送报文
    // if ((USE_DJ_NUM > 4 && i % 4 == 0) || (i == USE_DJ_NUM))
    //     CAN_Transmit(CAN2, &txMessage); // 当使用dj电机数量大于4时，计数器到4发送一条报文，到使用电机数目时发送一条报文，因为dj一条can报文可以控制4个电机

    if ((motor->ID == 4) || (motor->ID == 8))
	    HAL_CAN_AddTxMessage(&hcan2, &txMessage, TXmessage, &pTxMailbox);
}

void motorMonitor(DJMotorPointer motor)
{
    if (motor->enable && motor->begin)
    {
        if (motor->valueNow.pulseDistanse < 5 && motor->valueNow.current > 3000)
        {
            motor->error.stuckCount++;
            if (motor->error.stuckCount > 500)
            {
                motor->error.stuckCount = 0;
                motor->statusFlag.stuckFlag = true;
                Led.Stuck = true;
                if (motor->limit.isLooseStuck)
                {
                    motor->enable = false; // todo :蜂鸣器还没加
                }
            }
        }
        else
        {
            motor->error.stuckCount = 0;
            motor->statusFlag.stuckFlag = false;
            Led.Stuck = false;
        }
        if (motor->argum.lastRxTime++ > 20)
        {
            if (motor->error.timeoutCount++ > 10)
            {
                motor->error.timeoutCount = 0;
                motor->statusFlag.overtimeFlag = true;
                Led.Timeout = true; // todo :蜂鸣器还没加
            }
        }
    }
}

void DJFunc()
{
    for (uint8_t i = 0; i < USE_DJ_NUM; i++)
    {
        if (DJmotor[i].enable)
        {
            if (DJmotor[i].begin)
            {
                switch (DJmotor[i].mode)
                {
                case DJ_RPM:
                    DJSpeedMode(&DJmotor[i]);
                    break;
                case DJ_POSITION:
                    DJPositionMode(&DJmotor[i]);
                    break;
                case DJ_ZERO:
                    DJZeroMode(&DJmotor[i]);
                    break;
                case DJ_CURRENT:
                    break;
                default:
                    break;
                }
            }
            else
            {
                DJLockPosition(&DJmotor[i]);
            }
        }
        if (i <= 8)
            DJCurrentTransmit(&DJmotor[i], (i + 1));
    }
}

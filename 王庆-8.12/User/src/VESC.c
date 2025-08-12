#include "VESC.h"

vescParam VESC_x4255;
vescLimit limit;
vescMotor VESCmotor[USE_VESC_NUM];

/**
 * @brief 电机初始化
 *
 */
void vescInit()
{
    VESC_x4255.PolePairs = 14; // todo:遇到新的电机记得添加极对数

    limit.stuckDect = true;
    limit.timeoutDect = true;
    limit.releaseWhenStuck = true;
    limit.currentLimit = 20;

    for (u8 i = 0; i <= USE_VESC_NUM; i++)
    {
        VESCmotor[i].limit = limit;
        VESCmotor[i].param = VESC_x4255;
        VESCmotor[i].enable = false;
        VESCmotor[i].begin = false;
        VESCmotor[i].mode = VESC_POSITION;
        VESCmotor[i].valueSet.speed = 0;
        VESCmotor[i].valueSet.duty = 0.1; // 占空比*最大速度即为电机实际转速
        VESCmotor[i].valueSet.current = 0;
        VESCmotor[i].valueSet.handbrakeCurrent = 5;
        VESCmotor[i].valueSet.brakeCurrent = 5;
    }
}

/**
 * @brief 接收驱动器发来的消息
 *
 * @param Rx_message1
 */
void vescReceiveData_CAN2(CAN_RxHeaderTypeDef Rx_message1, uint8_t *RXmessage)
{
    int32_t index = 0;
    u8 id = (Rx_message1.ExtId & 0x0f) - 1;
    if (id == 5) // 3.0
    {
        if ((Rx_message1.IDE == CAN_ID_EXT) && (Rx_message1.RTR == CAN_RTR_DATA))
        {
            if ((Rx_message1.ExtId >> 8) == CAN_PACKET_STATUS)
            {
                VESCmotor[id].valueReal.speed = buffer_32_to_float(RXmessage, 1e0, &index) / VESCmotor[id].param.PolePairs;
                VESCmotor[id].valueReal.current = buffer_16_to_float(RXmessage, 1e1, &index);
            }
            if ((Rx_message1.ExtId >> 8) == CAN_PACKET_STATUS_4)
            {
                index += 6;
                VESCmotor[id].valueReal.angle = buffer_16_to_float(RXmessage, 5e1, &index);
            }
        }
    }
    else // 0.95
    {
        if ((Rx_message1.ExtId >> 8) == CAN_PACKET_STATUS)
        {
            VESCmotor[id].valueReal.speed = buffer_32_to_float(RXmessage, 1e0, &index);
            VESCmotor[id].valueReal.current = buffer_16_to_float(RXmessage, 1e1, &index);
            VESCmotor[id].valueReal.angle = buffer_16_to_float(RXmessage, 1e1, &index); // 一些通讯格式

            ChangeDataByte(&RXmessage[6], &RXmessage[7]);
            DecodeU16Data(&VESCmotor[id].argum.angleNow, &RXmessage[6]);

            VESCmotor[id].argum.distance = VESCmotor[id].argum.angleNow - VESCmotor[id].argum.anglePre;
            VESCmotor[id].argum.anglePre = VESCmotor[id].argum.angleNow;
            if (ABS(VESCmotor[id].argum.distance) > 1800)
                VESCmotor[id].argum.distance -= 3600; // 判断应该正转还是反转
            VESCmotor[id].argum.position += VESCmotor[id].argum.distance;
            VESCmotor[id].valueReal.angleABS = VESCmotor[id].argum.position / 10.0;

            VESCmotor[id].begin = 1; // 一旦接收到消息就将begin置位
        }
    }
    VESCmotor[id].argum.lastRxTime = 0;
}

/**
 * @brief 位置模式设置位置
 *
 * @param controlID 驱动器id
 * @param pos 设定的位置，单位degree
 * @param InConGrpFlag 是否加入can控制表
 */
void vescSetPos(u8 controlID, float pos, bool InConGrpFlag)
{
    int32_t sendIndex = 0;

    if (CAN_Queue_ifFull(&VESC_SendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }
    buffer_append_int32(VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].Data, (uint32_t)(pos * 1e6f), &sendIndex);
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].TxHeader.StdId = 0xf0000000 | controlID | ((uint32_t)CAN_PACKET_SET_POS << 8); // 报文id为1111
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].TxHeader.DLC = sendIndex;
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    VESC_SendQueue.Rear = IncVESCRear;
}

/**
 * @brief 速度模式设置速度
 *
 * @param controlID 驱动器id
 * @param speed 设定的速度，单位rpm
 * @param InConGrpFlag 是否加入can控制表
 */
void vescSetSpeed(u8 controlID, float speed, bool InConGrpFlag)
{
    int32_t sendIndex = 0;

    if (CAN_Queue_ifFull(&VESC_SendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }
    buffer_append_int32(VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].Data, speed, &sendIndex);
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].TxHeader.StdId = 0xf0000000 | controlID | ((uint32_t)CAN_PACKET_SET_RPM << 8);
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].TxHeader.DLC = sendIndex;
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    VESC_SendQueue.Rear = IncVESCRear;
}

/**
 * @brief 电流模式设置电流
 *
 * @param controlID
 * @param current 设定的电流，单位A
 * @param InConGrpFlag
 */
void vescSetCurrent(u8 controlID, float current, bool InConGrpFlag)
{
    int32_t sendIndex = 0;

    if (CAN_Queue_ifFull(&VESC_SendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }
    buffer_append_int32(VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(current * 1e3f), &sendIndex);
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].TxHeader.StdId = 0xf0000000 | controlID | ((uint32_t)CAN_PACKET_SET_CURRENT << 8);
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].TxHeader.DLC = sendIndex;
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    VESC_SendQueue.Rear = IncVESCRear;
}

/**
 * @brief 占空比模式，和速度模式差不多，duty * speed_MAX = speed_real
 *
 * @param controlID
 * @param duty 占空比
 * @param InConGrpFlag
 */
void vescSetDuty(u8 controlID, float duty, bool InConGrpFlag)
{
    int32_t sendIndex = 0;

    if (CAN_Queue_ifFull(&VESC_SendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }
    buffer_append_int32(VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].Data, (uint32_t)(duty * 100000), &sendIndex);
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].TxHeader.StdId = 0xf0000000 | controlID | ((uint32_t)CAN_PACKET_SET_DUTY << 8);
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].TxHeader.DLC = sendIndex;
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    VESC_SendQueue.Rear = IncVESCRear;
}

/**
 * @brief 锁位置，设置刹车电流
 *
 * @param controlID
 * @param handbrakeCurrent 手刹电流
 * @param InConGrpFlag
 */
void vescSetHandbrakeCurrent(u8 controlID, float handbrakeCurrent, bool InConGrpFlag)
{
    int32_t sendIndex = 0;

    if (CAN_Queue_ifFull(&VESC_SendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }
    buffer_append_int32(VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].Data, (uint32_t)(handbrakeCurrent * 1e3f), &sendIndex);
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].TxHeader.ExtId = 0xf0000000 | controlID | ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE << 8);
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].TxHeader.DLC = sendIndex;
    VESC_SendQueue.CAN_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    VESC_SendQueue.Rear = IncVESCRear;
}

/**
 * @brief 判断是否超时
 *
 * @param id 驱动器id
 */
void vescIfTimeout(u8 id)
{
    if (((VESCmotor[id].argum.lastRxTime++) > 1000) && (VESCmotor[id].enable))
    {
        VESCmotor[id].argum.timeoutTicks++;
        if (VESCmotor[id].argum.timeoutTicks > 50)
        {
            VESCmotor[id].status.timeoutFlag = 1;
        }
        else
        {
            VESCmotor[id].status.timeoutFlag = 0;
        }
    }
}

/**
 * @brief 判断是否堵转
 *
 * @param id 驱动器id
 */
void vescIfStuck(u8 id)
{
    if ((VESCmotor[id].valueReal.current) > 45 && (VESCmotor[id].enable))
    {
        VESCmotor[id].argum.stuckCount++;
        if (VESCmotor[id].argum.stuckCount > 3000)
        {
            VESCmotor[id].status.stuckFlag = true;
            if (VESCmotor[id].limit.releaseWhenStuck)
            {
                VESCmotor[id].enable = false;
            }
        }
        else
            VESCmotor[id].status.stuckFlag = false;
    }
}

/**
 * @brief vesc功能的实现
 *
 */
void vescFunc()
{
    for (u8 i = 0; i < 4; i++)
    {
        if (VESCmotor[i].enable)
        {
            if (VESCmotor[i].limit.stuckDect)
                vescIfStuck(i);
            if (VESCmotor[i].limit.timeoutDect)
                vescIfTimeout(i);
            if (VESCmotor[i].begin)
            {
                switch (VESCmotor[i].mode)
                {
                case VESC_RPM:
                    vescSetSpeed(i + 1, VESCmotor[i].valueSet.speed * VESCmotor[i].param.PolePairs, false);
                    break;
                case VESC_POSITION:
                    vescSetPos(i + 1, VESCmotor[i].valueSet.angle, false);
                    break;
                case VESC_CURRENT:
                    vescSetCurrent(i + 1, VESCmotor[i].valueSet.current, false);
                    break;
                case VESC_DUTY:
                    vescSetDuty(i + 1, VESCmotor[i].valueSet.duty, false);
                    break;
                case VESC_HANDBRAKE:
                    vescSetHandbrakeCurrent(i + 1, VESCmotor[i].valueSet.handbrakeCurrent, false);
                    break;
                default:
                    break;
                }
            }
            else
                vescSetHandbrakeCurrent(i + 1, VESCmotor[i].valueSet.handbrakeCurrent, false);
            //	vescSetPos(i + 1, 0.0, false);
        }
        else
            vescSetCurrent(i + 1, 0.0, false);
    }
}

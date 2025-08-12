#include "zdrive.h"

Zdrive Zmotor[4];
u8 sendFlag = 1;

void zdriveInit()
{
    for (int i = 0; i < 4; i++)
    {
        Zmotor[i].errClearFlag = false;
        Zmotor[i].param.gearRatio = 5.6f;      // 机构减速比
			
	/*----------------------------------减速比------------------------------*/
        Zmotor[i].param.reductionRatio = 1.f; // 电机减速比
			
        Zmotor[i].enable = false;
        Zmotor[i].begin = false;
        Zmotor[i].mode = Zdrive_Disable;
        Zmotor[i].valueSetNow.speed = 0;
        Zmotor[i].valueSetNow.angle = 0;
        Zmotor[i].argum.lockAngle = 0;
        Zmotor[i].PVTMode.PVTStage = 0;
        zdriveSetPresentPosition(0, i + 1);
    }
}

void zdriveSetMode(float mode, u8 id) // can.data[0] =  0x3D
{
    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }
    Zmotor[id - 1].mode = (ZdriveMode)mode;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x3D;
    memcpy(&(ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[1]), &mode, sizeof(float));

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 5;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void zdriveSetSpeed(float speed, u8 id) // can.data[0] = 0x45
{
    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    speed /= 60.f; // 单位转换rpm -> r/s
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x45;
    memcpy(&(ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[1]), &speed, sizeof(float));

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 5;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void zdriveSetPosition(float angle_degree, u8 id) // can.data[0] = 0x47
{
    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    angle_degree /= 360.f; // 驱动器设置角度以 圈 为单位
    angle_degree *= 5.6f;   // 轮子有140/25的减速比
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x47;
    memcpy(&(ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[1]), &angle_degree, sizeof(float));

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 5;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void zdriveSetPresentPosition(float angle_degree, u8 id) // 设置当前位置can.data[0] = 0x5F
{
    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    angle_degree /= 360.f; // 驱动器设置角度以 圈 为单位
    // angle_degree *= 140/25;  // 电机有10的减速比
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x5F;
    memcpy(&(ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[1]), &angle_degree, sizeof(float));

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 5;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void ZdriveSetPVT(float speed, float angle_degree, u8 id) // 速度（float）+位置（float）
{
    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    angle_degree /= 360.f; // 驱动器设置角度以 圈 为单位
    speed /= 60.f;         // 单位转换rpm -> r/s

    double temp;
    temp = cvtFloat2Double(speed, angle_degree);

    memcpy(ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data, &temp, sizeof(double));

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 8;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void ZdriveAskMode(u8 id) // 0x3C
{
    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x3C;

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC= 1;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void ZdriveAskCurrent(u8 id) // 0x52
{
    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x52;

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 1;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void ZdriveAskSpeed(u8 id) // 0x5C
{
    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x5C;

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 1;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void ZdriveAskPosition(u8 id) // 0x5E
{
    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x5E;

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 1;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void ZdriveAskErr(u8 id) // 0x40
{
    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x40;

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 1;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void ZdriveSavePresentPos(u8 id)
{
	if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x01;

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 5;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;
}

void zdriveReceiveHandler(CAN_RxHeaderTypeDef rx_msg, uint8_t *RXmessage)
{
    u8 controlID = rx_msg.StdId;
    u8 operationID = RXmessage[0];

    if (controlID > 8)
        return;
    Zmotor[controlID - 1].status.err = Zdrive_Well; // 没有反馈错误就是正常的
    switch (operationID)
    {
    case 0x5E: // position
    {
        memcpy(&(Zmotor[controlID - 1].valueRealLast.angle), &RXmessage[1], sizeof(float));
        Zmotor[controlID - 1].valueRealLast.angle *= (360.f / Zmotor[controlID - 1].param.reductionRatio);
        break;
    }
    case 0x52: // current
    {
        memcpy(&(Zmotor[controlID - 1].valueRealLast.current), &RXmessage[1], sizeof(float));
        break;
    }
    case 0x5C: // speed
    {
        memcpy(&(Zmotor[controlID - 1].valueRealLast.speed), &RXmessage[1], sizeof(float));
        Zmotor[controlID - 1].valueRealLast.speed *= 60; // 单位转换为rpm
        break;
    }
    case 0x3C: // mode
    {
        float tempMode;
        memcpy(&tempMode, &RXmessage[1], sizeof(float));
        Zmotor[controlID - 1].modeRead = (ZdriveMode)tempMode;
        break;
    }
    case 0x40: // err
    {
        float tempErr;
        memcpy(&tempErr, &RXmessage[1], sizeof(float));
        Zmotor[controlID - 1].status.err = (ZdriveErr)tempErr;
        break;
    }
		case 0x46: // pin
    {
        float tempPin;
        memcpy(&tempPin, &RXmessage[1], sizeof(float));
        Zmotor[controlID - 1].valueRealLast.pin = ((float)tempPin)*360/5.6f;
        break;
    }
    default:
        break;
    }
		
}

void ZdriveAskPin(u8 id)
{
	    if (CAN_Queue_ifFull(&ZDriveSendQueue))
    {
        Flag.CAN2SendQueueFULL++;
        return;
    }

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].Data[0] = 0x46;

    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.StdId = id;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].TxHeader.DLC = 1;
    ZDriveSendQueue.CAN_DataSend[ZDriveSendQueue.Rear].InConGrpFlag = false;
    ZDriveSendQueue.Rear = IncZDriveRear;

}

int Poscnt = 0;
void zdriveFunc()
{
    for (u8 i = 0; i < 4; i++)
    {
        if (Zmotor[i].enable)
        {
            if (Zmotor[i].begin)
            {
                Zmotor[i].argum.lockAngle = Zmotor[i].valueRealLast.angle;
                switch (Zmotor[i].mode)
                {
                case Zdrive_Disable:
                    if (Zmotor[i].modeRead != Zdrive_Disable)
                        zdriveSetMode((float)Zdrive_Disable, i + 1);
                    break;
                case Zdrive_Current:
                {
                    if (Zmotor[i].modeRead != Zdrive_Current)
                        zdriveSetMode((float)Zdrive_Current, i + 1);
                    break;
                }
                case Zdrive_Speed:
                {
                    if (Zmotor[i].modeRead != Zdrive_Speed)
                        zdriveSetMode((float)Zdrive_Speed, i + 1);
					else
						zdriveSetSpeed(Zmotor[i].valueSetNow.speed, i + 1);
                    break;
                }
                case Zdrive_Postion:
                {
                    if (Zmotor[i].modeRead != Zdrive_Postion)
                        zdriveSetMode((float)Zdrive_Postion, i + 1);
                    else
                    {
                        // if (Zmotor[i].PVTMode.PVTModeFlag == false)
                        //     zdriveSetPosition(Zmotor[i].valueSetNow.angle, i + 1);
                        // else
                        //     ZdriveSetPVT(Zmotor[i].valueSetNow.speed, Zmotor[i].valueSetNow.angle, i + 1);
                        // if ((Zmotor[i].valueSetNow.angle - Zmotor[i].valueSetLast.angle > 1) || (Zmotor[i].valueSetNow.angle - Zmotor[i].valueSetLast.angle < -1))
                        // {
                        //     Zmotor[i].valueSetLast.angle = Zmotor[i].valueSetNow.angle;
                        //     zdriveSetPosition(Zmotor[i].valueSetNow.angle, i + 1);
                        //     zdriveSetPosition(Zmotor[i].valueSetNow.angle, i + 1);
                        // }

                        //if (Zmotor[i].valueSetNow.angle != Zmotor[i].valueSetLast.angle)
                        //{
                        //    Zmotor[i].valueSetLast.angle = Zmotor[i].valueSetNow.angle;
												//	zdriveSetPosition(Zmotor[i].valueSetNow.angle, i + 1);
//														if(Poscnt--==0)
//														{
						if(Zmotor[i].valueSetNow.angle!=0)
						{
							if(ABS(ABS(Zmotor[i].valueSetNow.angle/Zmotor[i].valueRealLast.pin)-1)>1e-4f)
							zdriveSetPosition(Zmotor[i].valueSetNow.angle, i + 1);
						}
						else
						{
							if(ABS(Zmotor[i].valueRealLast.pin)>0.2f)
								zdriveSetPosition(Zmotor[i].valueSetNow.angle, i + 1);
						}									
                    }
                    break;
                }
                default:
                    break;
                }
            }
            else
            {
                // zdriveSetMode(Zdrive_Brake, i + 1);
                if (Zmotor[i].modeRead != Zdrive_Postion)
                    zdriveSetMode((float)Zdrive_Postion, i + 1);

				zdriveSetPosition(0, i + 1);

            }
        }
        else
        {
            if (Zmotor[i].modeRead != Zdrive_Disable)
                zdriveSetMode((float)Zdrive_Disable, i + 1);
        }

        ZdriveAskErr(i + 1);
        ZdriveAskMode(i + 1);
        ZdriveAskCurrent(i + 1);
        ZdriveAskSpeed(i + 1);
        ZdriveAskPosition(i + 1);
	    ZdriveAskPin(i + 1);
    }
}



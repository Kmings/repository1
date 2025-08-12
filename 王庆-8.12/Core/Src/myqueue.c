/**
 * @file queue.c
 * @brief
 *  注意这是机构的队列函数文件
 * @author ZhiHang_FU (fu_zhihang@126.com)
 * @version 1.0
 * @date
 * @copyright Copyright (c) 2023  the author of this program
 */
#include "myqueue.h"

CAN_SendQueueType CAN1_SendQueue, VESC_SendQueue, ZDriveSendQueue, DM_SendQueue,J60_SendQueue;                 // CAN口报文发送队列  其中VESC是通过CAN2发送的

/**
 * @brief Initializ CAN message queue
 */
void CAN_InitSendQueue(void)
{
    CAN1_SendQueue.Front = CAN1_SendQueue.Rear = 0;
    VESC_SendQueue.Front = VESC_SendQueue.Rear = 0;
    ZDriveSendQueue.Front = ZDriveSendQueue.Rear = 0;
    DM_SendQueue.Front = DM_SendQueue.Rear = 0;
	  J60_SendQueue.Front = J60_SendQueue.Rear = 0;
    CAN1_SendQueue.IDE = CAN_ID_EXT;
    VESC_SendQueue.IDE = CAN_ID_EXT;
    ZDriveSendQueue.IDE = CAN_ID_STD;
    DM_SendQueue.IDE = CAN_ID_STD;
	  J60_SendQueue.IDE = CAN_ID_STD;
}

/**
 * @brief Judge whether the CAN queue is empty
 * @param  can_queueMy Param doc
 * @return true
 * @return false
 */
bool CAN_Queue_ifEmpty(CAN_SendQueueType *can_queue)
{
    return ((can_queue->Front == can_queue->Rear));
}

/**
 * @brief Judge whether the CAN queue is full
 * @param  can_queueMy Param doc
 * @return true
 * @return false
 */
bool CAN_Queue_ifFull(CAN_SendQueueType *can_queue)
{
    return (((can_queue->Rear + 1) % CAN_QUEUESIZE) == can_queue->Front);
}

/**
 * @brief  Out of CAN queue
 * @param  CANx   select which CAN channel
 * @param  can_queue
 */
void CAN_QueueOut(CAN_HandleTypeDef *hcan,CAN_SendQueueType *queue)
{
	uint32_t pTxMailbox = 0;
  if (CAN_Queue_ifEmpty(queue))
	{
		Flag.CANSendQueueEMPTY = true;
		return;
	}
	if (hcan == &hcan1) // 回复主控
  {
		queue->CAN_DataSend[queue->Front].TxHeader.IDE= CAN_ID_EXT;
		queue->CAN_DataSend[queue->Front].TxHeader.StdId = 0x0;
  }
	else if (hcan == &hcan2)
	{
	if(queue->CAN_DataSend[queue->Front].TxHeader.IDE == CAN_ID_EXT)
		if((queue->CAN_DataSend[queue->Front].TxHeader.ExtId & 0xf0000000) == 0xf0000000)
			queue->CAN_DataSend[queue->Front].TxHeader.ExtId &= 0x0fffffff;	
	}
	queue->CAN_DataSend[queue->Front].TxHeader.RTR = CAN_RTR_DATA;
  HAL_CAN_AddTxMessage(hcan, &queue->CAN_DataSend[queue->Front].TxHeader, queue->CAN_DataSend[queue->Front].Data, &pTxMailbox);
	queue->Front= (queue->Front+1) % CAN_QUEUESIZE;
}//将元素从队列中取出并发送

void CAN1_enQueue(int dlc, u32 id, u8 rxmsg[8])
{
	if (CAN_Queue_ifFull(&CAN1_SendQueue))
	 {
		 Flag.CAN1SendQueueFULL++;
		 return;
	 }
  else
		{
		CAN1_SendQueue.CAN_DataSend[CAN1_SendQueue.Rear].TxHeader.DLC = dlc;
		CAN1_SendQueue.CAN_DataSend[CAN1_SendQueue.Rear].TxHeader.ExtId = id;
		memcpy(CAN1_SendQueue.CAN_DataSend[CAN1_SendQueue.Rear].Data, rxmsg, dlc);
		CAN1_SendQueue.Rear = (CAN1_SendQueue.Rear + 1) % CAN_QUEUESIZE;
		}
}

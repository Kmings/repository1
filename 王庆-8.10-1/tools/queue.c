#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void queueInit(queue *q)
{
    q->Front = 0;
    q->Rear = 0;
    printf("Queue size: %d (compile-time)\n", QUEUE_SIZE); // Add this line
    for(int i=0; i<QUEUE_SIZE; i++){
        q->data[i] = 0;
    }
    printf("Queue init\r\n");
}

bool queueEmpty(queue *q)
{
    return q->Front == q->Rear;
}

bool queueFull(queue *q)
{
    return (q->Rear + 1) % QUEUE_SIZE == q->Front;
}

uint8_t queueSize(queue *q)
{
    return (q->Rear - q->Front + QUEUE_SIZE) % QUEUE_SIZE;
}

void queueEnqueue(queue *q, uint8_t data)
{
    if (queueFull(q))
    {
        printf("queue is full\n");
        return;
    }
    q->data[q->Rear] = data;
    q->Rear = (q->Rear + 1) % QUEUE_SIZE;
}

uint8_t queueDequeue(queue *q)
{
    if (queueEmpty(q))
    {
        printf("queue is empty\n");
        return 0;
    }
    uint8_t data = q->data[q->Front];
    q->Front = (q->Front + 1) % QUEUE_SIZE;
    return data;
}

void queuePrint(queue *q)
{
    if (queueEmpty(q))
    {
        printf("queue is empty\n");
        return;
    }
    for (uint8_t i = q->Front; i != q->Rear; i = (i + 1) % QUEUE_SIZE)
    {
        printf("%d ", q->data[i]);
    }
    printf("\n");
}
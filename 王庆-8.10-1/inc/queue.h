#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define QUEUE_SIZE 21  // 将20改为21，实际可存储20个元素
typedef struct
{
    uint8_t data[QUEUE_SIZE];
    uint8_t Front;
    uint8_t Rear;
} queue;

void queueInit(queue *q);
bool queueEmpty(queue *q);
bool queueFull(queue *q);
uint8_t queueSize(queue *q);
void queueEnqueue(queue *q, uint8_t data);
uint8_t queueDequeue(queue *q);
void queuePrint(queue *q);

#endif
#include <stdio.h>
#include "main.h"
#include "queue.h"
#include "stack.h"

queue q;
stack s;

int main(int argc, char const *argv[]) {
    printf("队列的演示：\n");

    queueInit(&q);
    for(int i = 0; i < 20; i++)
    {
        queueEnqueue(&q, i);
    }
    printf("queue size: %d\n", queueSize(&q));
    queuePrint(&q);
    for(int i = 0; i < 20; i++)
    {
        printf("dequeue: %d\n", queueDequeue(&q));
    }
    printf("queue size: %d\n", queueSize(&q));
    queuePrint(&q);

    printf("\r\n栈的演示：\n");

    stackInit(&s, 50);
    float val1 = 2.5f, result1 = 0;
    uint8_t val2 = 12, result2 = 0;
    stackPush(&s, &val1, sizeof(float));
    stackPush(&s, &val2, sizeof(uint8_t));
    // 正确顺序：后入先出
    stackPop(&s, &result2, sizeof(uint8_t));  // 先弹出最后入栈的uint8_t
    stackPop(&s, &result1, sizeof(float));    // 再弹出先入栈的float
    printf("result1: %.2f\n", result1);
    printf("result2: %d\n", result2);
    stackFree(&s);
    return 0;
}
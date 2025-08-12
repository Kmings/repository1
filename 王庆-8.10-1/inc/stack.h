#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint8_t *base;  // 栈底
    uint8_t *top;   // 栈顶
    uint32_t size;  // 栈大小
} stack;

void stackInit(stack *s, int size);
bool stackEmpty(stack *s);
bool stackFull(stack *s);
uint32_t stackSize(stack *s);
bool stackPush(stack *s, void *data, uint32_t size);
bool stackPop(stack *s, void *data, uint32_t size);
void stackFree(stack *s);

#endif
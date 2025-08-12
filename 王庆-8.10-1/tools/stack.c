#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void stackInit(stack *s, int size)
{
    s->base = (uint8_t *)malloc(size) + size;  // 栈底指向分配内存的高地址端
    s->top = s->base;                          // 初始时栈顶=栈底
    s->size = size;
}

bool stackFull(stack *s)
{
    return (s->base - s->top) >= s->size;  // 已用空间=栈底-栈顶
}

uint32_t stackSize(stack *s)
{
    return s->base - s->top;  // 栈底-栈顶=当前元素数量
}

bool stackEmpty(stack *s)
{
    return s->base == s->top;
}

bool stackPush(stack *s, void *data, uint32_t size)
{
    if (stackFull(s)){
        return false;
    }
    s->top -= size;                  // 栈顶向下移动
    memcpy(s->top, data, size);      // 在栈顶位置写入数据
    return true;
}

bool stackPop(stack *s, void *data, uint32_t size)
{
    if (stackEmpty(s)){
        return false;
    }
    memcpy(data, s->top, size);      // 从栈顶位置读取数据
    s->top += size;                  // 栈顶向上移动
    return true;
}

void stackFree(stack *s)
{
    free(s->base - s->size);  // 释放malloc返回的原始指针（base - size）
    s->base = NULL;
    s->top = NULL;
    s->size = 0;
}

